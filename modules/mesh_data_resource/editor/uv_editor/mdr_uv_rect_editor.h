#ifndef MDR_UV_RECT_EDITOR_H
#define MDR_UV_RECT_EDITOR_H



#include "core/object/reference.h"
#include "scene/gui/panel_container.h"

class EditorPlugin;
class MeshDataResource;
class MeshDataInstance;
class MDIEdPlugin;
class MDRUVRectView;
class Button;

class MDRUVRectEditor : public PanelContainer {
	GDCLASS(MDRUVRectEditor, PanelContainer);

public:
	void set_plugin(MDIEdPlugin *plugin);
	void set_mesh_data_resource(const Ref<MeshDataResource> &a);
	void set_mesh_data_instance(MeshDataInstance *a);
	void ok_pressed();
	void cancel_pressed();

	MDRUVRectEditor();
	~MDRUVRectEditor();

	MDRUVRectView *uv_rect_view;

	Button *horizontal_mirror_button;
	Button *vertical_mirror_button;
	Button *rotate_left_button;
	Button *rotate_right_button;

protected:
	void _notification(int p_what);
	static void _bind_methods();
};

#endif
