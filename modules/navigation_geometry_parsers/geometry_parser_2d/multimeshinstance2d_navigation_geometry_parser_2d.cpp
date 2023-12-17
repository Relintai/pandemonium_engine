/*************************************************************************/
/*  multimeshinstance2d_navigation_geometry_parser_2d.cpp                */
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

#include "multimeshinstance2d_navigation_geometry_parser_2d.h"

#include "core/math/geometry.h"
#include "scene/2d/multimesh_instance_2d.h"
#include "scene/resources/mesh/multimesh.h"

#include "scene/resources/navigation_2d/navigation_mesh_source_geometry_data_2d.h"
#include "scene/resources/navigation_2d/navigation_polygon.h"

bool MultiMeshInstance2DNavigationGeometryParser2D::parses_node(Node *p_node) {
	return (Object::cast_to<MultiMeshInstance2D>(p_node) != nullptr);
}

void MultiMeshInstance2DNavigationGeometryParser2D::parse_geometry(Node *p_node, Ref<NavigationPolygon> p_navigation_polygon, Ref<NavigationMeshSourceGeometryData2D> p_source_geometry) {
	NavigationPolygon::ParsedGeometryType parsed_geometry_type = p_navigation_polygon->get_parsed_geometry_type();

	if (Object::cast_to<MultiMeshInstance2D>(p_node) && parsed_geometry_type != NavigationPolygon::PARSED_GEOMETRY_STATIC_COLLIDERS) {
		MultiMeshInstance2D *multimesh_instance = Object::cast_to<MultiMeshInstance2D>(p_node);
		Ref<MultiMesh> multimesh = multimesh_instance->get_multimesh();
		if (multimesh.is_valid() && multimesh->get_transform_format() == MultiMesh::TRANSFORM_2D) {
			Ref<Mesh> mesh = multimesh->get_mesh();
			if (mesh.is_valid()) {
				Vector<Vector<Point2>> mesh_subject_paths, dummy_clip_paths;

				for (int i = 0; i < mesh->get_surface_count(); i++) {
					if (mesh->surface_get_primitive_type(i) != Mesh::PRIMITIVE_TRIANGLES) {
						continue;
					}

					if (!(mesh->surface_get_format(i) & Mesh::ARRAY_FLAG_USE_2D_VERTICES)) {
						continue;
					}

					Vector<Point2> subject_path;

					int index_count = 0;
					if (mesh->surface_get_format(i) & Mesh::ARRAY_FORMAT_INDEX) {
						index_count = mesh->surface_get_array_index_len(i);
					} else {
						index_count = mesh->surface_get_array_len(i);
					}

					ERR_CONTINUE((index_count == 0 || (index_count % 3) != 0));

					Array a = mesh->surface_get_arrays(i);

					Vector<Vector2> mesh_vertices = a[Mesh::ARRAY_VERTEX];

					if (mesh->surface_get_format(i) & Mesh::ARRAY_FORMAT_INDEX) {
						Vector<int> mesh_indices = a[Mesh::ARRAY_INDEX];

						for (int j = 0; j < mesh_indices.size(); ++j) {
							int vertex_index = mesh_indices[j];
							const Vector2 &vertex = mesh_vertices[vertex_index];
							subject_path.push_back(vertex);
						}
					} else {
						for (int j = 0; j < mesh_vertices.size(); ++j) {
							const Vector2 &vertex = mesh_vertices[j];
							subject_path.push_back(vertex);
						}
					}
					mesh_subject_paths.push_back(subject_path);
				}

				Vector<Vector<Point2>> mesh_path_solution = Geometry::merge_all2_polygons_2d(mesh_subject_paths, dummy_clip_paths, Geometry::POLYGON_FILL_TYPE_NON_ZERO);

				int multimesh_instance_count = multimesh->get_visible_instance_count();
				if (multimesh_instance_count == -1) {
					multimesh_instance_count = multimesh->get_instance_count();
				}

				for (int i = 0; i < multimesh_instance_count; i++) {
					const Transform2D multimesh_instance_transform = multimesh_instance->get_transform() * multimesh->get_instance_transform_2d(i);

					for (int k = 0; k < mesh_path_solution.size(); ++k) {
						const Vector<Point2> &mesh_path = mesh_path_solution[k];

						PoolVector<Vector2> shape_outline;

						for (int j = 0; j < mesh_path.size(); j++) {
							const Vector2 &mesh_path_point = mesh_path[j];
							shape_outline.push_back(multimesh_instance_transform.xform(mesh_path_point));
						}

						p_source_geometry->add_obstruction_outline(shape_outline);
					}
				}
			}
		}
	}
}
