#ifndef TEXT_EDITOR_EDITOR_PLUGIN_H
#define TEXT_EDITOR_EDITOR_PLUGIN_H

/*
Copyright (c) 2019-2023 Péter Magyar

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

#include "core/io/resource_importer.h"
#include "core/object/reference.h"
#include "editor/editor_plugin.h"

class TextFileEditor;
class Texture;

class TextEditorEditorPlugin : public EditorPlugin {
	GDCLASS(TextEditorEditorPlugin, EditorPlugin);

public:
	void make_visible(const bool visible);
	const Ref<Texture> get_icon() const;
	bool has_main_screen() const;
	String get_name() const;
	void edit(Object *p_object);
	bool handles(Object *p_object) const;

	TextEditorEditorPlugin(EditorNode *p_node);
	~TextEditorEditorPlugin();

	EditorNode *editor;

	TextFileEditor *window;

protected:
	void _on_filesystem_dock_entry_pressed(int id);

	static void _bind_methods();

	Ref<Texture> _icon;
	int _filesystem_dock_entry_id;
};

#endif
