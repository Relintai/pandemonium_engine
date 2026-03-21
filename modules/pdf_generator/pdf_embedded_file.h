#ifndef PDF_EMBEDDED_FILE_H
#define PDF_EMBEDDED_FILE_H

/*************************************************************************/
/*  pdf_embedded_file.h                                                  */
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

#include "core/object/reference.h"

class PDFEmbeddedFile : public Reference {
	GDCLASS(PDFEmbeddedFile, Reference);

public:
#if 0

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_EmbeddedFile_SetName(HPDF_EmbeddedFile emfile,
			const char *name);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_EmbeddedFile_SetDescription(HPDF_EmbeddedFile emfile,
			const char *new_description);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_EmbeddedFile_SetSubtype(HPDF_EmbeddedFile emfile,
			const char *subtype);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_EmbeddedFile_SetAFRelationship(HPDF_EmbeddedFile emfile,
			HPDF_AFRelationship relationship);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_EmbeddedFile_SetSize(HPDF_EmbeddedFile emfile,
			HPDF_UINT64 size);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_EmbeddedFile_SetCreationDate(HPDF_EmbeddedFile emfile,
			HPDF_Date creationDate);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_EmbeddedFile_SetLastModificationDate(HPDF_EmbeddedFile emfile,
			HPDF_Date lastModificationDate);

#endif

	uint32_t get_status();

	PDFEmbeddedFile();
	~PDFEmbeddedFile();

	void *_get_hpdf_embedded_file() const;
	void _set_hpdf_embedded_file(void *p_embedded_file);

protected:
	static void _bind_methods();

	void *_embedded_file;

	uint32_t _status;
};

#endif
