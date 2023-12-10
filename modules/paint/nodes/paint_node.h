#ifndef PAINT_NODE_H
#define PAINT_NODE_H

#include "core/math/vector2i.h"

#include "scene/main/node_2d.h"

class PaintProject;

class PaintNode : public Node2D {
	GDCLASS(PaintNode, Node2D);

public:
	enum {
		NOTIFICATION_PARENT_PAINT_NODE_RESIZED = 2500,
		NOTIFICATION_PAINT_PROJECT_PRE_RENDER = 2501,
		NOTIFICATION_PAINT_PROJECT_POST_RENDER = 2502,
	};

	Vector2i get_size();
	void set_size(const Vector2i &size);

	bool get_draw_outline();
	void set_draw_outline(const bool val);

	Ref<Image> render_image();
	virtual Ref<Image> _render_image();

	Ref<Image> get_rendered_image();
	virtual Ref<Image> _get_rendered_image();

	bool render_should_evaluate_children();
	virtual bool _render_should_evaluate_children();

	void render_evaluate_paint_node(PaintNode *node, Transform2D transform, Ref<Image> image);
	void render_paint_node(PaintNode *node, Transform2D transform, Ref<Image> image);

	PoolVector2iArray util_get_pixels_in_line(const Vector2i &from, const Vector2i &to);

	int util_to_1d_v(const Vector2i &p, int w);
	int util_to_1d(int x, int y, int w);
	Vector2i util_to_2d(int idx, int w);

	Color util_color_from_array(const PoolRealArray &color_array);
	Color util_random_color();
	Color util_random_color_alt();

	bool forward_canvas_gui_input(const Ref<InputEvent> &p_event);
	virtual bool _forward_canvas_gui_input(const Ref<InputEvent> &p_event);

	PaintProject *get_paint_project();
	PaintNode *get_parent_paint_node();
	PaintNode *find_parent_paint_node();

	String get_configuration_warning() const;

	void _propagate_notification_resized();
	void _propagate_notification_project_pre_render();
	void _propagate_notification_project_post_render();

	PaintNode();
	~PaintNode();

protected:
	void _notification(int p_what);

	static void _bind_methods();

	Vector2i _size;
	bool _draw_outline;
};

#endif
