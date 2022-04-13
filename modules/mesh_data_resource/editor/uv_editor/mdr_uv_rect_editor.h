#ifndef MDR_UV_RECT_EDITOR_H
#define MDR_UV_RECT_EDITOR_H

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

#include "core/reference.h"
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
	void set_mesh_data_resource(Ref<MeshDataResource> a);
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
