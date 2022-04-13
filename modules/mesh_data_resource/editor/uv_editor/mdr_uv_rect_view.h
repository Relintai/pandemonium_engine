#ifndef MDR_UV_RECT_VIEW_H
#define MDR_UV_RECT_VIEW_H

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

#include "core/math/rect2.h"
#include "core/pool_vector.h"
#include "core/reference.h"
#include "scene/gui/control.h"

class MeshDataResource;
class MeshDataInstance;
class Texture;
class EditorPlugin;
class UndoRedo;
class Button;
class EditorZoomWidget;
class SpinBox;
class MDRUVRectViewNode;

class MDRUVRectView : public Control {
	GDCLASS(MDRUVRectView, Control);

public:
	void set_plugin(EditorPlugin *plugin);
	void set_mesh_data_resource(Ref<MeshDataResource> a);
	void set_mesh_data_instance(MeshDataInstance *a);
	void set_selected(MDRUVRectViewNode *node);

	void store_uvs();
	PoolVector2Array get_uvs(Ref<MeshDataResource> mdr);
	void apply_uvs(Ref<MeshDataResource> mdr, PoolVector2Array stored_uvs);

	void refresh();
	void clear();
	void refresh_rects();
	void clear_rects();

	void _enter_tree();
	void _draw();

	void on_mirror_horizontal_button_pressed();
	void on_mirror_vertical_button_pressed();
	void on_rotate_left_button_button_pressed();

	void on_rotate_amount_spinbox_changed(float val);
	void on_rotate_right_button_button_pressed();
	void on_edited_resource_changed();

	void on_zoom_changed(float zoom);

	void on_visibility_changed();

	void ok_pressed();
	void cancel_pressed();

	void apply_zoom();

	MDRUVRectView();
	~MDRUVRectView();

protected:
	void _notification(int p_what);
	static void _bind_methods();

	EditorZoomWidget *zoom_widget;

	Button *mirror_horizontal_button;
	Button *mirror_vertical_button;

	Button *rotate_left_button;
	SpinBox *rotate_amount_spinbox;
	Button *rotate_right_button;

public:
	float _rect_scale;

	Ref<MeshDataResource> _mdr;
	Ref<Texture> _background_texture;

	Rect2 base_rect;
	Vector2 edited_resource_current_size;

	PoolVector2Array _stored_uvs;

	EditorPlugin *_plugin;
	UndoRedo *_undo_redo;

	MDRUVRectViewNode *selected_rect;

	float rotation_amount;
};

#endif
