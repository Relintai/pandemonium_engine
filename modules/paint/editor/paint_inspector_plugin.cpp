/*************************************************************************/
/*  paint_inspector_plugin.cpp                                           */
/*************************************************************************/
/*                         This file is part of:                         */
/*                          PANDEMONIUM ENGINE                           */
/*             https://github.com/Relintai/pandemonium_engine            */
/*************************************************************************/
/* Copyright (c) 2022-present Péter Magyar.                              */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "paint_inspector_plugin.h"

#include "../ui/property_inspectors/paint_project_property_inspector.h"
#include "../ui/property_inspectors/paint_project_tools_property_inspector.h"
#include "../ui/property_inspectors/paint_tools_property_inspector.h"

#include "../nodes/paint_node.h"

bool PaintInspectorPlugin::can_handle(Object *p_object) {
	return p_object->is_class("PaintNode");
}
void PaintInspectorPlugin::parse_begin(Object *p_object) {
	PaintNode *paint_node = Object::cast_to<PaintNode>(p_object);

	PaintProjectPropertyInspector *pc = memnew(PaintProjectPropertyInspector);
	pc->set_paint_node(paint_node);
	add_custom_control(pc);

	if (p_object->is_class("PaintCanvas")) {
		PaintToolsPropertyInspector *ptool = memnew(PaintToolsPropertyInspector);
		ptool->set_paint_node(paint_node);
		add_custom_control(ptool);
	}

	if (p_object->is_class("PaintProject")) {
		PaintProjectToolsPropertyInspector *pct = memnew(PaintProjectToolsPropertyInspector);
		pct->set_paint_node(paint_node);
		add_custom_control(pct);
	}
}

PaintInspectorPlugin::PaintInspectorPlugin() {
}

PaintInspectorPlugin::~PaintInspectorPlugin() {
}

void PaintInspectorPlugin::_bind_methods() {
}
