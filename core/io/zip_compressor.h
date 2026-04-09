#ifndef ZIP_COMPRESSOR_H
#define ZIP_COMPRESSOR_H

/*************************************************************************/
/*  zip_compressor.h                                                     */
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

#include "core/object/reference.h"

#include "core/os/file_access.h"

#include "core/io/zip_io.h"

class ZipCompressor : public Reference {
	GDCLASS(ZipCompressor, Reference);

public:
	enum OpenMode {
		OPEN_MODE_CREATE = 0,
		OPEN_MODE_CREATE_AFTER,
		OPEN_MODE_ADD_IN_ZIP,
		OPEN_MODE_UNZIP,
	};

	Error open(const String &p_file, const OpenMode p_open_mode);
	void close();

	// Zip creation

	void zip_open_new_file_in_zip(const String &p_file_path);
	void zip_write_file_to_in_file_in_zip(const String &p_file_path);
	void zip_write_data_to_in_file_in_zip(const PoolByteArray &p_data);
	void zip_write_text_to_in_file_in_zip(const String &p_text);
	void zip_close_file_in_zip();

	/*
	zipOpenNewFileInZip4(p_zip, );
	zipWriteInFileInZip(p_zip, buf, got);
	zipCloseFileInZip(p_zip);
	*/

	// Unzipping

	void unzip_go_to_first_file();
	bool unzip_next_file();
	void unzip_read_current_file();
	void unzip_close_current_file();
	void unzip_get_current_file_size();
	void unzip_write_current_file_to_file();
	PoolByteArray unzip_get_current_file_data();

	/*
	int ret = unzGoToFirstFile(pkg);
	ret = unzGoToNextFile(pkg);
	unzOpenCurrentFile(pkg);
	unzReadCurrentFile(pkg, data.ptrw(), data.size());
	unzCloseCurrentFile(pkg);

	unzGetCurrentFileInfo(pkg, &info, fname, 16384, nullptr, 0, nullptr, 0);
	*/

	// Helper methods
	Error zip_folder(const String &p_path, const String &p_zip_file);
	Error unzip_to_folder(const String &p_zip_file, const String &p_path);

	ZipCompressor();
	~ZipCompressor();

protected:
	static void _bind_methods();

	// Helper for zip_folder()
	void _zip_folder_recursive(zipFile &p_zip, const String &p_root_path, const String &p_folder);

	enum InternalMode {
		INTERNAL_MODE_UNINITIALIZED,
		INTERNAL_MODE_UNZIP,
		INTERNAL_MODE_ZIP,
	};

	InternalMode _internal_mode;
};

VARIANT_ENUM_CAST(ZipCompressor::OpenMode);

#endif

#endif
