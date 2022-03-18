#ifndef SKELETON_EDITOR_MODULE_PLUGIN_H
#define SKELETON_EDITOR_MODULE_PLUGIN_H
/*
Copyright (c) 2020 Péter Magyar

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

#include "editor/editor_node.h"
#include "editor/editor_plugin.h"

class SkeletonEditorModulePlugin : public EditorPlugin {
	GDCLASS(SkeletonEditorModulePlugin, EditorPlugin);

public:
	bool has_main_screen() const { return false; }

	virtual String get_name() const { return "SkeletonEditorModulePlugin"; }

	SkeletonEditorModulePlugin(EditorNode *p_node);

protected:
	void _notification(int p_what);
};

#endif
