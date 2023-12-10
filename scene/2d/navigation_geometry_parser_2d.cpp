/**************************************************************************/
/*  navigation_geometry_parser_2d.cpp                                     */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#include "navigation_geometry_parser_2d.h"

#include "scene/main/node_2d.h"

#include "scene/resources/navigation_2d/navigation_mesh_source_geometry_data_2d.h"
#include "scene/resources/navigation_2d/navigation_polygon.h"

bool NavigationGeometryParser2D::parses_node(Node *p_node) {
	if (Object::cast_to<Node2D>(p_node)) {
		return false;
	}

	if (has_method("_parses_node")) {
		return call("_parses_node", p_node);
	}

	return false;
}

void NavigationGeometryParser2D::parse_node_geometry(Ref<NavigationPolygon> p_navigation_polygon, Node *p_node, Ref<NavigationMeshSourceGeometryData2D> p_source_geometry) {
	parse_geometry(p_node, p_navigation_polygon, p_source_geometry);
}

void NavigationGeometryParser2D::parse_geometry(Node *p_node, Ref<NavigationPolygon> p_navigation_polygon, Ref<NavigationMeshSourceGeometryData2D> p_source_geometry) {
	if (has_method("_parse_geometry")) {
		call("_parse_geometry", p_node, p_navigation_polygon, p_source_geometry);
	}
}

void NavigationGeometryParser2D::_bind_methods() {
	BIND_VMETHOD(MethodInfo(Variant::BOOL, "_parses_node", PropertyInfo(Variant::OBJECT, "node", PROPERTY_HINT_RESOURCE_TYPE, "Node")));
	BIND_VMETHOD(MethodInfo("_parse_geometry",
			PropertyInfo(Variant::OBJECT, "node", PROPERTY_HINT_RESOURCE_TYPE, "Node"),
			PropertyInfo(Variant::OBJECT, "navigation_polygon", PROPERTY_HINT_RESOURCE_TYPE, "NavigationPolygon"),
			PropertyInfo(Variant::OBJECT, "source_geometry", PROPERTY_HINT_RESOURCE_TYPE, "NavigationMeshSourceGeometryData2D")));
}
