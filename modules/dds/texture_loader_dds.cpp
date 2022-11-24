/*************************************************************************/
/*  texture_loader_dds.cpp                                               */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
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

#include "texture_loader_dds.h"

#include "core/os/file_access.h"

#define PF_FOURCC(s) ((uint32_t)(((s)[3] << 24U) | ((s)[2] << 16U) | ((s)[1] << 8U) | ((s)[0])))

// Reference: https://docs.microsoft.com/en-us/windows/win32/direct3ddds/dds-header

enum {
	DDS_MAGIC = 0x20534444,
	DDSD_PITCH = 0x00000008,
	DDSD_LINEARSIZE = 0x00080000,
	DDSD_MIPMAPCOUNT = 0x00020000,
	DDPF_FOURCC = 0x00000004,
	DDPF_ALPHAPIXELS = 0x00000001,
	DDPF_INDEXED = 0x00000020,
	DDPF_RGB = 0x00000040,
};

enum DDSFormat {
	DDS_DXT1,
	DDS_DXT3,
	DDS_DXT5,
	DDS_ATI1,
	DDS_ATI2,
	DDS_A2XY,
	DDS_BGRA8,
	DDS_BGR8,
	DDS_RGBA8, //flipped in dds
	DDS_RGB8, //flipped in dds
	DDS_BGR5A1,
	DDS_BGR565,
	DDS_BGR10A2,
	DDS_INDEXED,
	DDS_LUMINANCE,
	DDS_LUMINANCE_ALPHA,
	DDS_MAX
};

struct DDSFormatInfo {
	const char *name;
	bool compressed;
	bool palette;
	uint32_t divisor;
	uint32_t block_size;
	Image::Format format;
};

static const DDSFormatInfo dds_format_info[DDS_MAX] = {
	{ "DXT1/BC1", true, false, 4, 8, Image::FORMAT_DXT1 },
	{ "DXT3/BC2", true, false, 4, 16, Image::FORMAT_DXT3 },
	{ "DXT5/BC3", true, false, 4, 16, Image::FORMAT_DXT5 },
	{ "ATI1/BC4", true, false, 4, 8, Image::FORMAT_RGTC_R },
	{ "ATI2/3DC/BC5", true, false, 4, 16, Image::FORMAT_RGTC_RG },
	{ "A2XY/DXN/BC5", true, false, 4, 16, Image::FORMAT_RGTC_RG },
	{ "BGRA8", false, false, 1, 4, Image::FORMAT_RGBA8 },
	{ "BGR8", false, false, 1, 3, Image::FORMAT_RGB8 },
	{ "RGBA8", false, false, 1, 4, Image::FORMAT_RGBA8 },
	{ "RGB8", false, false, 1, 3, Image::FORMAT_RGB8 },
	{ "BGR5A1", false, false, 1, 2, Image::FORMAT_RGBA8 },
	{ "BGR565", false, false, 1, 2, Image::FORMAT_RGB8 },
	{ "BGR10A2", false, false, 1, 4, Image::FORMAT_RGBA8 },
	{ "GRAYSCALE", false, false, 1, 1, Image::FORMAT_L8 },
	{ "GRAYSCALE_ALPHA", false, false, 1, 2, Image::FORMAT_LA8 }
};

RES ResourceFormatDDS::load(const String &p_path, const String &p_original_path, Error *r_error) {
	if (r_error) {
		*r_error = ERR_CANT_OPEN;
	}

	Error err;
	FileAccess *f = FileAccess::open(p_path, FileAccess::READ, &err);
	if (!f) {
		return RES();
	}

	FileAccessRef fref(f);
	if (r_error) {
		*r_error = ERR_FILE_CORRUPT;
	}

	ERR_FAIL_COND_V_MSG(err != OK, RES(), "Unable to open DDS texture file '" + p_path + "'.");

	Ref<Image> img = memnew(Image);
	Error i_error = img->load(p_path);
	if (r_error) {
		*r_error = i_error;
	}

	Ref<ImageTexture> texture = memnew(ImageTexture);
	texture->create_from_image(img);

	if (r_error) {
		*r_error = OK;
	}

	return texture;
}

void ResourceFormatDDS::get_recognized_extensions(List<String> *p_extensions) const {
	p_extensions->push_back("dds");
}

bool ResourceFormatDDS::handles_type(const String &p_type) const {
	return ClassDB::is_parent_class(p_type, "Texture");
}

String ResourceFormatDDS::get_resource_type(const String &p_path) const {
	if (p_path.get_extension().to_lower() == "dds") {
		return "ImageTexture";
	}
	return "";
}
