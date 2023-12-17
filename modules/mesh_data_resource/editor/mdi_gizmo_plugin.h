#ifndef MDI_GIZMO_PLUGIN_H
#define MDI_GIZMO_PLUGIN_H



#include "editor/spatial_editor_gizmos.h"

class MDIEdPlugin;

class MDIGizmoPlugin : public EditorSpatialGizmoPlugin {
	GDCLASS(MDIGizmoPlugin, EditorSpatialGizmoPlugin);

public:
	void _init();
	String get_gizmo_name() const;
	int get_priority() const;
	bool is_handle_highlighted(const EditorSpatialGizmo *p_gizmo, int p_idx, bool p_secondary) const;

	MDIGizmoPlugin();
	~MDIGizmoPlugin();

	MDIEdPlugin *plugin;

protected:
	Ref<EditorSpatialGizmo> create_gizmo(Spatial *p_spatial);
	static void _bind_methods();
};

#endif
