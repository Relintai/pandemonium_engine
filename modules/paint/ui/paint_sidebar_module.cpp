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

#include "paint_sidebar_module.h"

#include "../nodes/paint_node.h"

void PaintSidebarModule::on_paint_node_selected(PaintNode *paint_node) {
	call("_on_paint_node_selected", paint_node);
}
void PaintSidebarModule::on_paint_node_selected_bind(Node *paint_node) {
	call("_on_paint_node_selected", paint_node);
}
void PaintSidebarModule::_on_paint_node_selected(Node *paint_node) {
}

PaintSidebarModule::PaintSidebarModule() {
}

PaintSidebarModule::~PaintSidebarModule() {
}

void PaintSidebarModule::_bind_methods() {
	BIND_VMETHOD(MethodInfo("_on_paint_node_selected", PropertyInfo(Variant::OBJECT, "paint_node", PROPERTY_HINT_RESOURCE_TYPE, "PaintNode")));

	ClassDB::bind_method(D_METHOD("on_paint_node_selected", "paint_node"), &PaintSidebarModule::on_paint_node_selected_bind);
	ClassDB::bind_method(D_METHOD("_on_paint_node_selected", "paint_node"), &PaintSidebarModule::_on_paint_node_selected);
}
