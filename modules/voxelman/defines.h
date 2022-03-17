
#ifndef VOXEL_DEFINES_H
#define VOXEL_DEFINES_H

#define core_input_h "core/os/input.h"
#define spatial_editor_plugin_h "editor/plugins/spatial_editor_plugin.h"
#define camera_h "scene/3d/camera.h"
#define spatial_h "scene/3d/spatial.h"
#define navigation_h "scene/3d/navigation.h"
#define light_h "scene/3d/light.h"
#define visual_server_h "servers/visual_server.h"
#define mesh_instance_h "scene/3d/mesh_instance.h"
#define pool_vector_h "core/pool_vector.h"
#define physics_server_h "servers/physics_server.h"
#define immediate_geometry_h "scene/3d/immediate_geometry.h"
#define include_pool_vector

#define VARIANT_ARRAY_GET(arr)             \
	Vector<Variant> r;                     \
	for (int i = 0; i < arr.size(); i++) { \
		r.push_back(arr[i].get_ref_ptr()); \
	}                                      \
	return r;

#define VARIANT_ARRAY_SET(arr, arr_into, type) \
	arr_into.clear();                          \
	for (int i = 0; i < arr.size(); i++) {     \
		Ref<type> e = Ref<type>(arr[i]);       \
		arr_into.push_back(e);                 \
	}

#define INSTANCE_VALIDATE(var) ObjectDB::instance_validate(var)
#define CONNECT(sig, obj, target_method_class, method) connect(sig, obj, #method)
#define DISCONNECT(sig, obj, target_method_class, method) disconnect(sig, obj, #method)
#define GET_WORLD get_world
#define INSTANCE instance
#define VREMOVE remove

#endif
