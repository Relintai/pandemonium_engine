#ifndef PDF_DATE_H
#define PDF_DATE_H

/*************************************************************************/
/*  pdf_date.h                                                           */
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

class PDFDate : public Reference {
	GDCLASS(PDFDate, Reference);

public:
	int get_year() const;
	void set_year(const int p_value);

	int get_month() const;
	void set_month(const int p_value);

	int get_day() const;
	void set_day(const int p_value);

	int get_hour() const;
	void set_hour(const int p_value);

	int get_minutes() const;
	void set_minutes(const int p_value);

	int get_seconds() const;
	void set_seconds(const int p_value);

	uint8_t get_ind() const;
	void set_ind(const uint8_t p_value);

	int get_off_hour() const;
	void set_off_hour(const int p_value);

	int get_off_minutes() const;
	void set_off_minutes(const int p_value);

	PDFDate();
	~PDFDate();

protected:
	static void _bind_methods();

	int _year;
	int _month;
	int _day;
	int _hour;
	int _minutes;
	int _seconds;
	uint8_t _ind;
	int _off_hour;
	int _off_minutes;
};

#endif
