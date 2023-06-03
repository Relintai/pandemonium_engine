
#include "navigation_mesh_generator_dummy.h"

#include "core/config/project_settings.h"

#include "scene/2d/navigation_geometry_parser_2d.h"
#ifndef _3D_DISABLED
#include "scene/3d/navigation_geometry_parser_3d.h"
#endif // _3D_DISABLED
#include "scene/main/node.h"
#include "scene/resources/navigation_mesh.h"
#include "scene/resources/navigation_mesh_source_geometry_data_2d.h"
#ifndef _3D_DISABLED
#include "scene/resources/navigation_mesh_source_geometry_data_3d.h"
#endif // _3D_DISABLED

void NavigationMeshGeneratorDummy::cleanup() {}
void NavigationMeshGeneratorDummy::process() {}

// 2D //////////////////////////////
void NavigationMeshGeneratorDummy::register_geometry_parser_2d(Ref<NavigationGeometryParser2D> p_geometry_parser) {}
void NavigationMeshGeneratorDummy::unregister_geometry_parser_2d(Ref<NavigationGeometryParser2D> p_geometry_parser) {}

Ref<NavigationMeshSourceGeometryData2D> NavigationMeshGeneratorDummy::parse_2d_source_geometry_data(Ref<NavigationPolygon> p_navigation_polygon, Node *p_root_node, Ref<FuncRef> p_callback) {
	return Ref<NavigationMeshSourceGeometryData2D>();
}
void NavigationMeshGeneratorDummy::bake_2d_from_source_geometry_data(Ref<NavigationPolygon> p_navigation_polygon, Ref<NavigationMeshSourceGeometryData2D> p_source_geometry_data, Ref<FuncRef> p_callback) {}

void NavigationMeshGeneratorDummy::parse_and_bake_2d(Ref<NavigationPolygon> p_navigation_polygon, Node *p_root_node, Ref<FuncRef> p_callback) {}

bool NavigationMeshGeneratorDummy::is_navigation_polygon_baking(Ref<NavigationPolygon> p_navigation_polygon) const {
	return false;
}

// 3D //////////////////////////////
#ifndef _3D_DISABLED
void NavigationMeshGeneratorDummy::register_geometry_parser_3d(Ref<NavigationGeometryParser3D> p_geometry_parser) {}
void NavigationMeshGeneratorDummy::unregister_geometry_parser_3d(Ref<NavigationGeometryParser3D> p_geometry_parser) {}

Ref<NavigationMeshSourceGeometryData3D> NavigationMeshGeneratorDummy::parse_3d_source_geometry_data(Ref<NavigationMesh> p_navigation_mesh, Node *p_root_node, Ref<FuncRef> p_callback) {
	return Ref<NavigationMeshSourceGeometryData3D>();
}
void NavigationMeshGeneratorDummy::bake_3d_from_source_geometry_data(Ref<NavigationMesh> p_navigation_mesh, Ref<NavigationMeshSourceGeometryData3D> p_source_geometry_data, Ref<FuncRef> p_callback) {}

void NavigationMeshGeneratorDummy::parse_and_bake_3d(Ref<NavigationMesh> p_navigation_mesh, Node *p_root_node, Ref<FuncRef> p_callback) {}

bool NavigationMeshGeneratorDummy::is_navigation_mesh_baking(Ref<NavigationMesh> p_navigation_mesh) const {
	return false;
}
#endif // _3D_DISABLED
