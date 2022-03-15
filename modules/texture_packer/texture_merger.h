/*
Copyright (c) 2019-2022 Péter Magyar

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef TEXTURE_MERGER_H
#define TEXTURE_MERGER_H

#include "core/version.h"

#if VERSION_MAJOR > 3
#include "core/templates/vector.h"
#include "core/config/engine.h"
#else
#include "core/vector.h"
#include "core/engine.h"
#endif

#include "scene/main/node.h"

#include "texture_packer.h"

class TextureMerger : public Node {
	GDCLASS(TextureMerger, Node);

public:
	bool get_dirty() const;
	void set_dirty(const bool value);

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

	bool get_automatic_merge() const;
	void set_automatic_merge(const bool value);

	Ref<TexturePacker> get_packer() const;
	void set_packer(const Ref<TexturePacker> &packer);

	Vector<Variant> get_textures();
	void set_textures(const Vector<Variant> &textures);

	Ref<AtlasTexture> add_texture(const Ref<Texture> &texture);
	Ref<AtlasTexture> get_texture(const Ref<Texture> &texture);
	Ref<AtlasTexture> get_texture_index(const int index);
	Ref<Texture> get_original_texture(const int index);
	bool contains_texture(const Ref<Texture> &texture);

	bool unref_texture_index(const int index);
	bool unref_texture(const Ref<Texture> &texture);
	void remove_texture_index(const int index);
	void remove_texture(const Ref<Texture> &texture);

	int get_texture_count();
	void clear();

	Ref<ImageTexture> get_generated_texture(const int index);
	int get_generated_texture_count();

	void merge();

#if VERSION_MAJOR >= 4
	GDVIRTUAL0(_texture_merged);
	GDVIRTUAL1(_texture_added, Ref<AtlasTexture>);
	GDVIRTUAL0(_texture_removed);
#endif

	TextureMerger();
	~TextureMerger();

protected:
	static void _bind_methods();
	void _notification(int p_what);

private:
	bool _automatic_merge;
	bool _dirty;

	Ref<TexturePacker> _packer;
	Vector<Ref<Texture> > _textures;
};

#endif
