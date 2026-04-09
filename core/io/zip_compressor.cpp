/*************************************************************************/
/*  zip_compressor.cpp                                                   */
/*************************************************************************/
/*                         This file is part of:                         */
/*                          PANDEMONIUM ENGINE                           */
/*             https://github.com/Relintai/pandemonium_engine            */
/*************************************************************************/
/* Copyright (c) 2022-present Péter Magyar.                              */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#ifdef MINIZIP_ENABLED

#include "zip_compressor.h"

#include "core/os/dir_access.h"
#include "core/os/file_access.h"
#include "core/os/os.h"

Error ZipCompressor::open(const String &p_file, const OpenMode p_open_mode) {
	if (_internal_mode != INTERNAL_MODE_UNINITIALIZED) {
		close();
	}

	if (p_open_mode == OPEN_MODE_UNZIP) {
		_dst_f = nullptr;
		_io_dst = zipio_create_io_from_file(&_dst_f);

		_unzip_file = unzOpen2(p_file.utf8().get_data(), &_io_dst);

		if (!_unzip_file) {
			return ERR_FILE_CANT_OPEN;
		}

		_internal_mode = INTERNAL_MODE_UNZIP;
	} else {
		_dst_f = nullptr;
		_io_dst = zipio_create_io_from_file(&_dst_f);

		int append = 0;

		switch (p_open_mode) {
			case OPEN_MODE_UNZIP:
			case OPEN_MODE_CREATE:
				append = APPEND_STATUS_CREATE;
				break;
			case OPEN_MODE_CREATE_AFTER:
				append = APPEND_STATUS_CREATEAFTER;
				break;
			case OPEN_MODE_ADD_IN_ZIP:
				append = APPEND_STATUS_ADDINZIP;
				break;
		}

		_zip = zipOpen2(p_file.utf8().get_data(), append, nullptr, &_io_dst);

		if (!_zip) {
			return ERR_FILE_CANT_OPEN;
		}

		_internal_mode = INTERNAL_MODE_ZIP;
	}

	return OK;
}
void ZipCompressor::close() {
	if (_internal_mode == INTERNAL_MODE_UNINITIALIZED) {
		return;
	}

	if (_internal_mode == INTERNAL_MODE_UNZIP) {
		unzClose(_unzip_file);
	} else {
		zipClose(_zip, nullptr);
	}

	_internal_mode = INTERNAL_MODE_UNINITIALIZED;
}

// Zip creation

void ZipCompressor::zip_open_new_file_in_zip(const String &p_file_path, const uint32_t p_mode) {
	ERR_FAIL_COND(_internal_mode != INTERNAL_MODE_ZIP);

	OS::Time time = OS::get_singleton()->get_time();
	OS::Date date = OS::get_singleton()->get_date();

	zip_fileinfo zipfi;
	zipfi.tmz_date.tm_hour = time.hour;
	zipfi.tmz_date.tm_mday = date.day;
	zipfi.tmz_date.tm_min = time.min;
	zipfi.tmz_date.tm_mon = date.month - 1; // Note: "tm" month range - 0..11, Pandemonium month range - 1..12, http://www.cplusplus.com/reference/ctime/tm/
	zipfi.tmz_date.tm_sec = time.sec;
	zipfi.tmz_date.tm_year = date.year;
	zipfi.dosDate = 0;
	// 0120000: symbolic link type
	// 0100000: regular file type
	// 0000755: permissions rwxr-xr-x
	// 0000644: permissions rw-r--r--
	zipfi.external_fa = (p_mode << 16L) | !(p_mode & 0200);
	zipfi.internal_fa = 0;

	zipOpenNewFileInZip4(_zip,
			p_file_path.utf8().get_data(),
			&zipfi,
			nullptr,
			0,
			nullptr,
			0,
			nullptr,
			Z_DEFLATED,
			Z_DEFAULT_COMPRESSION,
			0,
			-MAX_WBITS,
			DEF_MEM_LEVEL,
			Z_DEFAULT_STRATEGY,
			nullptr,
			0,
			0x0314, // "version made by", 0x03 - Unix, 0x14 - ZIP specification version 2.0, required to store Unix file permissions
			0);
}
void ZipCompressor::zip_write_file_to_in_file_in_zip(const String &p_file_path) {
	ERR_FAIL_COND(_internal_mode != INTERNAL_MODE_ZIP);

	FileAccessRef fa = FileAccess::open(p_file_path, FileAccess::READ);
	if (!fa) {
		ERR_PRINT(vformat(TTR("Could not open file to read from path \"%s\"."), p_file_path));
		return;
	}

	const int bufsize = 16384;
	uint8_t buf[bufsize];

	while (true) {
		uint64_t got = fa->get_buffer(buf, bufsize);
		if (got == 0) {
			break;
		}
		zipWriteInFileInZip(_zip, buf, got);
	}
}
void ZipCompressor::zip_write_data_to_in_file_in_zip(const PoolByteArray &p_data) {
	ERR_FAIL_COND(_internal_mode != INTERNAL_MODE_ZIP);

	PoolByteArray::Read r = p_data.read();

	zipWriteInFileInZip(_zip, r.ptr(), p_data.size());
}
void ZipCompressor::zip_write_text_to_in_file_in_zip(const String &p_text) {
	ERR_FAIL_COND(_internal_mode != INTERNAL_MODE_ZIP);

	zipWriteInFileInZip(_zip, p_text.utf8().get_data(), p_text.utf8().size());
}
void ZipCompressor::zip_close_file_in_zip() {
	ERR_FAIL_COND(_internal_mode != INTERNAL_MODE_ZIP);

	zipCloseFileInZip(_zip);
}

// Unzipping

Error ZipCompressor::unzip_go_to_first_file() {
	ERR_FAIL_COND_V(_internal_mode != INTERNAL_MODE_UNZIP, ERR_UNCONFIGURED);

	if (_unzip_file_open) {
		unzip_close_current_file();
	}

	int ret = unzGoToFirstFile(_unzip_file);

	if (ret == UNZ_OK) {
		_unzip_load_file_info();
		return OK;
	}

	return ERR_DOES_NOT_EXIST;
}
Error ZipCompressor::unzip_next_file() {
	ERR_FAIL_COND_V(_internal_mode != INTERNAL_MODE_UNZIP, ERR_UNCONFIGURED);

	if (_unzip_file_open) {
		unzip_close_current_file();
	}

	int ret = unzGoToNextFile(_unzip_file);

	if (ret == UNZ_END_OF_LIST_OF_FILE) {
		return ERR_FILE_EOF;
	}

	if (ret == UNZ_OK) {
		_unzip_load_file_info();
		return OK;
	}

	return ERR_DOES_NOT_EXIST;
}
Error ZipCompressor::unzip_locate_file(const String &p_file_name, const bool p_case_sensitive) {
	ERR_FAIL_COND_V(_internal_mode != INTERNAL_MODE_UNZIP, ERR_UNCONFIGURED);

	if (_unzip_file_open) {
		unzip_close_current_file();
	}

	int ret = unzLocateFile(_unzip_file, p_file_name.utf8().get_data(), p_case_sensitive ? 1 : 2);

	if (ret == UNZ_OK) {
		_unzip_load_file_info();
		return OK;
	}

	return ERR_DOES_NOT_EXIST;
}

String ZipCompressor::unzip_get_current_file_name() {
	return _unzip_current_file_name;
}
uint32_t ZipCompressor::unzip_get_current_file_internal_file_attributes() {
	return _unzip_current_file_info.internal_fa;
}
uint32_t ZipCompressor::unzip_get_current_file_external_file_attributes() {
	return _unzip_current_file_info.external_fa;
}
uint32_t ZipCompressor::unzip_get_current_file_compressed_size() {
	return _unzip_current_file_info.compressed_size;
}
uint32_t ZipCompressor::unzip_get_current_file_uncompressed_size() {
	return _unzip_current_file_info.uncompressed_size;
}
uint32_t ZipCompressor::unzip_get_current_file_crc() {
	return _unzip_current_file_info.crc;
}
uint32_t ZipCompressor::unzip_get_current_file_disk_num_start() {
	return _unzip_current_file_info.disk_num_start;
}

Error ZipCompressor::unzip_open_current_file() {
	ERR_FAIL_COND_V(_internal_mode != INTERNAL_MODE_UNZIP, ERR_UNCONFIGURED);

	if (_unzip_file_open) {
		unzip_close_current_file();
	}

	if (unzOpenCurrentFile(_unzip_file) == UNZ_OK) {
		_unzip_file_open = true;
		_unzip_load_file_info();
		return OK;
	}

	return ERR_FILE_CANT_OPEN;
}
void ZipCompressor::unzip_close_current_file() {
	ERR_FAIL_COND(_internal_mode != INTERNAL_MODE_UNZIP);

	if (!_unzip_file_open) {
		return;
	}

	unzCloseCurrentFile(_unzip_file);
	_unzip_file_open = false;
}
PoolByteArray ZipCompressor::unzip_read_file_chunk(const uint32_t p_max_length) {
	ERR_FAIL_COND_V(_internal_mode != INTERNAL_MODE_UNZIP, PoolByteArray());
	ERR_FAIL_COND_V(!_unzip_file_open, PoolByteArray());

	PoolByteArray data;
	data.resize(p_max_length);
	PoolByteArray::Write w = data.write();

	int size = unzReadCurrentFile(_unzip_file, w.ptr(), data.size());

	ERR_FAIL_COND_V(size < 0, PoolByteArray());

	data.resize(size);

	return data;
}
uint32_t ZipCompressor::unzip_get_file_position() {
	ERR_FAIL_COND_V(_internal_mode != INTERNAL_MODE_UNZIP, ERR_UNCONFIGURED);
	ERR_FAIL_COND_V(!_unzip_file_open, ERR_UNCONFIGURED);

	return unzGetOffset(_unzip_file);
}
void ZipCompressor::unzip_seek_file(const uint32_t p_pos) {
	ERR_FAIL_COND(_internal_mode != INTERNAL_MODE_UNZIP);
	ERR_FAIL_COND(!_unzip_file_open);

	ERR_FAIL_COND(unzSetOffset(_unzip_file, p_pos) != UNZ_OK);
}

void ZipCompressor::unzip_write_current_file_to_file(const String &p_file_path) {
	ERR_FAIL_COND(_internal_mode != INTERNAL_MODE_UNZIP);

	if (_unzip_file_open) {
		unzip_close_current_file();
	}

	Vector<uint8_t> data;
	data.resize(_unzip_current_file_info.uncompressed_size);

	// Read
	unzOpenCurrentFile(_unzip_file);
	unzReadCurrentFile(_unzip_file, data.ptrw(), data.size());
	unzCloseCurrentFile(_unzip_file);

	FileAccessRef f = FileAccess::open(p_file_path, FileAccess::WRITE);

	ERR_FAIL_COND_MSG(!f, "Can't open file from path '" + p_file_path + "'.");

	f->store_buffer(data.ptr(), data.size());

#ifndef WINDOWS_ENABLED
	FileAccess::set_unix_permissions(p_file_path, (_unzip_current_file_info.external_fa >> 16) & 0x01FF);
#endif
}
PoolByteArray ZipCompressor::unzip_get_current_file_data() {
	ERR_FAIL_COND_V(_internal_mode != INTERNAL_MODE_UNZIP, PoolByteArray());

	if (_unzip_file_open) {
		unzip_close_current_file();
	}

	PoolByteArray data;
	data.resize(_unzip_current_file_info.uncompressed_size);
	PoolByteArray::Write w = data.write();

	// Read
	unzOpenCurrentFile(_unzip_file);
	unzReadCurrentFile(_unzip_file, w.ptr(), data.size());
	unzCloseCurrentFile(_unzip_file);

	return data;
}

void ZipCompressor::_unzip_load_file_info() {
	char fname[16384];
	unzGetCurrentFileInfo(_unzip_file, &_unzip_current_file_info, fname, 16384, nullptr, 0, nullptr, 0);

	_unzip_current_file_name = String::utf8(fname).simplify_path();
}

// Helper methods
Error ZipCompressor::zip_folder(const String &p_path, const String &p_zip_file) {
	FileAccess *dst_f = nullptr;
	zlib_filefunc_def io_dst = zipio_create_io_from_file(&dst_f);
	zipFile zip = zipOpen2(p_zip_file.utf8().get_data(), APPEND_STATUS_CREATE, nullptr, &io_dst);

	_zip_folder_recursive(zip, p_path.path_ensure_end_slash(), "/");

	zipClose(zip, nullptr);

	return OK;
}

Error ZipCompressor::unzip_to_folder(const String &p_zip_file, const String &p_path) {
	// unzClose() will take care of closing the file stored in the unzFile,
	// so we don't need to `memdelete(fa)` in this method.
	FileAccess *fa = nullptr;
	zlib_filefunc_def io = zipio_create_io_from_file(&fa);

	unzFile pkg = unzOpen2(p_zip_file.utf8().get_data(), &io);
	if (!pkg) {
		return ERR_FILE_CANT_OPEN;
	}
	int ret = unzGoToFirstFile(pkg);

	DirAccessRef d = DirAccess::create(DirAccess::ACCESS_FILESYSTEM);
	if (!d->dir_exists(p_path)) {
		unzClose(pkg);
		return ERR_DOES_NOT_EXIST;
	}

	String contents_dir = p_path.trim_suffix("/").trim_suffix("\\");

	ret = unzGoToFirstFile(pkg);
	while (ret == UNZ_OK) {
		// Get filename.
		unz_file_info info;
		char fname[16384];
		unzGetCurrentFileInfo(pkg, &info, fname, 16384, nullptr, 0, nullptr, 0);

		String file_path(String::utf8(fname).simplify_path());

		String file = file_path.get_file();

		if (file.size() == 0) {
			ret = unzGoToNextFile(pkg);
			continue;
		}

		Vector<uint8_t> data;
		data.resize(info.uncompressed_size);

		// Read
		unzOpenCurrentFile(pkg);
		unzReadCurrentFile(pkg, data.ptrw(), data.size());
		unzCloseCurrentFile(pkg);

		String base_dir = file_path.get_base_dir().trim_suffix("/");

		if (base_dir != contents_dir && base_dir.begins_with(contents_dir)) {
			base_dir = base_dir.substr(contents_dir.length(), file_path.length()).trim_prefix("/");
			file = base_dir.plus_file(file);

			DirAccessRef da = DirAccess::create(DirAccess::ACCESS_FILESYSTEM);
			ERR_CONTINUE(!da);

			String output_dir = p_path.plus_file(base_dir);

			if (!DirAccess::exists(output_dir)) {
				Error mkdir_err = da->make_dir_recursive(output_dir);
				ERR_CONTINUE(mkdir_err != OK);
			}
		}

		String to_write = p_path.plus_file(file);
		FileAccessRef f = FileAccess::open(to_write, FileAccess::WRITE);

		if (!f) {
			ret = unzGoToNextFile(pkg);
			ERR_CONTINUE_MSG(true, "Can't open file from path '" + String(to_write) + "'.");
		}

		f->store_buffer(data.ptr(), data.size());

#ifndef WINDOWS_ENABLED
		FileAccess::set_unix_permissions(to_write, (info.external_fa >> 16) & 0x01FF);
#endif

		ret = unzGoToNextFile(pkg);
	}

	unzClose(pkg);

	return OK;
}

ZipCompressor::ZipCompressor() {
	_internal_mode = INTERNAL_MODE_UNINITIALIZED;
	_dst_f = NULL;
	_unzip_file_open = false;
	_unzip_file = NULL;
}

ZipCompressor::~ZipCompressor() {
}

void ZipCompressor::_bind_methods() {
	ClassDB::bind_method(D_METHOD("zip_folder", "path", "zip_file"), &ZipCompressor::zip_folder);
	ClassDB::bind_method(D_METHOD("unzip_to_folder", "zip_file", "path"), &ZipCompressor::unzip_to_folder);
}

void ZipCompressor::_zip_folder_recursive(zipFile &p_zip, const String &p_root_path, const String &p_folder) {
	String dir = p_root_path.plus_file(p_folder);

	DirAccess *da = DirAccess::open(dir);
	da->list_dir_begin();
	String f = da->get_next();
	while (f.empty()) {
		if (f == "." || f == "..") {
			f = da->get_next();
			continue;
		}

		if (da->is_link(f)) {
			OS::Time time = OS::get_singleton()->get_time();
			OS::Date date = OS::get_singleton()->get_date();

			zip_fileinfo zipfi;
			zipfi.tmz_date.tm_hour = time.hour;
			zipfi.tmz_date.tm_mday = date.day;
			zipfi.tmz_date.tm_min = time.min;
			zipfi.tmz_date.tm_mon = date.month - 1; // Note: "tm" month range - 0..11, Pandemonium month range - 1..12, http://www.cplusplus.com/reference/ctime/tm/
			zipfi.tmz_date.tm_sec = time.sec;
			zipfi.tmz_date.tm_year = date.year;
			zipfi.dosDate = 0;
			// 0120000: symbolic link type
			// 0000755: permissions rwxr-xr-x
			// 0000644: permissions rw-r--r--
			uint32_t _mode = 0120644;
			zipfi.external_fa = (_mode << 16L) | !(_mode & 0200);
			zipfi.internal_fa = 0;

			zipOpenNewFileInZip4(p_zip,
					p_folder.plus_file(f).utf8().get_data(),
					&zipfi,
					nullptr,
					0,
					nullptr,
					0,
					nullptr,
					Z_DEFLATED,
					Z_DEFAULT_COMPRESSION,
					0,
					-MAX_WBITS,
					DEF_MEM_LEVEL,
					Z_DEFAULT_STRATEGY,
					nullptr,
					0,
					0x0314, // "version made by", 0x03 - Unix, 0x14 - ZIP specification version 2.0, required to store Unix file permissions
					0);

			String target = da->read_link(f);
			zipWriteInFileInZip(p_zip, target.utf8().get_data(), target.utf8().size());
			zipCloseFileInZip(p_zip);
		} else if (da->current_is_dir()) {
			_zip_folder_recursive(p_zip, p_root_path, p_folder.plus_file(f));
		} else {
			//bool is_executable = (p_folder.ends_with("MacOS") && (f == p_pkg_name));

			OS::Time time = OS::get_singleton()->get_time();
			OS::Date date = OS::get_singleton()->get_date();

			zip_fileinfo zipfi;
			zipfi.tmz_date.tm_hour = time.hour;
			zipfi.tmz_date.tm_mday = date.day;
			zipfi.tmz_date.tm_min = time.min;
			zipfi.tmz_date.tm_mon = date.month - 1; // Note: "tm" month range - 0..11, Pandemonium month range - 1..12, http://www.cplusplus.com/reference/ctime/tm/
			zipfi.tmz_date.tm_sec = time.sec;
			zipfi.tmz_date.tm_year = date.year;
			zipfi.dosDate = 0;
			// 0100000: regular file type
			// 0000755: permissions rwxr-xr-x
			// 0000644: permissions rw-r--r--
			//uint32_t _mode = (is_executable ? 0100755 : 0100644);
			uint32_t _mode = 0100644;
			zipfi.external_fa = (_mode << 16L) | !(_mode & 0200);
			zipfi.internal_fa = 0;

			zipOpenNewFileInZip4(p_zip,
					p_folder.plus_file(f).utf8().get_data(),
					&zipfi,
					nullptr,
					0,
					nullptr,
					0,
					nullptr,
					Z_DEFLATED,
					Z_DEFAULT_COMPRESSION,
					0,
					-MAX_WBITS,
					DEF_MEM_LEVEL,
					Z_DEFAULT_STRATEGY,
					nullptr,
					0,
					0x0314, // "version made by", 0x03 - Unix, 0x14 - ZIP specification version 2.0, required to store Unix file permissions
					0);

			FileAccessRef fa = FileAccess::open(dir.plus_file(f), FileAccess::READ);
			if (!fa) {
				ERR_PRINT(vformat(TTR("Could not open file to read from path \"%s\"."), dir.plus_file(f)));
				return;
			}

			const int bufsize = 16384;
			uint8_t buf[bufsize];

			while (true) {
				uint64_t got = fa->get_buffer(buf, bufsize);
				if (got == 0) {
					break;
				}
				zipWriteInFileInZip(p_zip, buf, got);
			}

			zipCloseFileInZip(p_zip);
		}

		f = da->get_next();
	}

	da->list_dir_end();
	memdelete(da);
}

#endif
