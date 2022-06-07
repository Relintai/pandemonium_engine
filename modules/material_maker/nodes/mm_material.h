#ifndef MAT_MAKER_MATERIAL_H
#define MAT_MAKER_MATERIAL_H

#include "core/image.h"
#include "core/math/vector2.h"
#include "core/math/vector3.h"
#include "core/reference.h"
#include "core/variant.h"
#include "core/vector.h"

#include "core/resource.h"

class MMNode;
class ThreadPoolExecuteJob;

class MMMaterial : public Resource {
	GDCLASS(MMMaterial, Resource);

public:
	Vector2 get_image_size();
	void set_image_size(const Vector2 &val);

	Vector<Variant> get_nodes();
	void set_nodes(const Vector<Variant> &val);

	bool get_initialized() const;
	void set_initialized(const bool val);

	bool get_rendering() const;
	void set_rendering(const bool val);

	bool get_queued_render() const;
	void set_queued_render(const bool val);

	void initialize();

	void add_node(const Ref<MMNode> &node);
	void remove_node(const Ref<MMNode> &node);

	void render();
	void render_non_threaded();
	void render_threaded();

	void _thread_func();
	void cancel_render_and_wait();
	void on_node_changed();

	MMMaterial();
	~MMMaterial();

	Vector2 image_size;
	Vector<Ref<MMNode>> nodes;

protected:
	static void _bind_methods();

	//TODO detect if threadpool is enabled.
	bool USE_THREADS;
	bool initialized;
	bool rendering;
	bool queued_render;
	Ref<ThreadPoolExecuteJob> job;
};

#endif
