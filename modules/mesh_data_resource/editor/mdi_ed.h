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

class EditorPlugin;
class MeshDataResource;
class MeshDataInstance;
class MDIEdPlugin;
class MDREdUVPreviewer;
class MDRUVRectEditorPopup;

class MDIEd : public PanelContainer {
	GDCLASS(MDIEd, PanelContainer);

public:
	void _enter_tree();
	void set_plugin(EditorPlugin *plugin);
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

	void _on_Extrude_pressed();
	void _on_AddBox_pressed();
	void _on_UnwrapButton_pressed();
	void _on_add_triangle_pressed();
	void _on_add_quad_pressed();
	void _on_split_pressed();
	void _on_connect_to_first_selected_pressed();
	void _on_connect_to_avg_pressed();
	void _on_connect_to_last_selected_pressed();
	void _on_disconnect_pressed();
	void _on_add_triangle_at_pressed();
	void _on_add_auad_at_pressed();
	void _oncreate_face_pressed();
	void _on_delete_pressed();
	void _on_GenNormals_pressed();
	void _on_RemDoubles_pressed();
	void _on_MergeOptimize_pressed();
	void _on_GenTangents_pressed();
	void _on_mark_seam_pressed();
	void _on_unmark_seam_pressed();
	void _on_apply_seams_pressed();
	void _on_uv_edit_pressed();

	void on_pivot_average_toggled(bool on);
	void on_pivot_mdi_origin_toggled(bool on);
	void on_pivot_world_origin_toggled(bool on);
	void on_visual_indicator_outline_toggled(bool on);
	void on_visual_indicator_seam_toggled(bool on);
	void on_visual_indicator_handle_toggled(bool on);
	void _on_select_all_pressed();

	void onhandle_selection_type_front_toggled(bool on);
	void onhandle_selection_type_back_toggled(bool on);
	void onhandle_selection_type_all_toggled(bool on);

	void _on_clean_mesh_pressed();
	void _on_flip_face_pressed();

	MDIEd();
	~MDIEd();

	MDIEdPlugin *_plugin;

	MDREdUVPreviewer *uv_preview;
	MDRUVRectEditorPopup *uv_editor;

protected:
	static void _bind_methods();
};

#endif
