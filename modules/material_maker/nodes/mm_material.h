#ifndef MM_MATERIAL_H
#define MM_MATERIAL_H

class MMMateial : public Resource {
	GDCLASS(MMMateial, Resource);

public:
	Vector2 get_image_size();
	void set_image_size(const Vector2 &val);

	Array get_nodes();
	void set_nodes(const Array &val);

	bool get_initialized() const;
	void set_initialized(const bool val);

	bool get_rendering() const;
	void set_rendering(const bool val);

	bool get_queued_render() const;
	void set_queued_render(const bool val);

	Ref<ThreadPoolExecuteJob> get_job();
	void set_job(const Ref<ThreadPoolExecuteJob> &val);

	void initialize();
	void add_node(const MMNode &node);
	void remove_node(const MMNode &node);
	void render();
	void render_non_threaded();
	void render_threaded();
	void _thread_func();
	void cancel_render_and_wait();
	void on_node_changed();

	MMMateial();
	~MMMateial();

protected:
	static void _bind_methods();

	//tool
	//threads are implemented using my thread pool engine module.
	//if you want to use this without that module in your engine set this to false,
	//and comment out the lines that give errors
	const USE_THREADS = true;
	//export(Vector2)
	Vector2 image_size = Vector2(128, 128);
	//export(Array)
	Array nodes = ;
	bool initialized = false;
	bool rendering = false;
	bool queued_render = false;
	Ref<ThreadPoolExecuteJob> job = ThreadPoolExecuteJob.new();
};

#endif
