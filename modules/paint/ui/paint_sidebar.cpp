/*
Copyright (c) 2022 Péter Magyar

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

#include "paint_sidebar.h"

#include "../nodes/paint_node.h"
#include "paint_sidebar_module.h"
#include "scene/gui/box_container.h"

void PaintSidebar::add_module(PaintSidebarModule *module) {
	ERR_FAIL_COND(!module);

	_main_container->add_child(module);
}
void PaintSidebar::remove_module(PaintSidebarModule *module) {
	ERR_FAIL_COND(!module);

	_main_container->remove_child(module);
}

void PaintSidebar::add_module_bind(Node *module) {
	PaintSidebarModule *m = Object::cast_to<PaintSidebarModule>(module);
	add_module(m);
}
void PaintSidebar::remove_module_bind(Node *module) {
	PaintSidebarModule *m = Object::cast_to<PaintSidebarModule>(module);
	remove_module(m);
}

PaintSidebar::PaintSidebar() {
	_main_container = memnew(VBoxContainer);
	add_child(_main_container);
}

void PaintSidebar::on_paint_node_selected(PaintNode *paint_node) {
	for (int i = 0; i < _main_container->get_child_count(); ++i) {
		PaintSidebarModule *module = Object::cast_to<PaintSidebarModule>(_main_container->get_child(i));

		if (module) {
			module->on_paint_node_selected(paint_node);
		}
	}
}
void PaintSidebar::on_paint_node_selected_bind(Node *paint_node) {
	PaintNode *m = Object::cast_to<PaintNode>(paint_node);
	on_paint_node_selected(m);
}

PaintSidebar::~PaintSidebar() {
}

void PaintSidebar::_bind_methods() {
	ClassDB::bind_method(D_METHOD("add_module", "module"), &PaintSidebar::add_module_bind);
	ClassDB::bind_method(D_METHOD("remove_module", "module"), &PaintSidebar::remove_module_bind);
	ClassDB::bind_method(D_METHOD("on_paint_node_selected", "paint_node"), &PaintSidebar::on_paint_node_selected_bind);
}
