#ifndef MDI_ED_H
#define MDI_ED_H

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

#include "scene/gui/panel_container.h"

class MDIEdPlugin;
class MeshDataResource;
class MeshDataInstance;
class MDIEdPlugin;
class MDREdUVPreviewer;
class MDRUVRectEditorPopup;
class VBoxContainer;
class Button;

class MDIEd : public PanelContainer {
	GDCLASS(MDIEd, PanelContainer);

public:
	void set_plugin(MDIEdPlugin *plugin);
	void set_mesh_data_resource(Ref<MeshDataResource> a);
	void set_mesh_data_instance(MeshDataInstance *a);
	void _unhandled_key_input(Ref<InputEventKey> event);

	//Edit modes
	void set_edit_mode_translate();
	void set_edit_mode_rotate();
	void set_edit_mode_scale();

	void on_edit_mode_translate_toggled(bool on);
	void on_edit_mode_rotate_toggled(bool on);
	void on_edit_mode_scale_toggled(bool on);

	//axis locks
	bool get_axis_x();
	bool get_axis_y();
	bool get_axis_z();
	void set_axis_x(bool on);
	void set_axis_y(bool on);
	void set_axis_z(bool on);

	void on_axis_x_toggled(bool on);
	void on_axis_y_toggled(bool on);
	void on_axis_z_toggled(bool on);

	//selection modes
	void on_selection_mode_vertex_toggled(bool on);
	void on_selection_mode_edge_toggled(bool on);
	void on_selection_mode_face_toggled(bool on);

	void set_selection_mode_vertex();
	void set_selection_mode_edge();
	void set_selection_mode_face();

	void on_handle_selection_type_front_toggled(bool on);
	void on_handle_selection_type_back_toggled(bool on);
	void on_handle_selection_type_all_toggled(bool on);

	void on_select_all_pressed();

	//Vertex ops
	void on_create_face_pressed();
	void on_split_pressed();
	void on_connect_to_first_selected_pressed();
	void on_connect_to_avg_pressed();
	void on_connect_to_last_selected_pressed();

	//Edge ops
	void on_extrude_pressed();
	void on_add_triangle_at_pressed();
	void on_add_quad_at_pressed();
	void on_mark_seam_pressed();
	void on_unmark_seam_pressed();

	//Face ops
	void on_delete_pressed();
	void on_flip_face_pressed();

	void on_pivot_average_toggled(bool on);
	void on_pivot_mdi_origin_toggled(bool on);
	void on_pivot_world_origin_toggled(bool on);

	void on_visual_indicator_outline_toggled(bool on);
	void on_visual_indicator_seam_toggled(bool on);
	void on_visual_indicator_handle_toggled(bool on);

	//Ops
	void on_generate_normals_pressed();
	void on_generate_tangents_pressed();

	void on_remove_doubles_pressed();
	void on_merge_optimize_pressed();
	void on_clean_mesh_pressed();

	void on_uv_edit_pressed();
	void on_apply_seams_pressed();
	void on_unwrap_button_pressed();

	void on_add_box_pressed();
	void on_add_triangle_pressed();
	void on_add_quad_pressed();

	void _on_disconnect_pressed();

	MDIEd();
	~MDIEd();

	MDIEdPlugin *_plugin;

	MDREdUVPreviewer *uv_preview;
	MDRUVRectEditorPopup *uv_editor;

	VBoxContainer *vertex_operations_container;
	VBoxContainer *edge_operations_container;
	VBoxContainer *face_operations_container;

	Button *_edit_mode_translate_button;
	Button *_edit_mode_rotate_button;
	Button *_edit_mode_scale_button;

	Button *_axis_lock_x_button;
	Button *_axis_lock_y_button;
	Button *_axis_lock_z_button;

	Button *_selection_mode_vertex_button;
	Button *_selection_mode_edge_button;
	Button *_selection_mode_face_button;

protected:
	static void _bind_methods();
};

#endif
