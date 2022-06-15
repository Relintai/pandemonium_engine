
#include "curve_view.h"

#include "../../../nodes/bases/curve_base.h"
#include "core/vector.h"

bool CurveView::get_show_axes() const {
	return show_axes;
}

void CurveView::set_show_axes(const bool val) {
	show_axes = val;
}

Ref<CurveBase> CurveView::get_curve() {
	return curve;
}

void CurveView::set_curve(const Ref<CurveBase> &val) {
	curve = val;
	update();
}

Vector2 CurveView::transform_point(const Vector2 &p) {
	return (Vector2(0.0, 1.0) + Vector2(1.0, -1.0) * p) * get_size();
}

Vector2 CurveView::reverse_transform_point(const Vector2 &p) {
	return Vector2(0.0, 1.0) + Vector2(1.0, -1.0) * p / get_size();
}

void CurveView::_draw() {
	if (!curve.is_valid()) {
		return;
	}

	//	var current_theme : Theme = get_node("/root/MainWindow").theme;
	//;
	//	var bg = current_theme.get_stylebox("panel", "Panel").bg_color;
	//	var fg = current_theme.get_color("font_color", "Label");
	//;
	//	var axes_color : Color = bg.linear_interpolate(fg, 0.25);
	//	var curve_color : Color = bg.linear_interpolate(fg, 0.75);
	Color axes_color = Color(0.9, 0.9, 0.9, 1);
	Color curve_color = Color(1, 1, 1, 1);

	if (show_axes) {
		for (int i = 0; i < 5; ++i) { //i in range(5)
			Vector2 p = transform_point(0.25 * Vector2(i, i));
			draw_line(Vector2(p.x, 0), Vector2(p.x, get_size().y - 1), axes_color);
			draw_line(Vector2(0, p.y), Vector2(get_size().x - 1, p.y), axes_color);
		}
	}

	Vector<CurveBase::Point> points = curve->get_points();

	for (int i = 0; i < points.size() - 1; ++i) {
		Vector2 p1 = points[i].p;
		Vector2 p2 = points[i + 1].p;
		float d = (p2.x - p1.x) / 3.0;
		float yac = p1.y + d * points[i].rs;
		float ybc = p2.y - d * points[i + 1].ls;
		Vector2 p = transform_point(p1);
		int count = MAX(1, static_cast<int>((transform_point(p2).x - p.x / 5.0)));

		for (int tt = 0; tt < count; ++tt) {
			float t = (tt + 1.0) / count;
			float omt = (1.0 - t);
			float omt2 = omt * omt;
			float omt3 = omt2 * omt;
			float t2 = t * t;
			float t3 = t2 * t;
			float x = p1.x + (p2.x - p1.x) * t;
			Vector2 np = transform_point(Vector2(x, p1.y * omt3 + yac * omt2 * t * 3.0 + ybc * omt * t2 * 3.0 + p2.y * t3));
			draw_line(p, np, curve_color);
			p = np;
		}
	}
}

void CurveView::_on_resize() {
	update();
}

CurveView::CurveView() {
	show_axes = false;

	set_anchors_and_margins_preset(PRESET_WIDE, PRESET_MODE_MINSIZE, 4);

	set_mouse_filter(MOUSE_FILTER_IGNORE);
}

CurveView::~CurveView() {
}

void CurveView::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_POSTINITIALIZE: {
			connect("resized", this, "_on_resize");
			update();
		} break;
		case NOTIFICATION_DRAW: {
			_draw();
		} break;
		default: {
		} break;
	}
}

void CurveView::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_show_axes"), &CurveView::get_show_axes);
	ClassDB::bind_method(D_METHOD("set_show_axes", "value"), &CurveView::set_show_axes);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "show_axes"), "set_show_axes", "get_show_axes");

	ClassDB::bind_method(D_METHOD("get_curve"), &CurveView::get_curve);
	ClassDB::bind_method(D_METHOD("set_curve", "value"), &CurveView::set_curve);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "curve", PROPERTY_HINT_RESOURCE_TYPE, "CurveBase"), "set_curve", "get_curve");

	ClassDB::bind_method(D_METHOD("_on_resize"), &CurveView::_on_resize);

	ClassDB::bind_method(D_METHOD("transform_point", "p"), &CurveView::transform_point);
	ClassDB::bind_method(D_METHOD("reverse_transform_point", "p"), &CurveView::reverse_transform_point);
}
