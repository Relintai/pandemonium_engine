/*************************************************************************/
/*  pdf_document.cpp                                                     */
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

#include "pdf_document.h"

#include "core/log/logger.h"
#include "core/os/dir_access.h"
#include "core/os/file_access.h"

#include "hpdf.h"
#include "hpdf_doc.h"

uint32_t PDFDocument::new_document() {
	_status = HPDF_NewDoc(_doc);
	return _status;
}
void PDFDocument::free_document() {
	HPDF_FreeDoc(_doc);
}
bool PDFDocument::has_document() {
	return HPDF_HasDoc(_doc);
}
void PDFDocument::free_document_all() {
	HPDF_FreeDocAll(_doc);
}

uint32_t PDFDocument::set_pages_configuration(const uint32_t p_page_per_pages) {
	_status = HPDF_SetPagesConfiguration(_doc, (HPDF_UINT)p_page_per_pages);
	return _status;
}

PoolByteArray PDFDocument::get_contents() {
	uint32_t stream_size = HPDF_GetStreamSize(_doc);

	PoolByteArray data;
	data.resize((int)stream_size);

	PoolByteArray::Write w = data.write();

	uint32_t final_size;

	_status = HPDF_GetContents(_doc, w.ptr(), &final_size);

	w.release();

	data.resize(final_size);

	return data;
}
uint32_t PDFDocument::save_to_file(const String &p_file) {
	String abs_path = DirAccess::get_filesystem_abspath_for(p_file.simplify_path());

	_status = HPDF_SaveToFile(_doc, abs_path.utf8().get_data());
	return _status;

	/*
	FileAccess *f = FileAccess::open(p_file, FileAccess::WRITE);

	ERR_FAIL_COND(!f);

	uint32_t stream_size = HPDF_GetStreamSize(_doc);

	Vector<uint8_t> data;
	data.resize((int)stream_size);

	uint32_t final_size;
	_status = HPDF_GetContents(_doc, data.ptrw(), &final_size);
	data.resize(final_size);

	f->store_buffer(data.ptr(), final_size);
	f->close();
	*/
}

uint32_t PDFDocument::get_status() {
	return _status;
}
uint32_t PDFDocument::get_error_no() {
	return HPDF_GetError(_doc);
}
uint32_t PDFDocument::get_error_detail_no() {
	return HPDF_GetErrorDetail(_doc);
}
void PDFDocument::reset_error() {
	HPDF_ResetError(_doc);
}

PDFDocument::PDFDocument() {
	_doc = HPDF_New(NULL, NULL);

	if (!_doc) {
		PLOG_ERR("Couldn't create PDFDocument object!");
	}
}

PDFDocument::~PDFDocument() {
	if (_doc) {
		HPDF_Free(_doc);
	}
}

void PDFDocument::_bind_methods() {
	ClassDB::bind_method(D_METHOD("new_document"), &PDFDocument::new_document);
	ClassDB::bind_method(D_METHOD("free_document"), &PDFDocument::free_document);
	ClassDB::bind_method(D_METHOD("has_document"), &PDFDocument::has_document);
	ClassDB::bind_method(D_METHOD("free_document_all"), &PDFDocument::free_document_all);

	ClassDB::bind_method(D_METHOD("set_pages_configuration", "page_per_pages"), &PDFDocument::set_pages_configuration);

	ClassDB::bind_method(D_METHOD("get_contents"), &PDFDocument::get_contents);
	ClassDB::bind_method(D_METHOD("save_to_file", "file"), &PDFDocument::save_to_file);

	ClassDB::bind_method(D_METHOD("get_status"), &PDFDocument::get_status);
	ClassDB::bind_method(D_METHOD("get_error_no"), &PDFDocument::get_error_no);
	ClassDB::bind_method(D_METHOD("get_error_detail_no"), &PDFDocument::get_error_detail_no);

	//ClassDB::bind_method(D_METHOD("get_user_id"), &PDFDocument::get_user_id);
	//ClassDB::bind_method(D_METHOD("set_user_id", "val"), &PDFDocument::set_user_id);
	//ADD_PROPERTY(PropertyInfo(Variant::INT, "user_id"), "set_user_id", "get_user_id");
}
