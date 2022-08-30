#include "world.h"

#include "core/config/engine.h"
#include "core/core_string_names.h"
#include "scene/3d/camera.h"
#include "scene/3d/spatial.h"
#include "scene/3d/world_environment_3d.h"
#include "scene/resources/world_2d.h"
#include "scene/resources/world_3d.h"
#include "viewport.h"

Camera *World::get_camera() const {
	return camera;
}

Ref<World2D> World::get_world_2d() const {
	return world_2d;
}

void World::set_world_2d(const Ref<World2D> &p_world_2d) {
	if (world_2d == p_world_2d) {
		return;
	}

	if (_parent_world && _parent_world->find_world_2d() == p_world_2d) {
		WARN_PRINT("Unable to use parent world as world_2d");
		return;
	}

	Ref<World2D> old_world;

	if (is_inside_tree()) {
		old_world = find_world_2d();
	}

	if (p_world_2d.is_valid()) {
		world_2d = p_world_2d;
	} else {
		WARN_PRINT("Invalid world");
		world_2d = Ref<World2D>(memnew(World2D));
	}

	_on_set_world_2d(old_world);
}

Ref<World3D> World::get_world_3d() const {
	return world_3d;
}

void World::set_world_3d(const Ref<World3D> &p_world_3d) {
	if (world_3d == p_world_3d) {
		return;
	}

	if (is_inside_tree()) {
		_propagate_exit_world(this);
	}

	Ref<World3D> old_world = world_3d;

	if (own_world_3d.is_valid() && world_3d.is_valid()) {
		world_3d->disconnect(CoreStringNames::get_singleton()->changed, this, "_own_world_3d_changed");
	}

	world_3d = p_world_3d;

	if (own_world_3d.is_valid()) {
		if (world_3d.is_valid()) {
			own_world_3d = world_3d->duplicate();
			world_3d->connect(CoreStringNames::get_singleton()->changed, this, "_own_world_3d_changed");
		} else {
			own_world_3d = Ref<World3D>(memnew(World3D));
		}
	}

	if (is_inside_tree()) {
		_propagate_enter_world(this);
	}

	_on_set_world_3d(old_world);
}

bool World::is_using_own_world_3d() const {
	return own_world_3d.is_valid();
}

void World::set_use_own_world_3d(bool p_use_own_world_3d) {
	if (p_use_own_world_3d == own_world_3d.is_valid()) {
		return;
	}

	if (is_inside_tree()) {
		_propagate_exit_world(this);
	}

	if (p_use_own_world_3d) {
		if (world_3d.is_valid()) {
			own_world_3d = world_3d->duplicate();
			world_3d->connect(CoreStringNames::get_singleton()->changed, this, "_own_world_3d_changed");
		} else {
			own_world_3d = Ref<World3D>(memnew(World3D));
		}
	} else {
		own_world_3d = Ref<World3D>();
		if (world_3d.is_valid()) {
			world_3d->disconnect(CoreStringNames::get_singleton()->changed, this, "_own_world_3d_changed");
		}
	}

	if (is_inside_tree()) {
		_propagate_enter_world(this);
	}

	_on_set_use_own_world_3d(p_use_own_world_3d);
}

bool World::get_override_in_parent_viewport() {
	return _override_in_parent_viewport;
}
void World::set_override_in_parent_viewport(const bool value) {
	if (_override_in_parent_viewport == value) {
		return;
	}

	if (!Engine::get_singleton()->is_editor_hint() && is_inside_tree()) {
		World *w = get_viewport();

		if (w) {
			if (_override_in_parent_viewport) {
				if (w->get_override_world() == this) {
					w->set_override_world(NULL);
				}
			} else {
				w->set_override_world(this);
			}
		}
	}

	_override_in_parent_viewport = value;
}

Ref<World2D> World::find_world_2d() const {
	if (_override_world) {
		return _override_world->find_world_2d();
	}

	if (world_2d.is_valid()) {
		return world_2d;
	} else if (_parent_world) {
		return _parent_world->find_world_2d();
	} else {
		return Ref<World2D>();
	}
}

Ref<World3D> World::find_world_3d() const {
	if (_override_world) {
		return _override_world->find_world_3d();
	}

	if (own_world_3d.is_valid()) {
		return own_world_3d;
	} else if (world_3d.is_valid()) {
		return world_3d;
	} else if (_parent_world) {
		return _parent_world->find_world_3d();
	} else {
		return Ref<World3D>();
	}
}

Ref<World2D> World::find_world_2d_no_override() const {
	if (world_2d.is_valid()) {
		return world_2d;
	} else if (_parent_world) {
		return _parent_world->find_world_2d();
	} else {
		return Ref<World2D>();
	}
}

Ref<World3D> World::find_world_3d_no_override() const {
	if (own_world_3d.is_valid()) {
		return own_world_3d;
	} else if (world_3d.is_valid()) {
		return world_3d;
	} else if (_parent_world) {
		return _parent_world->find_world_3d();
	} else {
		return Ref<World3D>();
	}
}

World *World::get_override_world() {
	return _override_world;
}
World *World::get_override_world_or_this() {
	if (!_override_world) {
		return this;
	}

	return _override_world;
}
void World::set_override_world(World *p_world) {
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}

	if (p_world == _override_world || p_world == this) {
		return;
	}

	World *old_world = get_override_world();
	Ref<World3D> old_world_3d = find_world_3d();

	_on_before_world_override_changed();

	if (old_world) {
		old_world->_remove_overridden_world(this);
	}

	_clear_override_cameras();

	_override_world = p_world;

	if (_override_world) {
		_override_world->_add_overridden_world(this);
	}

	if (old_world_3d.is_valid()) {
		Ref<World3D> new_world_3d = find_world_3d();

		if (old_world_3d != new_world_3d) {
			if (new_world_3d.is_valid()) {
				old_world_3d->move_cameras_into(new_world_3d);

			} else {
				ERR_PRINT("!new_world_3d.is_valid()");
			}
		}
	} else {
		ERR_PRINT("!old_world_3d.is_valid()");
	}

	if (_override_world) {
		_add_override_cameras_into(_override_world);
	}

	_on_after_world_override_changed();
}
void World::set_override_world_bind(Node *p_world) {
	World *w = Object::cast_to<World>(p_world);

	ERR_FAIL_COND(p_world && !w);

	set_override_world(w);
}

void World::gui_reset_canvas_sort_index() {
	if (_override_world) {
		_override_world->gui_reset_canvas_sort_index();
	}
}
int World::gui_get_canvas_sort_index() {
	if (_override_world) {
		return _override_world->gui_get_canvas_sort_index();
	}

	return 0;
}
void World::enable_canvas_transform_override(bool p_enable) {
	if (_override_world) {
		_override_world->enable_canvas_transform_override(p_enable);
	}
}
bool World::is_canvas_transform_override_enbled() const {
	if (_override_world) {
		return _override_world->is_canvas_transform_override_enbled();
	}

	return false;
}
void World::set_canvas_transform_override(const Transform2D &p_transform) {
	if (_override_world) {
		_override_world->set_canvas_transform_override(p_transform);
	}
}
Transform2D World::get_canvas_transform_override() const {
	if (_override_world) {
		return _override_world->get_canvas_transform_override();
	}

	return Transform2D();
}

void World::set_canvas_transform(const Transform2D &p_transform) {
	if (_override_world) {
		_override_world->set_canvas_transform(p_transform);
	}
}

Transform2D World::get_canvas_transform() const {
	if (_override_world) {
		return _override_world->get_canvas_transform();
	}

	return Transform2D();
}

void World::set_global_canvas_transform(const Transform2D &p_transform) {
	if (_override_world) {
		_override_world->set_global_canvas_transform(p_transform);
	}
}

Transform2D World::get_global_canvas_transform() const {
	if (_override_world) {
		return _override_world->get_global_canvas_transform();
	}

	return Transform2D();
}

Transform2D World::get_final_transform() const {
	if (_override_world) {
		return _override_world->get_final_transform();
	}

	return Transform2D();
}

Rect2 World::get_visible_rect() const {
	if (_override_world) {
		return _override_world->get_visible_rect();
	}

	return Rect2();
}

RID World::get_viewport_rid() const {
	if (_override_world) {
		return _override_world->get_viewport_rid();
	}

	return RID();
}

Vector2 World::get_camera_coords(const Vector2 &p_viewport_coords) const {
	if (_override_world) {
		return _override_world->get_camera_coords(p_viewport_coords);
	}

	Transform2D xf = get_final_transform();
	return xf.xform(p_viewport_coords);
}

Vector2 World::get_camera_rect_size() const {
	if (_override_world) {
		return _override_world->get_camera_rect_size();
	}

	return size;
}

void World::update_worlds() {
	if (!is_inside_tree()) {
		return;
	}

	Rect2 abstracted_rect = Rect2(Vector2(), get_visible_rect().size);
	Rect2 xformed_rect = (get_global_canvas_transform() * get_canvas_transform()).affine_inverse().xform(abstracted_rect);
	find_world_2d()->_update_world(this, xformed_rect);
	find_world_2d()->_update();

	find_world_3d()->_update(get_tree()->get_frame());
}

void World::_world_3d_register_camera(Camera *p_camera) {
	if (_override_world) {
		_override_world->_world_3d_register_camera_as_override(p_camera);
	}

	_world_3d_register_camera_no_override(p_camera);
}

void World::_world_3d_remove_camera(Camera *p_camera) {
	if (_override_world) {
		_override_world->_world_3d_remove_camera_as_override(p_camera);
	}

	_world_3d_remove_camera_no_override(p_camera);
}

void World::_world_3d_register_camera_no_override(Camera *p_camera) {
	find_world_3d_no_override()->_register_camera(p_camera);
}

void World::_world_3d_remove_camera_no_override(Camera *p_camera) {
	find_world_3d_no_override()->_remove_camera(p_camera);
}

void World::_world_3d_register_camera_as_override(Camera *p_camera) {
	find_world_3d_no_override()->_register_camera(p_camera);

	_override_cameras.push_back(p_camera);

	_camera_add(p_camera);
}

void World::_world_3d_remove_camera_as_override(Camera *p_camera) {
	find_world_3d_no_override()->_remove_camera(p_camera);

	_override_cameras.erase(p_camera);

	_camera_remove(p_camera);
}

void World::_clear_override_cameras() {
	while (_override_cameras.size() > 0) {
		_world_3d_remove_camera_as_override(_override_cameras[0]);
	}

	_camera_set(_own_active_camera);
	_own_active_camera = NULL;
}

void World::_add_override_cameras_into(World *p_from) {
	_own_active_camera = camera;

	Ref<World3D> w3d = find_world_3d_no_override();

	ERR_FAIL_COND(!w3d.is_valid());

	List<Camera *> cameras;
	w3d->get_camera_list(&cameras);

	for (List<Camera *>::Element *E = cameras.front(); E; E = E->next()) {
		Camera *cam = E->get();

		_world_3d_register_camera_as_override(cam);
	}

	if (p_from->get_camera()) {
		_camera_set(p_from->get_camera());
	}
}

World::World() {
	world_2d = Ref<World2D>(memnew(World2D));
	_override_world = NULL;
	_override_in_parent_viewport = false;
	camera = NULL;
	_own_active_camera = NULL;
}
World::~World() {
}

void World::_camera_transform_changed_notify() {
#ifndef _3D_DISABLED
// If there is an active listener in the scene, it takes priority over the camera
//	if (camera && !listener)
//		SpatialSoundServer::get_singleton()->listener_set_transform(internal_listener, camera->get_camera_transform());
#endif
}

void World::_camera_set(Camera *p_camera) {
#ifndef _3D_DISABLED

	if (camera == p_camera) {
		return;
	}

	if (camera) {
		camera->notification(Camera::NOTIFICATION_LOST_CURRENT);
	}
	camera = p_camera;

	/*
	if (!camera_override) {
		if (camera) {
			RenderingServer::get_singleton()->viewport_attach_camera(viewport, camera->get_camera());
		} else {
			RenderingServer::get_singleton()->viewport_attach_camera(viewport, RID());
		}
	}
	*/

	if (camera) {
		camera->notification(Camera::NOTIFICATION_BECAME_CURRENT);
	}

	_update_listener();
	_camera_transform_changed_notify();
#endif
}

bool World::_camera_add(Camera *p_camera) {
	cameras.insert(p_camera);
	return cameras.size() == 1;
}

void World::_camera_remove(Camera *p_camera) {
	cameras.erase(p_camera);
	if (camera == p_camera) {
		camera->notification(Camera::NOTIFICATION_LOST_CURRENT);
		camera = nullptr;
	}
}

#ifndef _3D_DISABLED
void World::_camera_make_next_current(Camera *p_exclude) {
	for (Set<Camera *>::Element *E = cameras.front(); E; E = E->next()) {
		if (p_exclude == E->get()) {
			continue;
		}
		if (!E->get()->is_inside_tree()) {
			continue;
		}
		if (camera != nullptr) {
			return;
		}

		E->get()->make_current();
	}
}
#endif

void World::_update_listener() {}
void World::_update_listener_2d() {}

void World::_propagate_enter_world(Node *p_node) {
	if (p_node != this) {
		if (!p_node->is_inside_tree()) { //may not have entered scene yet
			return;
		}

		if (Object::cast_to<Spatial>(p_node) || Object::cast_to<WorldEnvironment3D>(p_node)) {
			p_node->notification(Spatial::NOTIFICATION_ENTER_WORLD);
		} else {
			World *v = Object::cast_to<World>(p_node);
			if (v) {
				if (v->world_3d.is_valid() || v->own_world_3d.is_valid()) {
					return;
				}
			}
		}
	}

	for (int i = 0; i < p_node->get_child_count(); i++) {
		_propagate_enter_world(p_node->get_child(i));
	}
}

void World::_propagate_exit_world(Node *p_node) {
	if (p_node != this) {
		if (!p_node->is_inside_tree()) { //may have exited scene already
			return;
		}

		if (Object::cast_to<Spatial>(p_node) || Object::cast_to<WorldEnvironment3D>(p_node)) {
			p_node->notification(Spatial::NOTIFICATION_EXIT_WORLD);
		} else {
			World *v = Object::cast_to<World>(p_node);
			if (v) {
				if (v->world_3d.is_valid() || v->own_world_3d.is_valid()) {
					return;
				}
			}
		}
	}

	for (int i = 0; i < p_node->get_child_count(); i++) {
		_propagate_exit_world(p_node->get_child(i));
	}
}

void World::_add_overridden_world(World *p_world) {
	ERR_FAIL_COND(!p_world);

	_overriding_worlds.push_back(p_world);
}
void World::_remove_overridden_world(World *p_world) {
	ERR_FAIL_COND(!p_world);

	for (int i = 0; i < _overriding_worlds.size(); ++i) {
		World *w = _overriding_worlds[i];

		if (w == p_world) {
			_overriding_worlds.remove(i);
			return;
		}
	}

	ERR_FAIL();
}

void World::_own_world_3d_changed() {
	ERR_FAIL_COND(world_3d.is_null());
	ERR_FAIL_COND(own_world_3d.is_null());

	if (is_inside_tree()) {
		_propagate_exit_world(this);
	}

	own_world_3d = world_3d->duplicate();

	if (is_inside_tree()) {
		_propagate_enter_world(this);
	}
}

void World::_on_set_use_own_world_3d(bool p_use_own_world_3d) {
}

void World::_on_set_world_3d(const Ref<World3D> &p_old_world) {
}
void World::_on_set_world_2d(const Ref<World2D> &p_old_world_2d) {
}

void World::_on_before_world_override_changed() {
}
void World::_on_after_world_override_changed() {
}

void World::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
			if (get_parent()) {
				_parent_world = get_parent()->get_world();
			} else {
				_parent_world = nullptr;
			}
		} break;
		case NOTIFICATION_READY: {
			if (_override_in_parent_viewport && !Engine::get_singleton()->is_editor_hint()) {
				if (get_parent()) {
					World *w = get_parent()->get_viewport();

					if (w) {
						w->set_override_world(this);
					}
				}
			}
		} break;
		case NOTIFICATION_EXIT_TREE: {
			if (!Engine::get_singleton()->is_editor_hint()) {
				set_override_world(NULL);

				while (_overriding_worlds.size() > 0) {
					World *w = _overriding_worlds[0];

					w->set_override_world(NULL);
				}
			}
		} break;
	}
}

void World::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_camera"), &World::get_camera);

	ClassDB::bind_method(D_METHOD("get_world_2d"), &World::get_world_2d);
	ClassDB::bind_method(D_METHOD("set_world_2d", "world_2d"), &World::set_world_2d);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "world_2d", PROPERTY_HINT_RESOURCE_TYPE, "World2D", 0), "set_world_2d", "get_world_2d");

	ClassDB::bind_method(D_METHOD("get_world_3d"), &World::get_world_3d);
	ClassDB::bind_method(D_METHOD("set_world_3d", "world"), &World::set_world_3d);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "world", PROPERTY_HINT_RESOURCE_TYPE, "World3D"), "set_world_3d", "get_world_3d");

	ClassDB::bind_method(D_METHOD("is_using_own_world_3d"), &World::is_using_own_world_3d);
	ClassDB::bind_method(D_METHOD("set_use_own_world_3d", "enable"), &World::set_use_own_world_3d);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "own_world_3d"), "set_use_own_world_3d", "is_using_own_world_3d");

	ClassDB::bind_method(D_METHOD("get_override_in_parent_viewport"), &World::get_override_in_parent_viewport);
	ClassDB::bind_method(D_METHOD("set_override_in_parent_viewport", "enable"), &World::set_override_in_parent_viewport);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "override_in_parent_viewport"), "set_override_in_parent_viewport", "get_override_in_parent_viewport");

	ClassDB::bind_method(D_METHOD("find_world_2d"), &World::find_world_2d);
	ClassDB::bind_method(D_METHOD("find_world_3d"), &World::find_world_3d);

	ClassDB::bind_method(D_METHOD("get_override_world"), &World::get_override_world);
	ClassDB::bind_method(D_METHOD("get_override_world_or_this"), &World::get_override_world_or_this);
	ClassDB::bind_method(D_METHOD("set_override_world", "world"), &World::set_override_world_bind);

	ClassDB::bind_method(D_METHOD("_own_world_3d_changed"), &World::_own_world_3d_changed);

	ClassDB::bind_method(D_METHOD("set_canvas_transform", "xform"), &World::set_canvas_transform);
	ClassDB::bind_method(D_METHOD("get_canvas_transform"), &World::get_canvas_transform);
	ADD_PROPERTY(PropertyInfo(Variant::TRANSFORM2D, "canvas_transform", PROPERTY_HINT_NONE, "", 0), "set_canvas_transform", "get_canvas_transform");

	ClassDB::bind_method(D_METHOD("set_global_canvas_transform", "xform"), &World::set_global_canvas_transform);
	ClassDB::bind_method(D_METHOD("get_global_canvas_transform"), &World::get_global_canvas_transform);
	ADD_PROPERTY(PropertyInfo(Variant::TRANSFORM2D, "global_canvas_transform", PROPERTY_HINT_NONE, "", 0), "set_global_canvas_transform", "get_global_canvas_transform");

	ClassDB::bind_method(D_METHOD("get_final_transform"), &World::get_final_transform);

	ClassDB::bind_method(D_METHOD("get_visible_rect"), &World::get_visible_rect);
	ClassDB::bind_method(D_METHOD("get_viewport_rid"), &World::get_viewport_rid);

	ClassDB::bind_method(D_METHOD("update_worlds"), &World::update_worlds);
}
