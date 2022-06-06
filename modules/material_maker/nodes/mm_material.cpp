
#include "mm_material.h"

Vector2 MMMateial::get_image_size() {
	return image_size;
}

void MMMateial::set_image_size(const Vector2 &val) {
	image_size = val;
}

Array MMMateial::get_nodes() {
	return nodes;
}

void MMMateial::set_nodes(const Array &val) {
	nodes = val;
}

bool MMMateial::get_initialized() const {
	return initialized;
}

void MMMateial::set_initialized(const bool val) {
	initialized = val;
}

bool MMMateial::get_rendering() const {
	return rendering;
}

void MMMateial::set_rendering(const bool val) {
	rendering = val;
}

bool MMMateial::get_queued_render() const {
	return queued_render;
}

void MMMateial::set_queued_render(const bool val) {
	queued_render = val;
}

Ref<ThreadPoolExecuteJob> MMMateial::get_job() {
	return job;
}

void MMMateial::set_job(const Ref<ThreadPoolExecuteJob> &val) {
	job = val;
}

//tool;
//threads are implemented using my thread pool engine module.;
//if you want to use this without that module in your engine set this to false,;
//and comment out the lines that give errors;
const USE_THREADS = true;
//export(Vector2) ;
Vector2 image_size = Vector2(128, 128);
//export(Array) ;
Array nodes = ;
bool initialized = false;
bool rendering = false;
bool queued_render = false;
Ref<ThreadPoolExecuteJob> job = ThreadPoolExecuteJob.new();

void MMMateial::initialize() {
	if (!initialized) {
		initialized = true;
		job.setup(self, "_thread_func");

		for (n in nodes) {
			n.init_properties();
			n.connect("changed", self, "on_node_changed");
		}
	}
}

void MMMateial::add_node(const MMNode &node) {
	nodes.append(node);
	node.connect("changed", self, "on_node_changed");
	emit_changed();
}

void MMMateial::remove_node(const MMNode &node) {
	if (!node) {
		return;
	}

	for (op in node.output_properties) {
		for (n in nodes) {
			if (n) {
				for (ip in n.input_properties) {
					if (ip.input_property == op) {
						ip.set_input_property(null);
					}
				}
			}
		}
	}

	nodes.erase(node);
	node.disconnect("changed", self, "on_node_changed");
	emit_changed();
}

void MMMateial::render() {
	initialize();

	if (rendering) {
		queued_render = true;
		return;
	}

	if (USE_THREADS) {
		render_threaded();
	}

	else {
		render_non_threaded();
	}
}

void MMMateial::render_non_threaded() {
	bool did_render = true;

	while (did_render) {
		did_render = false;

		for (n in nodes) {
			if (n && n.render(self)) {
				did_render = true;
			}
		}
	}
}

void MMMateial::render_threaded() {
	job.cancelled = false;

	if (!ThreadPool.has_job(job)) {
		ThreadPool.add_job(job);
	}
}

void MMMateial::_thread_func() {
	if (job.cancelled) {
		rendering = false;
		return;
	}

	rendering = true;
	job.cancelled = false;
	bool did_render = true;

	while (did_render) {
		did_render = false;

		for (n in nodes) {
			if (n && n.render(self)) {
				did_render = true;
			}

			if (job.cancelled) {
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

void MMMateial::cancel_render_and_wait() {
	if (rendering) {
		ThreadPool.cancel_job_wait(job);
		job.cancelled = false;
		pass;
	}
}

void MMMateial::on_node_changed() {
	emit_changed();
	call_deferred("render");
}
}

MMMateial::MMMateial() {
	image_size = Vector2(128, 128);
	nodes = ;
	initialized = false;
	rendering = false;
	queued_render = false;
	job = ThreadPoolExecuteJob.new();
}

MMMateial::~MMMateial() {
}

static void MMMateial::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_image_size"), &MMMateial::get_image_size);
	ClassDB::bind_method(D_METHOD("set_image_size", "value"), &MMMateial::set_image_size);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "image_size"), "set_image_size", "get_image_size");

	ClassDB::bind_method(D_METHOD("get_nodes"), &MMMateial::get_nodes);
	ClassDB::bind_method(D_METHOD("set_nodes", "value"), &MMMateial::set_nodes);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "nodes"), "set_nodes", "get_nodes");

	ClassDB::bind_method(D_METHOD("get_initialized"), &MMMateial::get_initialized);
	ClassDB::bind_method(D_METHOD("set_initialized", "value"), &MMMateial::set_initialized);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "initialized"), "set_initialized", "get_initialized");

	ClassDB::bind_method(D_METHOD("get_rendering"), &MMMateial::get_rendering);
	ClassDB::bind_method(D_METHOD("set_rendering", "value"), &MMMateial::set_rendering);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "rendering"), "set_rendering", "get_rendering");

	ClassDB::bind_method(D_METHOD("get_queued_render"), &MMMateial::get_queued_render);
	ClassDB::bind_method(D_METHOD("set_queued_render", "value"), &MMMateial::set_queued_render);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "queued_render"), "set_queued_render", "get_queued_render");

	ClassDB::bind_method(D_METHOD("get_job"), &MMMateial::get_job);
	ClassDB::bind_method(D_METHOD("set_job", "value"), &MMMateial::set_job);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "job", PROPERTY_HINT_RESOURCE_TYPE, "Ref<ThreadPoolExecuteJob>"), "set_job", "get_job");

	ClassDB::bind_method(D_METHOD("initialize"), &MMMateial::initialize);
	ClassDB::bind_method(D_METHOD("add_node", "node"), &MMMateial::add_node);
	ClassDB::bind_method(D_METHOD("remove_node", "node"), &MMMateial::remove_node);
	ClassDB::bind_method(D_METHOD("render"), &MMMateial::render);
	ClassDB::bind_method(D_METHOD("render_non_threaded"), &MMMateial::render_non_threaded);
	ClassDB::bind_method(D_METHOD("render_threaded"), &MMMateial::render_threaded);
	ClassDB::bind_method(D_METHOD("_thread_func"), &MMMateial::_thread_func);
	ClassDB::bind_method(D_METHOD("cancel_render_and_wait"), &MMMateial::cancel_render_and_wait);
	ClassDB::bind_method(D_METHOD("on_node_changed"), &MMMateial::on_node_changed);
}
