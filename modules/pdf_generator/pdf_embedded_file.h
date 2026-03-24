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

class PDFDate;

class PDFEmbeddedFile : public Reference {
	GDCLASS(PDFEmbeddedFile, Reference);

public:
	enum AFRelationship {
		AFRELATIONSHIP_SOURCE = 0,
		AFRELATIONSHIP_DATA,
		AFRELATIONSHIP_ALTERNATIVE,
		AFRELATIONSHIP_SUPPLEMENT,
		AFRELATIONSHIP_ENCRYPTEDPAYLOAD,
		AFRELATIONSHIP_FORMDATA,
		AFRELATIONSHIP_SCHEMA,
		AFRELATIONSHIP_UNSPECIFIED
	};

	uint32_t set_name(const String &p_name);
	uint32_t set_description(const String &p_description);
	uint32_t set_sub_type(const String &p_sub_type);
	uint32_t set_af_relationship(const AFRelationship p_relationship);
	uint32_t set_size(const uint64_t p_size);
	uint32_t set_creation_date(const Ref<PDFDate> &p_date);
	uint32_t set_last_modification_date(const Ref<PDFDate> &p_date);

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

VARIANT_ENUM_CAST(PDFEmbeddedFile::AFRelationship);

#endif
