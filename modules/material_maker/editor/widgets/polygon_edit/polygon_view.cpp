
#include "polygon_view.h"

#include "../../../nodes/bases/polygon_base.h"

Ref<PolygonBase> PolygonView::get_polygon() {
	return polygon;
}

void PolygonView::set_polygon(const Ref<PolygonBase> &val) {
	polygon = val;
	update();
}

Vector2 PolygonView::get_draw_size() {
	return draw_size;
}

void PolygonView::set_draw_size(const Vector2 &val) {
	draw_size = val;
}

Vector2 PolygonView::get_draw_offset() {
	return draw_offset;
}

void PolygonView::set_draw_offset(const Vector2 &val) {
	draw_offset = val;
}

bool PolygonView::get_closed() const {
	return closed;
}

void PolygonView::set_closed(const bool c) {
	closed = c;
	update();
}

Vector2 PolygonView::transform_point(const Vector2 &p) {
	return draw_offset + p * draw_size;
}

Vector2 PolygonView::reverse_transform_point(const Vector2 &p) {
	return (p - draw_offset) / draw_size;
}

void PolygonView::_draw() {
	if (!polygon.is_valid()) {
		return;
	}

	//	var current_theme : Theme = get_node("/root/MainWindow").theme;
	//	var bg = current_theme.get_stylebox("panel", "Panel").bg_color;
	//	var fg = current_theme.get_color("font_color", "Label");
	//	var axes_color : Color = bg.linear_interpolate(fg, 0.25);
	//	var curve_color : Color = bg.linear_interpolate(fg, 0.75);
	Color axes_color = Color(0.9, 0.9, 0.9, 1);
	Color curve_color = Color(1, 1, 1, 1);
	draw_rect(Rect2(draw_offset, draw_size), axes_color, false);

	PoolVector2Array points = polygon->get_points();

	int index = 0;

	if (closed) {
		index = points.size() - 1;
	}

	Vector2 tp = transform_point(points[index]);

	for (int i = 0; i < points.size(); ++i) {
		Vector2 p = points[i];
		Vector2 tnp = transform_point(p);
		draw_line(tp, tnp, curve_color);
		tp = tnp;
	}
}

void PolygonView::_on_resize() {
	float ds = MIN(get_size().x, get_size().y);
	draw_size = Vector2(ds, ds);
	draw_offset = 0.5 * (get_size() - draw_size);
	update();
}

PolygonView::PolygonView() {
	draw_size = Vector2(1, 1);
	draw_offset = Vector2(0, 0);
	closed = true;

	set_size(Vector2(64, 64));
	set_mouse_filter(MOUSE_FILTER_IGNORE);
}

PolygonView::~PolygonView() {
}

void PolygonView::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_POSTINITIALIZE: {
			//	polygon = MMPolygon.new();
			connect("resized", this, "_on_resize");
			_on_resize();
		} break;
		case NOTIFICATION_DRAW: {
			_draw();
		} break;
		default: {
		} break;
	}
}

void PolygonView::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_polygon"), &PolygonView::get_polygon);
	ClassDB::bind_method(D_METHOD("set_polygon", "value"), &PolygonView::set_polygon);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "polygon", PROPERTY_HINT_RESOURCE_TYPE, "PolygonBase"), "set_polygon", "get_polygon");

	ClassDB::bind_method(D_METHOD("get_draw_size"), &PolygonView::get_draw_size);
	ClassDB::bind_method(D_METHOD("set_draw_size", "value"), &PolygonView::set_draw_size);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "draw_size"), "set_draw_size", "get_draw_size");

	ClassDB::bind_method(D_METHOD("get_draw_offset"), &PolygonView::get_draw_offset);
	ClassDB::bind_method(D_METHOD("set_draw_offset", "value"), &PolygonView::set_draw_offset);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "draw_offset"), "set_draw_offset", "get_draw_offset");

	ClassDB::bind_method(D_METHOD("get_closed"), &PolygonView::get_closed);
	//	ClassDB::bind_method(D_METHOD("set_closed", "value"), &PolygonView::set_closed);
	ClassDB::bind_method(D_METHOD("set_closed", "c"), &PolygonView::set_closed, true);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "closed"), "set_closed", "get_closed");

	ClassDB::bind_method(D_METHOD("transform_point", "p"), &PolygonView::transform_point);
	ClassDB::bind_method(D_METHOD("reverse_transform_point", "p"), &PolygonView::reverse_transform_point);

	//ClassDB::bind_method(D_METHOD("_draw"), &PolygonView::_draw);
	ClassDB::bind_method(D_METHOD("_on_resize"), &PolygonView::_on_resize);
}
