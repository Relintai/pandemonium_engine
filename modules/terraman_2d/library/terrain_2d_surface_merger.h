#ifndef TERRAIN_2D_SURFACE_MERGER_H
#define TERRAIN_2D_SURFACE_MERGER_H

/*************************************************************************/
/*  terrain_2d_surface_merger.h                                          */
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

#include "terrain_2d_surface.h"

#include "scene/resources/texture.h"

#include "../../texture_packer/texture_packer.h"

#include "../defines.h"

class Terrain2DSurfaceMerger : public Terrain2DSurface {
	GDCLASS(Terrain2DSurfaceMerger, Terrain2DSurface)

public:
	Ref<AtlasTexture> get_region();
	void set_region(const Ref<AtlasTexture> &texture);

	Ref<Texture> get_texture();
	void set_texture(const Ref<Texture> &texture);

	void refresh_rects();

	Terrain2DSurfaceMerger();
	~Terrain2DSurfaceMerger();

protected:
	static void _bind_methods();

private:
	Ref<AtlasTexture> _region;
	Ref<Texture> _texture;
};

#endif
