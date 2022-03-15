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

#include "skeleton_editor_plugin_remover.h"

#include "editor/editor_data.h"
#include "editor/editor_node.h"
#include "editor/editor_plugin.h"
#include "editor/editor_scale.h"
#include "editor/editor_settings.h"

void SkeletonEditorPluginRemover::remove_built_in_editor_plugin() {
	EditorData &data = EditorNode::get_editor_data();

	for (int i = 0; i < data.get_editor_plugin_count(); ++i) {
		EditorPlugin *p = data.get_editor_plugin(i);

		if (p->is_class("SkeletonEditorPlugin")) {
			EditorNode::get_singleton()->remove_editor_plugin(p);

			break;
		}
	}

	Control *vp = EditorNode::get_singleton()->get_viewport();

	for (int i = 0; i < vp->get_child_count(); ++i) {
		Node *n = vp->get_child(i);

		if (n->is_class("SkeletonEditor")) {
			n->queue_delete();

			break;
		}
	}
}

SkeletonEditorPluginRemover::SkeletonEditorPluginRemover(EditorNode *p_node) {
	//note calling remove_built_int_editor_plugin here, or in code before this will cause a crash because
	//not all classes that are used in EditorNode::get_singleton()->remove_editor_plugin() initialized yet!
	call_deferred("remove_built_in_editor_plugin");
}

void SkeletonEditorPluginRemover::_bind_methods() {
	ClassDB::bind_method(D_METHOD("remove_built_in_editor_plugin"), &SkeletonEditorPluginRemover::remove_built_in_editor_plugin);
}