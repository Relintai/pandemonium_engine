#ifndef PHYSICAL_BONE_EDITOR_PLUGIN_H
#define PHYSICAL_BONE_EDITOR_PLUGIN_H

#include "core/string/ustring.h"

#include "editor/spatial_editor_gizmos.h"

class PhysicalBoneSpatialGizmoPlugin : public EditorSpatialGizmoPlugin {
	GDCLASS(PhysicalBoneSpatialGizmoPlugin, EditorSpatialGizmoPlugin);

public:
	bool has_gizmo(Spatial *p_spatial);
	String get_gizmo_name() const;
	int get_priority() const;
	void redraw(EditorSpatialGizmo *p_gizmo);

	PhysicalBoneSpatialGizmoPlugin();
};


#endif 
