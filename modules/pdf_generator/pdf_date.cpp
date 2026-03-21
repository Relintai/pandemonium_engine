/*************************************************************************/
/*  pdf_date.cpp                                                         */
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

#include "pdf_date.h"

#include "hpdf.h"

int PDFDate::get_year() const {
	return _year;
}
void PDFDate::set_year(const int p_value) {
	_year = p_value;
}

int PDFDate::get_month() const {
	return _month;
}
void PDFDate::set_month(const int p_value) {
	_month = p_value;
}

int PDFDate::get_day() const {
	return _day;
}
void PDFDate::set_day(const int p_value) {
	_day = p_value;
}

int PDFDate::get_hour() const {
	return _hour;
}
void PDFDate::set_hour(const int p_value) {
	_hour = p_value;
}

int PDFDate::get_minutes() const {
	return _minutes;
}
void PDFDate::set_minutes(const int p_value) {
	_minutes = p_value;
}

int PDFDate::get_seconds() const {
	return _seconds;
}
void PDFDate::set_seconds(const int p_value) {
	_seconds = p_value;
}

uint8_t PDFDate::get_ind() const {
	return _ind;
}
void PDFDate::set_ind(const uint8_t p_value) {
	_ind = p_value;
}

int PDFDate::get_off_hour() const {
	return _off_hour;
}
void PDFDate::set_off_hour(const int p_value) {
	_off_hour = p_value;
}

int PDFDate::get_off_minutes() const {
	return _off_minutes;
}
void PDFDate::set_off_minutes(const int p_value) {
	_off_minutes = p_value;
}

PDFDate::PDFDate() {
	_year = 0;
	_month = 0;
	_day = 0;
	_hour = 0;
	_minutes = 0;
	_seconds = 0;
	_ind = '\0';
	_off_hour = 0;
	_off_minutes = 0;
}

PDFDate::~PDFDate() {
}

void PDFDate::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_year"), &PDFDate::get_year);
	ClassDB::bind_method(D_METHOD("set_year", "val"), &PDFDate::set_year);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "year"), "set_year", "get_year");

	ClassDB::bind_method(D_METHOD("get_month"), &PDFDate::get_month);
	ClassDB::bind_method(D_METHOD("set_month", "val"), &PDFDate::set_month);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "month"), "set_month", "get_month");

	ClassDB::bind_method(D_METHOD("get_day"), &PDFDate::get_day);
	ClassDB::bind_method(D_METHOD("set_day", "val"), &PDFDate::set_day);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "day"), "set_day", "get_day");

	ClassDB::bind_method(D_METHOD("get_hour"), &PDFDate::get_hour);
	ClassDB::bind_method(D_METHOD("set_hour", "val"), &PDFDate::set_hour);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "hour"), "set_hour", "get_hour");

	ClassDB::bind_method(D_METHOD("get_minutes"), &PDFDate::get_minutes);
	ClassDB::bind_method(D_METHOD("set_minutes", "val"), &PDFDate::set_minutes);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "minutes"), "set_minutes", "get_minutes");

	ClassDB::bind_method(D_METHOD("get_seconds"), &PDFDate::get_seconds);
	ClassDB::bind_method(D_METHOD("set_seconds", "val"), &PDFDate::set_seconds);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "seconds"), "set_seconds", "get_seconds");

	ClassDB::bind_method(D_METHOD("get_ind"), &PDFDate::get_ind);
	ClassDB::bind_method(D_METHOD("set_ind", "val"), &PDFDate::set_ind);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "ind"), "set_ind", "get_ind");

	ClassDB::bind_method(D_METHOD("get_off_hour"), &PDFDate::get_off_hour);
	ClassDB::bind_method(D_METHOD("set_off_hour", "val"), &PDFDate::set_off_hour);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "off_hour"), "set_off_hour", "get_off_hour");

	ClassDB::bind_method(D_METHOD("get_off_minutes"), &PDFDate::get_off_minutes);
	ClassDB::bind_method(D_METHOD("set_off_minutes", "val"), &PDFDate::set_off_minutes);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "off_minutes"), "set_off_minutes", "get_off_minutes");
}
