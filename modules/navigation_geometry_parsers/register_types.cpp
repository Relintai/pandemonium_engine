/*************************************************************************/
/*  register_types.cpp                                                   */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
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

#include "register_types.h"

#include "core/config/engine.h"

#include "servers/navigation/navigation_mesh_generator.h"

#include "geometry_parser_2d/meshinstance2d_navigation_geometry_parser_2d.h"
#include "geometry_parser_2d/multimeshinstance2d_navigation_geometry_parser_2d.h"
#include "geometry_parser_2d/polygon2d_navigation_geometry_parser_2d.h"
#include "geometry_parser_2d/staticbody2d_navigation_geometry_parser_2d.h"
#include "geometry_parser_2d/tilemap_navigation_geometry_parser_2d.h"
#ifndef _3D_DISABLED
#include "geometry_parser_3d/meshinstance3d_navigation_geometry_parser_3d.h"
#include "geometry_parser_3d/multimeshinstance3d_navigation_geometry_parser_3d.h"
#include "geometry_parser_3d/staticbody3d_navigation_geometry_parser_3d.h"
#endif // _3D_DISABLED

void register_navigation_geometry_parsers_types(ModuleRegistrationLevel p_level) {
	if (p_level == MODULE_REGISTRATION_LEVEL_SCENE) {
		NavigationMeshGenerator::get_singleton()->register_geometry_parser_2d(memnew(MeshInstance2DNavigationGeometryParser2D));
		NavigationMeshGenerator::get_singleton()->register_geometry_parser_2d(memnew(MultiMeshInstance2DNavigationGeometryParser2D));
		NavigationMeshGenerator::get_singleton()->register_geometry_parser_2d(memnew(Polygon2DNavigationGeometryParser2D));
		NavigationMeshGenerator::get_singleton()->register_geometry_parser_2d(memnew(StaticBody2DNavigationGeometryParser2D));
		NavigationMeshGenerator::get_singleton()->register_geometry_parser_2d(memnew(TileMap2DNavigationGeometryParser2D));
#ifndef _3D_DISABLED
		NavigationMeshGenerator::get_singleton()->register_geometry_parser_3d(memnew(MeshInstance3DNavigationGeometryParser3D));
		NavigationMeshGenerator::get_singleton()->register_geometry_parser_3d(memnew(MultiMeshInstance3DNavigationGeometryParser3D));
		NavigationMeshGenerator::get_singleton()->register_geometry_parser_3d(memnew(StaticBody3DNavigationGeometryParser3D));
#endif // _3D_DISABLED
	}
}

void unregister_navigation_geometry_parsers_types(ModuleRegistrationLevel p_level) {
}
