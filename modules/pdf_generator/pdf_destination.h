#ifndef PDF_DESTINATION_H
#define PDF_DESTINATION_H

/*************************************************************************/
/*  pdf_destination.h                                                    */
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

class PDFDestination : public Reference {
	GDCLASS(PDFDestination, Reference);

public:
#if 0
	/*----- destination --------------------------------------------------------*/

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_Destination_SetXYZ(HPDF_Destination dst,
			HPDF_REAL left,
			HPDF_REAL top,
			HPDF_REAL zoom);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_Destination_SetFit(HPDF_Destination dst);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_Destination_SetFitH(HPDF_Destination dst,
			HPDF_REAL top);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_Destination_SetFitV(HPDF_Destination dst,
			HPDF_REAL left);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_Destination_SetFitR(HPDF_Destination dst,
			HPDF_REAL left,
			HPDF_REAL bottom,
			HPDF_REAL right,
			HPDF_REAL top);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_Destination_SetFitB(HPDF_Destination dst);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_Destination_SetFitBH(HPDF_Destination dst,
			HPDF_REAL top);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_Destination_SetFitBV(HPDF_Destination dst,
			HPDF_REAL left);

#endif

	uint32_t get_status();

	PDFDestination();
	~PDFDestination();

	void *_get_hpdf_destination() const;
	void _set_hpdf_destination(void *p_destination);

protected:
	static void _bind_methods();

	void *_destination;

	uint32_t _status;
};

#endif
