#ifndef PDF_IMAGE_H
#define PDF_IMAGE_H

/*************************************************************************/
/*  pdf_image.h                                                          */
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

class PDFImage : public Reference {
	GDCLASS(PDFImage, Reference);

public:
	uint32_t get_width();
	uint32_t get_height();

#if 0
	HPDF_EXPORT(HPDF_STATUS)
	HPDF_Image_AddSMask(HPDF_Image image,
			HPDF_Image smask);

	HPDF_EXPORT(HPDF_Point)
	HPDF_Image_GetSize(HPDF_Image image);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_Image_GetSize2(HPDF_Image image, HPDF_Point *size);

	HPDF_EXPORT(HPDF_UINT)
	HPDF_Image_GetWidth(HPDF_Image image);

	HPDF_EXPORT(HPDF_UINT)
	HPDF_Image_GetHeight(HPDF_Image image);

	HPDF_EXPORT(HPDF_UINT)
	HPDF_Image_GetBitsPerComponent(HPDF_Image image);

	HPDF_EXPORT(const char *)
	HPDF_Image_GetColorSpace(HPDF_Image image);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_Image_SetColorMask(HPDF_Image image,
			HPDF_UINT rmin,
			HPDF_UINT rmax,
			HPDF_UINT gmin,
			HPDF_UINT gmax,
			HPDF_UINT bmin,
			HPDF_UINT bmax);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_Image_SetMaskImage(HPDF_Image image,
			HPDF_Image mask_image);

	// hpdf_image.h
	HPDF_BOOL
	HPDF_Image_Validate(HPDF_Image image);

	HPDF_STATUS
	HPDF_Image_SetMask(HPDF_Image image,
			HPDF_BOOL mask);

	HPDF_STATUS
	HPDF_Image_SetColorSpace(HPDF_Image image,
			HPDF_Array colorspace);

	HPDF_STATUS
	HPDF_Image_SetRenderingIntent(HPDF_Image image,
			const char *intent);
#endif

	uint32_t get_status();

	PDFImage();
	~PDFImage();

	void *_get_hpdf_image() const;
	void _set_hpdf_image(void *p_image);

protected:
	static void _bind_methods();

	void *_image;

	uint32_t _status;
};

#endif
