/*************************************************************************/
/*  process_group.cpp                                                    */
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

#include "process_group.h"

#include "core/config/engine.h"

#include "scene_tree.h"

ProcessGroup::ProcessMode ProcessGroup::get_process_mode() const {
	return _process_mode;
}
void ProcessGroup::set_process_mode(const ProcessGroup::ProcessMode value) {
	_THREAD_SAFE_METHOD_

	if (_process_mode == value) {
		return;
	}

	_unregister_scene_tree_groups();

	_process_mode = value;

	if (_process_mode == PROCESS_MODE_SCENE_TREE) {
		_register_scene_tree_groups();
	}

	if (_process_mode == PROCESS_MODE_NORMAL) {
		if ((_group_flags & PROCESS_GROUP_FLAG_PROCESS) != 0) {
			set_process_internal(true);
		} else {
			set_process_internal(false);
		}

		if ((_group_flags & PROCESS_GROUP_FLAG_PHYSICS_PROCESS) != 0) {
			set_physics_process_internal(true);
		} else {
			set_physics_process_internal(false);
		}
	} else {
		set_process_internal(false);
		set_physics_process_internal(false);
	}

	//manual doesn't need anything
}

ProcessGroup::Mode ProcessGroup::get_mode() const {
	return _mode;
}
void ProcessGroup::set_mode(const ProcessGroup::Mode value) {
	_THREAD_SAFE_METHOD_

	if (_mode == value) {
		return;
	}

	_mode = value;
}

int ProcessGroup::get_group_flags() const {
	return _group_flags;
}
void ProcessGroup::set_group_flags(const int value) {
	_THREAD_SAFE_METHOD_

	if (_group_flags == value) {
		return;
	}

	_unregister_scene_tree_groups();

	_group_flags = value;

	_register_scene_tree_groups();

	if (_process_mode == PROCESS_MODE_NORMAL) {
		if ((_group_flags & PROCESS_GROUP_FLAG_PROCESS) != 0) {
			set_process_internal(true);
		} else {
			set_process_internal(false);
		}

		if ((_group_flags & PROCESS_GROUP_FLAG_PHYSICS_PROCESS) != 0) {
			set_physics_process_internal(true);
		} else {
			set_physics_process_internal(false);
		}
	}
}

bool ProcessGroup::get_use_priority() const {
	return _use_priority;
}
void ProcessGroup::set_use_priority(const bool value) {
	_use_priority = value;
}

bool ProcessGroup::get_use_threads() const {
	return _use_threads;
}
void ProcessGroup::set_use_threads(const bool value) {
	_THREAD_SAFE_METHOD_

	if (_use_threads == value) {
		return;
	}

	_use_threads = value;

	if (!should_use_threads()) {
		_quit_thread();
	} else {
		_setup_thread();
	}
}

void ProcessGroup::trigger_process() {
	if (_mode == MODE_OFF) {
		return;
	}

	if ((_group_flags & PROCESS_GROUP_FLAG_PROCESS) == 0) {
		return;
	}

	if (should_use_threads()) {
		ERR_FAIL_COND(is_working());

		switch (_mode) {
			case MODE_WAIT: {
				_current_process_type.set(CURRENT_PROCESS_TYPE_PROCESS);
				_process_semaphore.post();
			} break;
			case MODE_TRIGGER: {
				_current_process_type.set(CURRENT_PROCESS_TYPE_PROCESS);
				_process_semaphore.post();
			} break;
			case MODE_TRIGGER_UNIQUE: {
				if (_process_semaphore.get() > 0) {
					return;
				}

				_current_process_type.set(CURRENT_PROCESS_TYPE_PROCESS);
				_process_semaphore.post();
			} break;
			case MODE_TRIGGER_DEFERRED: {
				call_deferred("_trigger_process_deferred");
			} break;
			case MODE_OFF:
			default:
				break;
		}
	} else {
		switch (_mode) {
			case MODE_WAIT:
			case MODE_TRIGGER:
			case MODE_TRIGGER_UNIQUE: {
				_handle_process();
			} break;
			case MODE_TRIGGER_DEFERRED: {
				call_deferred("_handle_process");
			} break;
			case MODE_OFF:
			default:
				break;
		}
	}
}
void ProcessGroup::wait_process() {
	if (_mode == MODE_OFF) {
		return;
	}

	if ((_group_flags & PROCESS_GROUP_FLAG_PROCESS) == 0) {
		return;
	}

	if (should_use_threads()) {
		if (!is_working()) {
			return;
		}

		switch (_mode) {
			case MODE_WAIT: {
				// if the worker thread finished, this will just consume the count from the semaphore
				_main_semaphore.wait();
			} break;
			case MODE_TRIGGER:
			case MODE_TRIGGER_UNIQUE:
			case MODE_TRIGGER_DEFERRED:
			case MODE_OFF:
			default: {
				// No waiting needed for these
			} break;
		}
	}
}
void ProcessGroup::trigger_physics_process() {
	//No physics in editor
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}

	if (_mode == MODE_OFF) {
		return;
	}

	if ((_group_flags & PROCESS_GROUP_FLAG_PHYSICS_PROCESS) == 0) {
		return;
	}

	if (should_use_threads()) {
		ERR_FAIL_COND(is_working());

		switch (_mode) {
			case MODE_WAIT: {
				_current_process_type.set(CURRENT_PROCESS_TYPE_PHYSICS_PROCESS);
				_process_semaphore.post();
			} break;
			case MODE_TRIGGER:
			case MODE_TRIGGER_UNIQUE:
			case MODE_TRIGGER_DEFERRED:
			case MODE_OFF:
			default: {
				//Won't work with physics process. Unsafe.
			} break;
		}
	} else {
		switch (_mode) {
			case MODE_WAIT:
			case MODE_TRIGGER:
			case MODE_TRIGGER_UNIQUE: {
				_handle_physics_process();
			} break;
			case MODE_TRIGGER_DEFERRED:
			case MODE_OFF:
			default: {
				//Won't work with physics process. Unsafe.
			} break;
		}
	}
}
void ProcessGroup::wait_physics_process() {
	//No physics in editor
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}

	if (_mode == MODE_OFF) {
		return;
	}

	if ((_group_flags & PROCESS_GROUP_FLAG_PHYSICS_PROCESS) == 0) {
		return;
	}

	if (should_use_threads()) {
		if (!is_working()) {
			return;
		}

		switch (_mode) {
			case MODE_WAIT: {
				// if the worker thread finished, this will just consume the count from the semaphore
				_main_semaphore.wait();
			} break;
			case MODE_TRIGGER:
			case MODE_TRIGGER_UNIQUE:
			case MODE_TRIGGER_DEFERRED:
			case MODE_OFF:
			default: {
				// No waiting needed for these
			} break;
		}
	}
}

void ProcessGroup::register_node_process(Node *p_node) {
	_THREAD_SAFE_METHOD_

	ERR_FAIL_COND_MSG(_process_group.nodes.find(p_node) != -1, "Node already registered for process.");
	_process_group.nodes.push_back(p_node);
	_process_group.changed = true;
}
void ProcessGroup::unregister_node_process(Node *p_node) {
	_THREAD_SAFE_METHOD_

	_process_group.nodes.erase(p_node);
}
void ProcessGroup::node_process_changed(Node *p_node) {
	_THREAD_SAFE_METHOD_

	_process_group.changed = true;
}

void ProcessGroup::register_node_internal_process(Node *p_node) {
	_THREAD_SAFE_METHOD_

	ERR_FAIL_COND_MSG(_internal_process_group.nodes.find(p_node) != -1, "Node already registered for process.");
	_internal_process_group.nodes.push_back(p_node);
	_internal_process_group.changed = true;
}
void ProcessGroup::unregister_node_internal_process(Node *p_node) {
	_THREAD_SAFE_METHOD_

	_internal_process_group.nodes.erase(p_node);
}
void ProcessGroup::node_internal_process_changed(Node *p_node) {
	_THREAD_SAFE_METHOD_

	_internal_process_group.changed = true;
}

void ProcessGroup::register_node_physics_process(Node *p_node) {
	_THREAD_SAFE_METHOD_

	ERR_FAIL_COND_MSG(_physics_process_group.nodes.find(p_node) != -1, "Node already registered for process.");
	_physics_process_group.nodes.push_back(p_node);
	_physics_process_group.changed = true;
}
void ProcessGroup::unregister_node_physics_process(Node *p_node) {
	_THREAD_SAFE_METHOD_

	_physics_process_group.nodes.erase(p_node);
}
void ProcessGroup::node_physics_process_changed(Node *p_node) {
	_THREAD_SAFE_METHOD_

	_physics_process_group.changed = true;
}

void ProcessGroup::register_node_internal_physics_process(Node *p_node) {
	_THREAD_SAFE_METHOD_

	ERR_FAIL_COND_MSG(_internal_physics_process_group.nodes.find(p_node) != -1, "Node already registered for process.");
	_internal_physics_process_group.nodes.push_back(p_node);
	_internal_physics_process_group.changed = true;
}
void ProcessGroup::unregister_node_internal_physics_process(Node *p_node) {
	_THREAD_SAFE_METHOD_

	_internal_physics_process_group.nodes.erase(p_node);
}
void ProcessGroup::node_internal_physics_process_changed(Node *p_node) {
	_THREAD_SAFE_METHOD_

	_internal_physics_process_group.changed = true;
}

bool ProcessGroup::should_use_threads() const {
#if !defined(NO_THREADS)
	return _use_threads;
#else
	return false;
#endif
}

bool ProcessGroup::is_working() const {
	if (_mode == MODE_WAIT) {
		return _current_process_type.get() != CURRENT_PROCESS_TYPE_NONE;
	}

	return false;
}

String ProcessGroup::get_configuration_warning() const {
	String w;

	if ((_group_flags & PROCESS_GROUP_FLAG_PHYSICS_PROCESS) != 0) {
		// _use_threads -> need to specifically check for the property's value here
		if (_use_threads) {
			switch (_mode) {
				case MODE_WAIT: {
					//OK
				} break;
				case MODE_TRIGGER:
				case MODE_TRIGGER_UNIQUE:
				case MODE_TRIGGER_DEFERRED: {
					w += "In Trigger, Trigger Unique, Trigger Deferred modes physics process group flag cannot be used when threading is on, and it will do nothing as it's only safe to communicate with the physics server at certain times!";
				} break;
				case MODE_OFF:
				default:
					break;
			}
		} else {
			switch (_mode) {
				case MODE_WAIT:
				case MODE_TRIGGER:
				case MODE_TRIGGER_UNIQUE: {
					// OK
				} break;
				case MODE_TRIGGER_DEFERRED: {
					w += "In Trigger Deferred mode physics process group flag cannot be used when threading is off, and it will do nothing as it's only safe to communicate with the physics server at certain times!";
				} break;
				case MODE_OFF:
				default:
					break;
			}
		}
	}

	return w;
}

ProcessGroup::ProcessGroup() {
	_process_mode = PROCESS_MODE_SCENE_TREE;
	_mode = MODE_WAIT;
	_group_flags = PROCESS_GROUP_FLAG_PROCESS;
	_use_priority = false;
	_use_threads = true;

	_tread_run = true;

	_thread = NULL;
	_current_process_type.set(CURRENT_PROCESS_TYPE_NONE);
}

ProcessGroup::~ProcessGroup() {
	_cleanup();
}

void ProcessGroup::_setup() {
	_current_process_type.set(CURRENT_PROCESS_TYPE_NONE);
	_setup_thread();
}

void ProcessGroup::_cleanup() {
	_tread_run = false;
	_process_semaphore.post();
	_cleanup_thread();
}

void ProcessGroup::_setup_thread() {
	if (!should_use_threads()) {
		return;
	}

	if (_thread) {
		return;
	}

	_tread_run = true;

	_thread = memnew(Thread);
	_thread->start(_thread_func, this);
}
void ProcessGroup::_quit_thread() {
	if (!_tread_run || !_thread) {
		return;
	}

	_tread_run = false;
	_process_semaphore.post();
	_cleanup_thread();
}
void ProcessGroup::_quit_thread_deferred() {
	if (!_tread_run || !_thread) {
		return;
	}

	_tread_run = false;
	_process_semaphore.post();
	call_deferred("_cleanup_thread");
}
void ProcessGroup::_cleanup_thread() {
	if (_tread_run) {
		_tread_run = false;
		_process_semaphore.post();
	}

	if (!_thread) {
		return;
	}

	_thread->wait_to_finish();
	memdelete(_thread);
	_thread = NULL;
}

void ProcessGroup::_register_scene_tree_groups() {
	if (!is_inside_tree()) {
		return;
	}

	if (_process_mode != PROCESS_MODE_SCENE_TREE) {
		return;
	}

	if ((_group_flags & PROCESS_GROUP_FLAG_PROCESS) != 0) {
		add_to_group("_pg_process");
	}

	if ((_group_flags & PROCESS_GROUP_FLAG_PHYSICS_PROCESS) != 0) {
		add_to_group("_pg_physics_process");
	}
}
void ProcessGroup::_unregister_scene_tree_groups() {
	if (!is_inside_tree()) {
		return;
	}

	if (_process_mode != PROCESS_MODE_SCENE_TREE) {
		return;
	}

	if ((_group_flags & PROCESS_GROUP_FLAG_PROCESS) != 0) {
		remove_from_group("_pg_process");
	}

	if ((_group_flags & PROCESS_GROUP_FLAG_PHYSICS_PROCESS) != 0) {
		remove_from_group("_pg_physics_process");
	}
}

void ProcessGroup::_trigger_process_deferred() {
	if (should_use_threads()) {
		_current_process_type.set(CURRENT_PROCESS_TYPE_PROCESS);
		_process_semaphore.post();
	} else {
		_handle_process();
	}
}

void ProcessGroup::_handle_process() {
	Vector<Node *> nodes_copy;

	// Normal Process
	{
		_THREAD_SAFE_METHOD_
		_update_group_order(_process_group);
		nodes_copy = _process_group.nodes;
	}

	Node **nodes = nodes_copy.ptrw();
	int node_count = nodes_copy.size();

	for (int i = 0; i < node_count; i++) {
		Node *n = nodes[i];

		if (!n->can_process()) {
			continue;
		}

		if (!n->can_process_notification(NOTIFICATION_PROCESS_GROUP_PROCESS)) {
			continue;
		}

		n->notification(NOTIFICATION_PROCESS_GROUP_PROCESS);
	}

	// Internal Process
	{
		_THREAD_SAFE_METHOD_
		_update_group_order(_internal_process_group);
		nodes_copy = _internal_process_group.nodes;
	}

	nodes = nodes_copy.ptrw();
	node_count = nodes_copy.size();

	for (int i = 0; i < node_count; i++) {
		Node *n = nodes[i];

		if (!n->can_process()) {
			continue;
		}

		if (!n->can_process_notification(NOTIFICATION_PROCESS_GROUP_INTERNAL_PROCESS)) {
			continue;
		}

		n->notification(NOTIFICATION_PROCESS_GROUP_INTERNAL_PROCESS);
	}

	real_t delta = SceneTree::get_singleton()->get_idle_process_time();
	emit_signal("process", delta);
}
void ProcessGroup::_handle_physics_process() {
	Vector<Node *> nodes_copy;

	// Normal Physics Process
	{
		_THREAD_SAFE_METHOD_
		_update_group_order(_physics_process_group);
		nodes_copy = _physics_process_group.nodes;
	}

	Node **nodes = nodes_copy.ptrw();
	int node_count = nodes_copy.size();

	for (int i = 0; i < node_count; i++) {
		Node *n = nodes[i];

		if (!n->can_process()) {
			continue;
		}

		if (!n->can_process_notification(NOTIFICATION_PROCESS_GROUP_PHYSICS_PROCESS)) {
			continue;
		}

		n->notification(NOTIFICATION_PROCESS_GROUP_PHYSICS_PROCESS);
	}

	// Internal Physics Process
	{
		_THREAD_SAFE_METHOD_
		_update_group_order(_internal_physics_process_group);
		nodes_copy = _internal_physics_process_group.nodes;
	}

	nodes = nodes_copy.ptrw();
	node_count = nodes_copy.size();

	for (int i = 0; i < node_count; i++) {
		Node *n = nodes[i];

		if (!n->can_process()) {
			continue;
		}

		if (!n->can_process_notification(NOTIFICATION_PROCESS_GROUP_INTERNAL_PHYSICS_PROCESS)) {
			continue;
		}

		n->notification(NOTIFICATION_PROCESS_GROUP_INTERNAL_PHYSICS_PROCESS);
	}

	real_t delta = SceneTree::get_singleton()->get_physics_process_time();
	emit_signal("process", delta);
}

void ProcessGroup::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
			_setup();

			_register_scene_tree_groups();
		} break;
		case NOTIFICATION_EXIT_TREE: {
			_unregister_scene_tree_groups();

			_cleanup();
		} break;
		case NOTIFICATION_INTERNAL_PROCESS: {
			trigger_process();
		} break;
		case NOTIFICATION_INTERNAL_PHYSICS_PROCESS: {
			trigger_physics_process();
		} break;
	}
}

void ProcessGroup::_update_group_order(Group &g) {
	if (!g.changed) {
		return;
	}
	if (g.nodes.empty()) {
		return;
	}

	Node **nodes = g.nodes.ptrw();
	int node_count = g.nodes.size();

	if (_use_priority) {
		SortArray<Node *, Node::ComparatorWithPriority> node_sort;
		node_sort.sort(nodes, node_count);
	} else {
		SortArray<Node *, Node::Comparator> node_sort;
		node_sort.sort(nodes, node_count);
	}
	g.changed = false;
}

void ProcessGroup::_thread_func(void *udata) {
	ProcessGroup *self = static_cast<ProcessGroup *>(udata);

	while (self->_tread_run) {
		CurrentProcessType current_process_type = static_cast<CurrentProcessType>(self->_current_process_type.get());

		switch (current_process_type) {
			case CURRENT_PROCESS_TYPE_PROCESS: {
				self->_handle_process();
			} break;
			case CURRENT_PROCESS_TYPE_PHYSICS_PROCESS: {
				self->_handle_physics_process();
			} break;
			case CURRENT_PROCESS_TYPE_NONE:
			default: {
				// do nothing
			} break;
		}

		// Let the main thread continue
		if (current_process_type != CURRENT_PROCESS_TYPE_NONE && self->_mode == MODE_WAIT) {
			self->_current_process_type.set(CURRENT_PROCESS_TYPE_NONE);

			self->_main_semaphore.post();
		}

		self->_process_semaphore.wait();
	}
}

void ProcessGroup::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_process_mode"), &ProcessGroup::get_process_mode);
	ClassDB::bind_method(D_METHOD("set_process_mode", "value"), &ProcessGroup::set_process_mode);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "process_mode", PROPERTY_HINT_ENUM, "SceneTree,Normal,Manual"), "set_process_mode", "get_process_mode");

	ClassDB::bind_method(D_METHOD("get_mode"), &ProcessGroup::get_mode);
	ClassDB::bind_method(D_METHOD("set_mode", "value"), &ProcessGroup::set_mode);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "mode", PROPERTY_HINT_ENUM, "Wait,Trigger,Trigger unique,Trigger Deferred,Off"), "set_mode", "get_mode");

	ClassDB::bind_method(D_METHOD("get_group_flags"), &ProcessGroup::get_group_flags);
	ClassDB::bind_method(D_METHOD("set_group_flags", "value"), &ProcessGroup::set_group_flags);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "group_flags", PROPERTY_HINT_FLAGS, "Process,Physics Process"), "set_group_flags", "get_group_flags");

	ClassDB::bind_method(D_METHOD("get_use_priority"), &ProcessGroup::get_use_priority);
	ClassDB::bind_method(D_METHOD("set_use_priority", "value"), &ProcessGroup::set_use_priority);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_priority"), "set_use_priority", "get_use_priority");

	ClassDB::bind_method(D_METHOD("get_use_threads"), &ProcessGroup::get_use_threads);
	ClassDB::bind_method(D_METHOD("set_use_threads", "value"), &ProcessGroup::set_use_threads);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_threads"), "set_use_threads", "get_use_threads");

	ClassDB::bind_method(D_METHOD("trigger_process"), &ProcessGroup::trigger_process);
	ClassDB::bind_method(D_METHOD("wait_process"), &ProcessGroup::wait_process);
	ClassDB::bind_method(D_METHOD("trigger_physics_process"), &ProcessGroup::trigger_physics_process);
	ClassDB::bind_method(D_METHOD("wait_physics_process"), &ProcessGroup::wait_physics_process);

	ClassDB::bind_method(D_METHOD("should_use_threads"), &ProcessGroup::should_use_threads);

	ClassDB::bind_method(D_METHOD("is_working"), &ProcessGroup::is_working);

	ClassDB::bind_method(D_METHOD("_cleanup_thread"), &ProcessGroup::_cleanup_thread);
	ClassDB::bind_method(D_METHOD("_trigger_process_deferred"), &ProcessGroup::_trigger_process_deferred);

	ADD_SIGNAL(MethodInfo("process", PropertyInfo(Variant::REAL, "delta")));
	ADD_SIGNAL(MethodInfo("physics_process", PropertyInfo(Variant::REAL, "delta")));

	BIND_ENUM_CONSTANT(PROCESS_MODE_SCENE_TREE);
	BIND_ENUM_CONSTANT(PROCESS_MODE_NORMAL);
	BIND_ENUM_CONSTANT(PROCESS_MODE_MANUAL);

	BIND_ENUM_CONSTANT(MODE_WAIT);
	BIND_ENUM_CONSTANT(MODE_TRIGGER);
	BIND_ENUM_CONSTANT(MODE_TRIGGER_UNIQUE);
	BIND_ENUM_CONSTANT(MODE_TRIGGER_DEFERRED);
	BIND_ENUM_CONSTANT(MODE_OFF);

	BIND_ENUM_CONSTANT(PROCESS_GROUP_FLAG_PROCESS);
	BIND_ENUM_CONSTANT(PROCESS_GROUP_FLAG_PHYSICS_PROCESS);

	BIND_CONSTANT(NOTIFICATION_PROCESS_GROUP_PROCESS);
	BIND_CONSTANT(NOTIFICATION_PROCESS_GROUP_INTERNAL_PROCESS);
	BIND_CONSTANT(NOTIFICATION_PROCESS_GROUP_PHYSICS_PROCESS);
	BIND_CONSTANT(NOTIFICATION_PROCESS_GROUP_INTERNAL_PHYSICS_PROCESS);
}
