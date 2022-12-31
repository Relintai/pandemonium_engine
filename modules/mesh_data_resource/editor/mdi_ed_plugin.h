#ifndef MDI_ED_PLUGIN_H
#define MDI_ED_PLUGIN_H

/*
Copyright (c) 2019-2023 Péter Magyar

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

#include "editor/editor_plugin.h"

#include "core/input/input_event.h"

class Camera;
class MDIGizmo;
class MeshDataResource;
class MDIGizmoPlugin;
class MeshDataInstance;
class MDIEd;

class MDIEdPlugin : public EditorPlugin {
	GDCLASS(MDIEdPlugin, EditorPlugin);

public:
	bool handles(Object *object) const;
	void edit(Object *object);
	void make_visible(bool visible);

	String get_name() const;

	void set_gizmo_visible(const bool visible);

	void set_translate();
	void set_scale();
	void set_rotate();

	void set_axis_x(bool on);
	void set_axis_y(bool on);
	void set_axis_z(bool on);

	void set_selection_mode_vertex();
	void set_selection_mode_edge();
	void set_selection_mode_face();

	Ref<MeshDataResource> get_mdr();

	EditorPlugin::AfterGUIInput forward_spatial_gui_input(Camera *camera, const Ref<InputEvent> &p_event);

	void add_box();
	void add_triangle();
	void add_quad();

	void add_triangle_at();
	void add_quad_at();

	void split();

	void disconnect_action();

	void create_face();
	void delete_selected();

	void generate_normals();
	void remove_doubles();
	void merge_optimize();
	void generate_tangents();

	void connect_to_first_selected();
	void connect_to_avg();
	void connect_to_last_selected();

	void mark_seam();
	void unmark_seam();
	void apply_seam();

	void uv_unwrap();

	void set_pivot_averaged();
	void set_pivot_mdi_origin();
	void set_pivot_world_origin();

	void visual_indicator_outline_set(bool on);
	void visual_indicator_seam_set(bool on);
	void visual_indicator_handle_set(bool on);

	void select_all();

	void handle_selection_type_front();
	void handle_selection_type_back();
	void handle_selection_type_all();

	void extrude();
	void clean_mesh();

	void flip_selected_faces();

	Ref<MDIGizmo> get_gizmo_from(MeshDataInstance *mdi);

	MDIEdPlugin(EditorNode *p_node);
	~MDIEdPlugin();

	Ref<MDIGizmoPlugin> gizmo_plugin;
	MDIEd *mdi_ed_gui;

	MeshDataInstance *current_mesh_data_instance;

	EditorNode *editor;

protected:
	static void _bind_methods();
};

#endif
