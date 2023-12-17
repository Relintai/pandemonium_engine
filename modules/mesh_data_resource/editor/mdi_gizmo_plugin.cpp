

#include "mdi_gizmo_plugin.h"

#include "../nodes/mesh_data_instance.h"
#include "mdi_ed_plugin.h"
#include "mdi_gizmo.h"

String MDIGizmoPlugin::get_gizmo_name() const {
	return "MDIGizmo";
}
int MDIGizmoPlugin::get_priority() const {
	return 100;
}
bool MDIGizmoPlugin::is_handle_highlighted(const EditorSpatialGizmo *p_gizmo, int p_idx, bool p_secondary) const {
	return EditorSpatialGizmoPlugin::is_handle_highlighted(p_gizmo, p_idx, p_secondary);
}

Ref<EditorSpatialGizmo> MDIGizmoPlugin::create_gizmo(Spatial *p_spatial) {
	MeshDataInstance *mdi = Object::cast_to<MeshDataInstance>(p_spatial);

	if (mdi) {
		Ref<MDIGizmo> gizmo;
		gizmo.instance();

		gizmo->set_editor_plugin(plugin);
		gizmo->set_spatial_node(p_spatial);
		gizmo->setup();

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
