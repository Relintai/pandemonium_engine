#ifndef PDF_DOCUMENT_H
#define PDF_DOCUMENT_H

/*************************************************************************/
/*  pdf_document.h                                                       */
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

#include "core/string/ustring.h"

#include "core/object/reference.h"

struct _HPDF_Doc_Rec;
typedef struct _HPDF_Doc_Rec *HPDF_Doc;

class PDFPage;

class PDFDocument : public Reference {
	GDCLASS(PDFDocument, Reference);

public:
	uint32_t new_document();
	void free_document();
	bool has_document();
	void free_document_all();

	Ref<PDFPage> page_get_current();
	Ref<PDFPage> page_get_index(const uint32_t p_index);
	Ref<PDFPage> page_add();
	Ref<PDFPage> page_insert(const Ref<PDFPage> &p_page);

	uint32_t set_pages_configuration(const uint32_t p_page_per_pages);
	//get_page_by_index();

	PoolByteArray get_contents();
	uint32_t save_to_file(const String &p_file);

	uint32_t get_status();
	uint32_t get_error_no();
	uint32_t get_error_detail_no();
	void reset_error();

	PDFDocument();
	~PDFDocument();

protected:
	static void _bind_methods();

	HPDF_Doc _doc;

	uint32_t _status;
};

#endif
