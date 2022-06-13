#ifndef POLYGON_VIEW_H
#define POLYGON_VIEW_H

#include "core/reference.h"

#include "scene/gui/control.h"

class PolygonBase;

class PolygonView : public Control {
	GDCLASS(PolygonView, Control);

public:
	Ref<PolygonBase> get_polygon();
	void set_polygon(const Ref<PolygonBase> &val);

	Vector2 get_draw_size();
	void set_draw_size(const Vector2 &val);

	Vector2 get_draw_offset();
	void set_draw_offset(const Vector2 &val);

	bool get_closed() const;
	void set_closed(const bool c = true);

	Vector2 transform_point(const Vector2 &p);
	Vector2 reverse_transform_point(const Vector2 &p);

	void _draw();
	void _on_resize();

	PolygonView();
	~PolygonView();

protected:
	void _notification(int p_what);
	static void _bind_methods();

	Ref<PolygonBase> polygon;
	Vector2 draw_size = Vector2(1, 1);
	Vector2 draw_offset = Vector2(0, 0);
	bool closed = true;
};

#endif
