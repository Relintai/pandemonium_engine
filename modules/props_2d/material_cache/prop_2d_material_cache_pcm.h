#ifndef PROP_2D_MATERIAL_CACHE_PCM_H
#define PROP_2D_MATERIAL_CACHE_PCM_H

/*************************************************************************/
/*  prop_2d_material_cache_pcm.h                                         */
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

#include "prop_2d_material_cache.h"

#include "core/containers/vector.h"
#include "core/math/color.h"
#include "core/object/resource.h"

#include "core/math/rect2.h"
#include "scene/resources/material/material.h"

class TexturePacker;
class Prop2DData;

class Prop2DMaterialCachePCM : public Prop2DMaterialCache {
	GDCLASS(Prop2DMaterialCachePCM, Prop2DMaterialCache);

public:
	int get_texture_flags() const;
	void set_texture_flags(const int flags);

	int get_max_atlas_size() const;
	void set_max_atlas_size(const int size);

	bool get_keep_original_atlases() const;
	void set_keep_original_atlases(const bool value);

	Color get_background_color() const;
	void set_background_color(const Color &color);

	int get_margin() const;
	void set_margin(const int margin);

	Ref<AtlasTexture> texture_get_atlas_tex(const Ref<Texture> &texture);
	Rect2 texture_get_uv_rect(const Ref<Texture> &texture);

	Ref<Texture> texture_get_merged();

	void refresh_rects();

	void initial_setup_default();

	void _setup_material_albedo(Ref<Texture> texture);

	Prop2DMaterialCachePCM();
	~Prop2DMaterialCachePCM();

protected:
	static void _bind_methods();

	Ref<TexturePacker> _packer;
	Ref<Texture> _merged_texture;
};

#endif
