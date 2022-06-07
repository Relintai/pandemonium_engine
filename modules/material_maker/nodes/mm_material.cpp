
#include "mm_material.h"

#include "mm_node.h"
#include "mm_node_universal_property.h"
#include "modules/thread_pool/thread_pool.h"
#include "modules/thread_pool/thread_pool_job.h"

Vector2 MMMaterial::get_image_size() {
	return image_size;
}

void MMMaterial::set_image_size(const Vector2 &val) {
	image_size = val;
}

Vector<Variant> MMMaterial::get_nodes() {
	Vector<Variant> r;
	for (int i = 0; i < nodes.size(); i++) {
		r.push_back(nodes[i].get_ref_ptr());
	}
	return r;
}

void MMMaterial::set_nodes(const Vector<Variant> &val) {
	nodes.clear();
	for (int i = 0; i < val.size(); i++) {
		Ref<MMNode> e = Ref<MMNode>(val[i]);
		nodes.push_back(e);
	}
}

bool MMMaterial::get_initialized() const {
	return initialized;
}

void MMMaterial::set_initialized(const bool val) {
	initialized = val;
}

bool MMMaterial::get_rendering() const {
	return rendering;
}

void MMMaterial::set_rendering(const bool val) {
	rendering = val;
}

bool MMMaterial::get_queued_render() const {
	return queued_render;
}

void MMMaterial::set_queued_render(const bool val) {
	queued_render = val;
}

void MMMaterial::initialize() {
	if (!initialized) {
		initialized = true;
		job->setup(this, "_thread_func");

		for (int i = 0; i < nodes.size(); ++i) {
			Ref<MMNode> n = nodes[i];

			if (!n.is_valid()) {
				continue;
			}

			n->init_properties();
			n->connect("changed", this, "on_node_changed");
		}
	}
}

void MMMaterial::add_node(const Ref<MMNode> &node) {
	ERR_FAIL_COND(!node.is_valid());

	nodes.push_back(node);
	Ref<MMNode> n = node;
	n->connect("changed", this, "on_node_changed");
	emit_changed();
}

void MMMaterial::remove_node(const Ref<MMNode> &node) {
	if (!node.is_valid()) {
		return;
	}

	for (int i = 0; i < node->output_properties.size(); ++i) {
		Ref<MMNodeUniversalProperty> op = node->output_properties[i];

		for (int j = 0; j < nodes.size(); ++j) {
			Ref<MMNode> n = nodes[j];

			if (!n.is_valid()) {
				continue;
			}

			for (int k = 0; k < n->input_properties.size(); ++k) {
				Ref<MMNodeUniversalProperty> ip = n->input_properties[k];

				if (ip->get_input_property() == op) {
					ip->set_input_property(Ref<MMNodeUniversalProperty>());
				}
			}
		}
	}

	nodes.erase(node);

	Ref<MMNode> nn = node;

	nn->disconnect("changed", this, "on_node_changed");
	emit_changed();
}

void MMMaterial::render() {
	initialize();

	if (rendering) {
		queued_render = true;
		return;
	}

	if (USE_THREADS) {
		render_threaded();
	} else {
		render_non_threaded();
	}
}

void MMMaterial::render_non_threaded() {
	bool did_render = true;
	Ref<MMMaterial> self = Ref<MMMaterial>(this);

	while (did_render) {
		did_render = false;

		for (int i = 0; i < nodes.size(); ++i) {
			Ref<MMNode> n = nodes[i];

			if (n.is_valid() && n->render(self)) {
				did_render = true;
			}
		}
	}
}

void MMMaterial::render_threaded() {
	job->set_cancelled(false);

	if (!ThreadPool::get_singleton()->has_job(job)) {
		ThreadPool::get_singleton()->add_job(job);
	}
}

void MMMaterial::_thread_func() {
	if (job->get_cancelled()) {
		rendering = false;
		return;
	}

	rendering = true;
	job->set_cancelled(false);
	bool did_render = true;
	Ref<MMMaterial> self = Ref<MMMaterial>(this);

	while (did_render) {
		did_render = false;

		for (int i = 0; i < nodes.size(); ++i) {
			Ref<MMNode> n = nodes[i];

			if (n.is_valid() && n->render(self)) {
				did_render = true;
			}

			if (job->get_cancelled()) {
				rendering = false;
				return;
			}
		}
	}

	rendering = false;

	if (queued_render) {
		queued_render = false;
		_thread_func();
	}
}

void MMMaterial::cancel_render_and_wait() {
	if (rendering) {
		ThreadPool::get_singleton()->cancel_job_wait(job);
		job->set_cancelled(false);
	}
}

void MMMaterial::on_node_changed() {
	emit_changed();
	call_deferred("render");
}

MMMaterial::MMMaterial() {
	USE_THREADS = true;
	image_size = Vector2(128, 128);
	initialized = false;
	rendering = false;
	queued_render = false;
	job.instance();
}

MMMaterial::~MMMaterial() {
}

void MMMaterial::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_image_size"), &MMMaterial::get_image_size);
	ClassDB::bind_method(D_METHOD("set_image_size", "value"), &MMMaterial::set_image_size);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "image_size"), "set_image_size", "get_image_size");

	ClassDB::bind_method(D_METHOD("get_nodes"), &MMMaterial::get_nodes);
	ClassDB::bind_method(D_METHOD("set_nodes", "value"), &MMMaterial::set_nodes);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "nodes", PROPERTY_HINT_NONE, "17/17:MMNode", PROPERTY_USAGE_DEFAULT, "MMNode"), "set_nodes", "get_nodes");

	ClassDB::bind_method(D_METHOD("get_initialized"), &MMMaterial::get_initialized);
	ClassDB::bind_method(D_METHOD("set_initialized", "value"), &MMMaterial::set_initialized);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "initialized", PROPERTY_HINT_NONE, "", 0), "set_initialized", "get_initialized");

	ClassDB::bind_method(D_METHOD("get_rendering"), &MMMaterial::get_rendering);
	ClassDB::bind_method(D_METHOD("set_rendering", "value"), &MMMaterial::set_rendering);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "rendering", PROPERTY_HINT_NONE, "", 0), "set_rendering", "get_rendering");

	ClassDB::bind_method(D_METHOD("get_queued_render"), &MMMaterial::get_queued_render);
	ClassDB::bind_method(D_METHOD("set_queued_render", "value"), &MMMaterial::set_queued_render);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "queued_render", PROPERTY_HINT_NONE, "", 0), "set_queued_render", "get_queued_render");

	ClassDB::bind_method(D_METHOD("initialize"), &MMMaterial::initialize);

	ClassDB::bind_method(D_METHOD("add_node", "node"), &MMMaterial::add_node);
	ClassDB::bind_method(D_METHOD("remove_node", "node"), &MMMaterial::remove_node);

	ClassDB::bind_method(D_METHOD("render"), &MMMaterial::render);
	ClassDB::bind_method(D_METHOD("render_non_threaded"), &MMMaterial::render_non_threaded);
	ClassDB::bind_method(D_METHOD("render_threaded"), &MMMaterial::render_threaded);

	ClassDB::bind_method(D_METHOD("_thread_func"), &MMMaterial::_thread_func);

	ClassDB::bind_method(D_METHOD("cancel_render_and_wait"), &MMMaterial::cancel_render_and_wait);
	ClassDB::bind_method(D_METHOD("on_node_changed"), &MMMaterial::on_node_changed);
}
