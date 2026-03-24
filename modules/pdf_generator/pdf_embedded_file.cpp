/*************************************************************************/
/*  pdf_embedded_file.cpp                                                */
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

#include "pdf_embedded_file.h"

#include "pdf_date.h"

#include "hpdf.h"

uint32_t PDFEmbeddedFile::set_name(const String &p_name) {
	_status = HPDF_EmbeddedFile_SetName((HPDF_EmbeddedFile)_embedded_file, p_name.utf8().get_data());

	return _status;
}
uint32_t PDFEmbeddedFile::set_description(const String &p_description) {
	_status = HPDF_EmbeddedFile_SetDescription((HPDF_EmbeddedFile)_embedded_file, p_description.utf8().get_data());

	return _status;
}
uint32_t PDFEmbeddedFile::set_sub_type(const String &p_sub_type) {
	_status = HPDF_EmbeddedFile_SetSubtype((HPDF_EmbeddedFile)_embedded_file, p_sub_type.utf8().get_data());

	return _status;
}
uint32_t PDFEmbeddedFile::set_af_relationship(const AFRelationship p_relationship) {
	HPDF_AFRelationship relationship = static_cast<HPDF_AFRelationship>(p_relationship);

	_status = HPDF_EmbeddedFile_SetAFRelationship((HPDF_EmbeddedFile)_embedded_file, relationship);

	return _status;
}
uint32_t PDFEmbeddedFile::set_size(const uint64_t p_size) {
	_status = HPDF_EmbeddedFile_SetSize((HPDF_EmbeddedFile)_embedded_file, p_size);

	return _status;
}
uint32_t PDFEmbeddedFile::set_creation_date(const Ref<PDFDate> &p_date) {
	HPDF_Date date;

	if (p_date.is_valid()) {
		date.year = p_date->get_year();
		date.month = p_date->get_month();
		date.day = p_date->get_day();
		date.hour = p_date->get_hour();
		date.minutes = p_date->get_minutes();
		date.seconds = p_date->get_seconds();
		date.ind = static_cast<char>(p_date->get_ind());
		date.off_hour = p_date->get_off_hour();
		date.off_minutes = p_date->get_off_minutes();
	}

	_status = HPDF_EmbeddedFile_SetCreationDate((HPDF_EmbeddedFile)_embedded_file, date);

	return _status;
}
uint32_t PDFEmbeddedFile::set_last_modification_date(const Ref<PDFDate> &p_date) {
	HPDF_Date date;

	if (p_date.is_valid()) {
		date.year = p_date->get_year();
		date.month = p_date->get_month();
		date.day = p_date->get_day();
		date.hour = p_date->get_hour();
		date.minutes = p_date->get_minutes();
		date.seconds = p_date->get_seconds();
		date.ind = static_cast<char>(p_date->get_ind());
		date.off_hour = p_date->get_off_hour();
		date.off_minutes = p_date->get_off_minutes();
	}

	_status = HPDF_EmbeddedFile_SetLastModificationDate((HPDF_EmbeddedFile)_embedded_file, date);

	return _status;
}

uint32_t PDFEmbeddedFile::get_status() {
	return _status;
}

PDFEmbeddedFile::PDFEmbeddedFile() {
	_status = 0;

	_embedded_file = NULL;
}

PDFEmbeddedFile::~PDFEmbeddedFile() {
}

void *PDFEmbeddedFile::_get_hpdf_embedded_file() const {
	return _embedded_file;
}

void PDFEmbeddedFile::_set_hpdf_embedded_file(void *p_embedded_file) {
	_embedded_file = p_embedded_file;
}

void PDFEmbeddedFile::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_name", "name"), &PDFEmbeddedFile::set_name);
	ClassDB::bind_method(D_METHOD("set_description", "description"), &PDFEmbeddedFile::set_description);
	ClassDB::bind_method(D_METHOD("set_sub_type", "sub_type"), &PDFEmbeddedFile::set_sub_type);
	ClassDB::bind_method(D_METHOD("set_af_relationship", "relationship"), &PDFEmbeddedFile::set_af_relationship);
	ClassDB::bind_method(D_METHOD("set_size", "size"), &PDFEmbeddedFile::set_size);
	ClassDB::bind_method(D_METHOD("set_creation_date", "date"), &PDFEmbeddedFile::set_creation_date);
	ClassDB::bind_method(D_METHOD("set_last_modification_date", "date"), &PDFEmbeddedFile::set_last_modification_date);

	ClassDB::bind_method(D_METHOD("get_status"), &PDFEmbeddedFile::get_status);

	BIND_ENUM_CONSTANT(AFRELATIONSHIP_SOURCE);
	BIND_ENUM_CONSTANT(AFRELATIONSHIP_DATA);
	BIND_ENUM_CONSTANT(AFRELATIONSHIP_ALTERNATIVE);
	BIND_ENUM_CONSTANT(AFRELATIONSHIP_SUPPLEMENT);
	BIND_ENUM_CONSTANT(AFRELATIONSHIP_ENCRYPTEDPAYLOAD);
	BIND_ENUM_CONSTANT(AFRELATIONSHIP_FORMDATA);
	BIND_ENUM_CONSTANT(AFRELATIONSHIP_SCHEMA);
	BIND_ENUM_CONSTANT(AFRELATIONSHIP_UNSPECIFIED);
}
