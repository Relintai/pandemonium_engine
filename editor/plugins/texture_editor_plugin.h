#ifndef TEXTURE_EDITOR_PLUGIN_H
#define TEXTURE_EDITOR_PLUGIN_H

/*************************************************************************/
/*  texture_editor_plugin.h                                              */
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

#include "editor/editor_inspector.h"
#include "editor/editor_plugin.h"
#include "scene/gui/margin_container.h"

#include "core/object/object.h"
#include "core/object/reference.h"
#include "core/string/ustring.h"
#include "scene/resources/texture.h"

class EditorNode;
class Label;
class Texture;
class TextureRect;

class TexturePreview : public MarginContainer {
	GDCLASS(TexturePreview, MarginContainer);

private:
	TextureRect *texture_display = nullptr;

	TextureRect *checkerboard = nullptr;
	Label *metadata_label = nullptr;

	void _update_metadata_label_text();

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	TextureRect *get_texture_display();
	TexturePreview(Ref<Texture> p_texture, bool p_show_metadata);
};

class EditorInspectorPluginTexture : public EditorInspectorPlugin {
	GDCLASS(EditorInspectorPluginTexture, EditorInspectorPlugin);

public:
	virtual bool can_handle(Object *p_object);
	virtual void parse_begin(Object *p_object);
};

class TextureEditorPlugin : public EditorPlugin {
	GDCLASS(TextureEditorPlugin, EditorPlugin);

public:
	virtual String get_name() const { return "Texture"; }

	TextureEditorPlugin(EditorNode *p_node);
};

#endif // TEXTURE_EDITOR_PLUGIN_H
