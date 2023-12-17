

#include "mdr_ed_uv_previewer.h"

#include "../../mesh_data_resource.h"
#include "../../nodes/mesh_data_instance.h"
#include "scene/resources/texture.h"

void MDREdUVPreviewer::set_mesh_data_resource(const Ref<MeshDataResource> &a) {
	if (mesh_data_resource.is_valid()) {
		mesh_data_resource->disconnect("changed", this, "on_mdr_changed");
	}

	mesh_data_resource = a;

	if (mesh_data_resource.is_valid()) {
		mesh_data_resource->connect("changed", this, "on_mdr_changed");
	}

	update();
}

void MDREdUVPreviewer::set_mesh_data_instance(MeshDataInstance *a) {
	if (!a) {
		background_texture.unref();
		return;
	}

	background_texture = a->get_texture();
}

void MDREdUVPreviewer::on_mdr_changed() {
	update();
}

void MDREdUVPreviewer::_draw() {
	if (background_texture.is_valid()) {
		draw_texture_rect_region(background_texture, Rect2(Vector2(), get_size()), Rect2(Vector2(), background_texture->get_size()));
	}

	if (!mesh_data_resource.is_valid()) {
		return;
	}

	Array arr = mesh_data_resource->get_array();

	if (arr.size() != ArrayMesh::ARRAY_MAX) {
		return;
	}

	PoolVector2Array uvs = arr[ArrayMesh::ARRAY_TEX_UV];
	PoolIntArray indices = arr[ArrayMesh::ARRAY_INDEX];

	if (indices.size() % 3 == 0) {
		for (int i = 0; i < indices.size(); i += 3) {
			Color c = Color(1, 1, 1, 1);

			if (uvs[indices[i]].is_equal_approx(Vector2()) || uvs[indices[i + 1]].is_equal_approx(Vector2())) {
				c = Color(1, 0, 0, 1);
			} else {
				c = Color(1, 1, 1, 1);
			}

			draw_line(uvs[indices[i]] * get_size(), uvs[indices[i + 1]] * get_size(), c, 1, false);

			if (uvs[indices[i + 1]].is_equal_approx(Vector2()) || uvs[indices[i + 2]].is_equal_approx(Vector2())) {
				c = Color(1, 0, 0, 1);
			} else {
				c = Color(1, 1, 1, 1);
			}

			draw_line(uvs[indices[i + 1]] * get_size(), uvs[indices[i + 2]] * get_size(), c, 1, false);

			if (uvs[indices[i + 2]].is_equal_approx(Vector2()) || uvs[indices[i]].is_equal_approx(Vector2())) {
				c = Color(1, 0, 0, 1);
			} else {
				c = Color(1, 1, 1, 1);
			}

			draw_line(uvs[indices[i + 2]] * get_size(), uvs[indices[i]] * get_size(), c, 1, false);
		}
	}
}

void MDREdUVPreviewer::_notification(int p_what) {
	if (p_what == NOTIFICATION_DRAW) {
		_draw();
	}
}

MDREdUVPreviewer::MDREdUVPreviewer() {
}

MDREdUVPreviewer::~MDREdUVPreviewer() {
}

void MDREdUVPreviewer::_bind_methods() {
	ClassDB::bind_method(D_METHOD("on_mdr_changed"), &MDREdUVPreviewer::on_mdr_changed);
}
