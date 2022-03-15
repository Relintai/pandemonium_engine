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

#include "texture_merger.h"

bool TextureMerger::get_dirty() const {
	return _dirty;
}
void TextureMerger::set_dirty(const bool value) {
	_dirty = value;

	if (!_automatic_merge || Engine::get_singleton()->is_editor_hint())
		return;

	if (_dirty)
		set_process(true);
}

int TextureMerger::get_texture_flags() const {
	return _packer->get_texture_flags();
}
void TextureMerger::set_texture_flags(const int flags) {
	_packer->set_texture_flags(flags);
}

int TextureMerger::get_max_atlas_size() const {
	return _packer->get_max_atlas_size();
}
void TextureMerger::set_max_atlas_size(const int size) {
	_packer->set_max_atlas_size(size);
}

bool TextureMerger::get_keep_original_atlases() const {
	return _packer->get_keep_original_atlases();
}
void TextureMerger::set_keep_original_atlases(const bool value) {
	_packer->set_keep_original_atlases(value);
}

Color TextureMerger::get_background_color() const {
	return _packer->get_background_color();
}
void TextureMerger::set_background_color(const Color &color) {
	_packer->set_background_color(color);
}

int TextureMerger::get_margin() const {
	return _packer->get_margin();
}
void TextureMerger::set_margin(const int margin) {
	_packer->set_margin(margin);
}

bool TextureMerger::get_automatic_merge() const {
	return _automatic_merge;
}
void TextureMerger::set_automatic_merge(const bool value) {
	_automatic_merge = value;
}

Ref<TexturePacker> TextureMerger::get_packer() const {
	return _packer;
}
void TextureMerger::set_packer(const Ref<TexturePacker> &packer) {
	_packer = packer;
}

Vector<Variant> TextureMerger::get_textures() {
	Vector<Variant> r;
	for (int i = 0; i < _textures.size(); i++) {
#if VERSION_MAJOR < 4
		r.push_back(_textures[i].get_ref_ptr());
#else
		r.push_back(_textures[i]);
#endif
	}
	return r;
}
void TextureMerger::set_textures(const Vector<Variant> &textures) {
	bool texture_removed = false;
	for (int i = 0; i < _textures.size(); ++i) {
		if (_packer->unref_texture(_textures.get(i))) {
			texture_removed = true;
		}
	}

	_textures.clear();
	_textures.resize(textures.size());

	for (int i = 0; i < textures.size(); i++) {
		Ref<Texture> texture = Ref<Texture>(textures.get(i));

		_textures.set(i, texture);
	}

	if (Engine::get_singleton()->is_editor_hint())
		return;

	bool texture_added = false;
	for (int i = 0; i < _textures.size(); ++i) {
		Ref<Texture> texture = _textures.get(i);

		if (texture.is_valid() && !_packer->contains_texture(texture)) {
			Ref<AtlasTexture> tex = _packer->add_texture(texture);

			texture_added = true;
		}
	}

	if ((texture_added || texture_removed) && _automatic_merge)
		set_dirty(true);
}

Ref<AtlasTexture> TextureMerger::add_texture(const Ref<Texture> &texture) {
	ERR_FAIL_COND_V(!texture.is_valid(), Ref<AtlasTexture>());

	_textures.push_back(texture);

	bool contains = _packer->contains_texture(texture);

	Ref<AtlasTexture> tex = _packer->add_texture(texture);

	if (!contains) {
#if VERSION_MAJOR < 4
		if (has_method("_texture_added")) {
			call("_texture_added", tex);
		}
#else
		GDVIRTUAL_CALL(_texture_added, tex);
#endif

		emit_signal("texture_added", tex);

		set_dirty(true);
	}

	return tex;
}

Ref<Texture> TextureMerger::get_original_texture(const int index) {
	return _packer->get_original_texture(index);
}

bool TextureMerger::contains_texture(const Ref<Texture> &texture) {
	return _packer->contains_texture(texture);
}

Ref<AtlasTexture> TextureMerger::get_texture(const Ref<Texture> &texture) {
	return _packer->get_texture(texture);
}

Ref<AtlasTexture> TextureMerger::get_texture_index(const int index) {
	return _packer->get_texture_index(index);
}

bool TextureMerger::unref_texture_index(const int index) {
	if (_packer->unref_texture_index(index)) {
#if VERSION_MAJOR < 4
		if (has_method("_texture_removed")) {
			call("_texture_removed");
		}
#else
		GDVIRTUAL_CALL(_texture_removed);
#endif

		emit_signal("texture_removed");

		set_dirty(true);

		return true;
	}

	return false;
}

bool TextureMerger::unref_texture(const Ref<Texture> &texture) {
	if (_packer->unref_texture(texture)) {
#if VERSION_MAJOR < 4
		if (has_method("_texture_removed")) {
			call("_texture_removed");
		}
#else
		GDVIRTUAL_CALL(_texture_removed);
#endif

		emit_signal("texture_removed");

		set_dirty(true);

		return true;
	}

	return false;
}

void TextureMerger::remove_texture_index(const int index) {
	_packer->remove_texture_index(index);

#if VERSION_MAJOR < 4
	if (has_method("_texture_removed")) {
		call("_texture_removed");
	}
#else
	GDVIRTUAL_CALL(_texture_removed);
#endif

	emit_signal("texture_removed");

	set_dirty(true);
}

void TextureMerger::remove_texture(const Ref<Texture> &texture) {
	_packer->remove_texture(texture);

#if VERSION_MAJOR < 4
	if (has_method("_texture_removed")) {
		call("_texture_removed");
	}
#else
	GDVIRTUAL_CALL(_texture_removed);
#endif

	emit_signal("texture_removed");

	set_dirty(true);
}

int TextureMerger::get_texture_count() {
	return _packer->get_texture_count();
}

void TextureMerger::clear() {
	_packer->clear();
}

Ref<ImageTexture> TextureMerger::get_generated_texture(const int index) {
	return _packer->get_generated_texture(index);
}
int TextureMerger::get_generated_texture_count() {
	return _packer->get_generated_texture_count();
}

void TextureMerger::merge() {
	_packer->merge();

#if VERSION_MAJOR < 4
	if (has_method("_texture_merged")) {
		call("_texture_merged");
	}
#else
	GDVIRTUAL_CALL(_texture_merged);
#endif

	emit_signal("texture_merged");
}

TextureMerger::TextureMerger() {
	_automatic_merge = false;

#if VERSION_MAJOR < 4
	_packer.instance();
#else
	_packer.instantiate();
#endif

	_packer->set_keep_original_atlases(false);
}

TextureMerger::~TextureMerger() {
	_packer.unref();
}

void TextureMerger::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_PROCESS: {
			if (!_automatic_merge || Engine::get_singleton()->is_editor_hint())
				return;

			set_process(false);

			if (!_dirty)
				return;

			_dirty = false;

			merge();

		} break;
	}
}

void TextureMerger::_bind_methods() {
	ADD_SIGNAL(MethodInfo("texture_merged"));
	ADD_SIGNAL(MethodInfo("texture_added", PropertyInfo(Variant::OBJECT, "texture", PROPERTY_HINT_RESOURCE_TYPE, "AtlasTexture")));
	ADD_SIGNAL(MethodInfo("texture_removed"));

#if VERSION_MAJOR < 4
	BIND_VMETHOD(MethodInfo("_texture_merged"));
	BIND_VMETHOD(MethodInfo("_texture_added", PropertyInfo(Variant::OBJECT, "texture", PROPERTY_HINT_RESOURCE_TYPE, "AtlasTexture")));
	BIND_VMETHOD(MethodInfo("_texture_removed"));
#else
	GDVIRTUAL_BIND(_texture_merged);
	GDVIRTUAL_BIND(_texture_added, "texture");
	GDVIRTUAL_BIND(_texture_removed);
#endif

	ClassDB::bind_method(D_METHOD("get_dirty"), &TextureMerger::get_dirty);
	ClassDB::bind_method(D_METHOD("set_dirty", "value"), &TextureMerger::set_dirty);

	ClassDB::bind_method(D_METHOD("get_texture_flags"), &TextureMerger::get_texture_flags);
	ClassDB::bind_method(D_METHOD("set_texture_flags", "flags"), &TextureMerger::set_texture_flags);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "texture_flags", PROPERTY_HINT_FLAGS, "Mipmaps,Repeat,Filter,Anisotropic Linear,Convert to Linear,Mirrored Repeat,Video Surface"), "set_texture_flags", "get_texture_flags");

	ClassDB::bind_method(D_METHOD("get_max_atlas_size"), &TextureMerger::get_max_atlas_size);
	ClassDB::bind_method(D_METHOD("set_max_atlas_size", "size"), &TextureMerger::set_max_atlas_size);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "max_atlas_size"), "set_max_atlas_size", "get_max_atlas_size");

	ClassDB::bind_method(D_METHOD("get_keep_original_atlases"), &TextureMerger::get_keep_original_atlases);
	ClassDB::bind_method(D_METHOD("set_keep_original_atlases", "value"), &TextureMerger::set_keep_original_atlases);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "keep_original_atlases"), "set_keep_original_atlases", "get_keep_original_atlases");

	ClassDB::bind_method(D_METHOD("get_background_color"), &TextureMerger::get_background_color);
	ClassDB::bind_method(D_METHOD("set_background_color", "color"), &TextureMerger::set_background_color);
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "background_color"), "set_background_color", "get_background_color");

	ClassDB::bind_method(D_METHOD("get_margin"), &TextureMerger::get_margin);
	ClassDB::bind_method(D_METHOD("set_margin", "size"), &TextureMerger::set_margin);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "margin"), "set_margin", "get_margin");

	ClassDB::bind_method(D_METHOD("get_automatic_merge"), &TextureMerger::get_automatic_merge);
	ClassDB::bind_method(D_METHOD("set_automatic_merge", "value"), &TextureMerger::set_automatic_merge);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "automatic_merge"), "set_automatic_merge", "get_automatic_merge");

	ClassDB::bind_method(D_METHOD("get_textures"), &TextureMerger::get_textures);
	ClassDB::bind_method(D_METHOD("set_textures", "textures"), &TextureMerger::set_textures);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "textures", PROPERTY_HINT_NONE, "17/17:Texture", PROPERTY_USAGE_DEFAULT, "Texture"), "set_textures", "get_textures");

	ClassDB::bind_method(D_METHOD("get_packer"), &TextureMerger::get_packer);
	ClassDB::bind_method(D_METHOD("set_packer", "packer"), &TextureMerger::set_packer);

	ClassDB::bind_method(D_METHOD("add_texture", "texture"), &TextureMerger::add_texture);
	ClassDB::bind_method(D_METHOD("get_texture", "texture"), &TextureMerger::get_texture);
	ClassDB::bind_method(D_METHOD("get_texture_index", "index"), &TextureMerger::get_texture_index);
	ClassDB::bind_method(D_METHOD("get_original_texture", "index"), &TextureMerger::get_original_texture);

	ClassDB::bind_method(D_METHOD("unref_texture_index", "index"), &TextureMerger::unref_texture_index);
	ClassDB::bind_method(D_METHOD("unref_texture", "texture"), &TextureMerger::unref_texture);
	ClassDB::bind_method(D_METHOD("remove_texture_index", "index"), &TextureMerger::remove_texture_index);
	ClassDB::bind_method(D_METHOD("remove_texture", "texture"), &TextureMerger::remove_texture);

	ClassDB::bind_method(D_METHOD("get_texture_count"), &TextureMerger::get_texture_count);
	ClassDB::bind_method(D_METHOD("clear"), &TextureMerger::clear);

	ClassDB::bind_method(D_METHOD("get_generated_texture", "index"), &TextureMerger::get_generated_texture);
	ClassDB::bind_method(D_METHOD("get_generated_texture_count"), &TextureMerger::get_generated_texture_count);

	ClassDB::bind_method(D_METHOD("merge"), &TextureMerger::merge);
}
