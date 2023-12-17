#ifndef PROCESS_GROUP_H
#define PROCESS_GROUP_H

/*************************************************************************/
/*  process_group.h                                                      */
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

#include "core/os/safe_refcount.h"
#include "core/os/semaphore.h"
#include "core/os/thread.h"
#include "core/os/thread_safe.h"

#include "scene/main/node.h"

class ProcessGroup : public Node {
	GDCLASS(ProcessGroup, Node);

	_THREAD_SAFE_CLASS_

public:
	enum ProcessMode {
		PROCESS_MODE_SCENE_TREE,
		PROCESS_MODE_NORMAL,
		PROCESS_MODE_MANUAL,
	};

	enum Mode {
		MODE_WAIT,
		MODE_TRIGGER,
		MODE_TRIGGER_UNIQUE,
		MODE_TRIGGER_DEFERRED,
		MODE_OFF,
	};

	enum ProcessGroupFlags {
		PROCESS_GROUP_FLAG_PROCESS = 1 << 0,
		PROCESS_GROUP_FLAG_PHYSICS_PROCESS = 1 << 1,
	};

	enum {
		NOTIFICATION_PROCESS_GROUP_PROCESS = 60,
		NOTIFICATION_PROCESS_GROUP_INTERNAL_PROCESS,
		NOTIFICATION_PROCESS_GROUP_PHYSICS_PROCESS,
		NOTIFICATION_PROCESS_GROUP_INTERNAL_PHYSICS_PROCESS,
	};

public:
	ProcessMode get_process_mode() const;
	void set_process_mode(const ProcessMode value);

	Mode get_mode() const;
	void set_mode(const Mode value);

	int get_group_flags() const;
	void set_group_flags(const int value);

	bool get_use_priority() const;
	void set_use_priority(const bool value);

	bool get_use_threads() const;
	void set_use_threads(const bool value);

	void trigger_process();
	void wait_process();
	void trigger_physics_process();
	void wait_physics_process();

	void register_node_process(Node *p_node);
	void unregister_node_process(Node *p_node);
	void node_process_changed(Node *p_node);

	void register_node_internal_process(Node *p_node);
	void unregister_node_internal_process(Node *p_node);
	void node_internal_process_changed(Node *p_node);

	void register_node_physics_process(Node *p_node);
	void unregister_node_physics_process(Node *p_node);
	void node_physics_process_changed(Node *p_node);

	void register_node_internal_physics_process(Node *p_node);
	void unregister_node_internal_physics_process(Node *p_node);
	void node_internal_physics_process_changed(Node *p_node);

	//This way the property doesn't lose it's vlaue when using a no thread build
	bool should_use_threads() const;

	bool is_working() const;

	String get_configuration_warning() const;

	ProcessGroup();
	~ProcessGroup();

protected:
	struct Group {
		Vector<Node *> nodes;
		bool changed;
		Group() { changed = false; };
	};

	enum CurrentProcessType {
		CURRENT_PROCESS_TYPE_NONE,
		CURRENT_PROCESS_TYPE_PROCESS,
		CURRENT_PROCESS_TYPE_PHYSICS_PROCESS,
	};

	void _setup();
	void _cleanup();

	void _setup_thread();
	void _quit_thread();
	void _quit_thread_deferred();
	void _cleanup_thread();

	void _register_scene_tree_groups();
	void _unregister_scene_tree_groups();

	void _trigger_process_deferred();

	void _handle_process();
	void _handle_physics_process();

	void _update_group_order(Group &g);

	static void _thread_func(void *udata);

	void _notification(int p_what);
	static void _bind_methods();

	Group _process_group;
	Group _internal_process_group;
	Group _physics_process_group;
	Group _internal_physics_process_group;

	ProcessMode _process_mode;
	Mode _mode;
	int _group_flags;
	bool _use_priority;
	bool _use_threads;

	bool _tread_run;
	Thread *_thread;
	Semaphore _process_semaphore;
	Semaphore _main_semaphore;

	SafeNumeric<int> _current_process_type;
};

VARIANT_ENUM_CAST(ProcessGroup::ProcessMode);
VARIANT_ENUM_CAST(ProcessGroup::Mode);
VARIANT_ENUM_CAST(ProcessGroup::ProcessGroupFlags);

#endif
