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

#include "skeleton_editor_module_plugin.h"

#include "scene/3d/skeleton.h"
#include "spatial_editor_gizmos.h"

SkeletonEditorModulePlugin::SkeletonEditorModulePlugin(EditorNode *p_node) {
}

void SkeletonEditorModulePlugin::_notification(int p_what) {
	if (p_what == NOTIFICATION_ENTER_TREE) {
		add_spatial_gizmo_plugin(Ref<ModuleSkeletonSpatialGizmoPlugin>(memnew(ModuleSkeletonSpatialGizmoPlugin)));
	} else if (p_what == NOTIFICATION_POST_ENTER_TREE) {
		//Removing the built in SkeletonSpatialGizmoPlugin
		//it is allocated in SpatialEditor's NOTIFICATION_ENTER_TREE, that's why we are in NOTIFICATION_POST_ENTER_TREE
		Ref<EditorSpatialGizmoPlugin> original_plugin = SpatialEditor::get_singleton()->get_gizmo_plugin("Skeleton");

		if (original_plugin.is_valid()) {
			//note this is safe to do, as this gizmo is allocated by the SpatialEditor itself.
			SpatialEditor::get_singleton()->remove_gizmo_plugin(original_plugin);
		}
	}
}
