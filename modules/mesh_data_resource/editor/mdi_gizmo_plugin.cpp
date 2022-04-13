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

#include "mdi_gizmo_plugin.h"

#include "../nodes/mesh_data_instance.h"
#include "mdi_ed_plugin.h"
#include "mdi_gizmo.h"

String MDIGizmoPlugin::get_name() const {
	return "MDIGizmo";
}
int MDIGizmoPlugin::get_priority() const {
	return 100;
}
bool MDIGizmoPlugin::is_handle_highlighted(const EditorSpatialGizmo *p_gizmo, int p_idx) const {
	return EditorSpatialGizmoPlugin::is_handle_highlighted(p_gizmo, p_idx);
}

Ref<EditorSpatialGizmo> MDIGizmoPlugin::create_gizmo(Spatial *p_spatial) {
	MeshDataInstance *mdi = Object::cast_to<MeshDataInstance>(p_spatial);

	if (mdi) {
		Ref<MDIGizmo> gizmo;
		gizmo.instance();

		gizmo->set_editor_plugin(plugin);
		gizmo->set_spatial_node(p_spatial);
		gizmo->setup();
		plugin->register_gizmo(gizmo);

		return gizmo;
	} else {
		return Ref<EditorSpatialGizmo>();
	}
}

MDIGizmoPlugin::MDIGizmoPlugin() {
	plugin = nullptr;

	create_material("main", Color(0.7, 0.7, 0.7));
	create_material("seam", Color(1, 0, 0), false, true);
	create_handle_material("handles");
}

MDIGizmoPlugin::~MDIGizmoPlugin() {
}

void MDIGizmoPlugin::_bind_methods() {
}
