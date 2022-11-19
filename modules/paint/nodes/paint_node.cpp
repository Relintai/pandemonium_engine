#include "paint_node.h"

#include "../paint_utilities.h"
#include "paint_project.h"

Vector2i PaintNode::get_size() {
	return _size;
}
void PaintNode::set_size(const Vector2i &size) {
	_size = size;

	if (is_inside_tree()) {
		_propagate_notification_resized();
		update();
	}
}

bool PaintNode::get_draw_outline() {
	return _draw_outline;
}
void PaintNode::set_draw_outline(const bool val) {
	_draw_outline = val;

	if (is_inside_tree()) {
		update();
	}
}

void PaintNode::save_image() {
	_propagate_notification_project_pre_save();
	call("_save_image");
	_propagate_notification_project_post_save();
}
void PaintNode::_save_image() {
}

Ref<Image> PaintNode::get_save_image() {
	return call("_get_save_image");
}
Ref<Image> PaintNode::_get_save_image() {
	return Ref<Image>();
}

PoolVector2iArray PaintNode::util_get_pixels_in_line(const Vector2i &from, const Vector2i &to) {
	return PaintUtilities::get_pixels_in_line(from, to);
}

int PaintNode::util_to_1d_v(const Vector2i &p, int w) {
	return PaintUtilities::to_1D_v(p, w);
}
int PaintNode::util_to_1d(int x, int y, int w) {
	return PaintUtilities::to_1D(x, y, w);
}
Vector2i PaintNode::util_to_2d(int idx, int w) {
	return PaintUtilities::to_2D(idx, w);
}

Color PaintNode::util_color_from_array(const PoolRealArray &color_array) {
	return PaintUtilities::color_from_array(color_array);
}
Color PaintNode::util_random_color() {
	return PaintUtilities::random_color();
}
Color PaintNode::util_random_color_alt() {
	return PaintUtilities::random_color_alt();
}

bool PaintNode::forward_canvas_gui_input(const Ref<InputEvent> &p_event) {
	return call("_forward_canvas_gui_input", p_event);
}
bool PaintNode::_forward_canvas_gui_input(const Ref<InputEvent> &p_event) {
	return false;
}

PaintProject *PaintNode::get_paint_project() {
	PaintNode *p = this;

	while (p) {
		PaintProject *pp = Object::cast_to<PaintProject>(p);

		if (pp) {
			return pp;
		}

		p = Object::cast_to<PaintNode>(p->get_parent());
	}

	return NULL;
}

String PaintNode::get_configuration_warning() const {
	const PaintNode *p = this;

	while (p) {
		const PaintProject *pp = Object::cast_to<PaintProject>(p);

		if (pp) {
			return "";
		}

		p = Object::cast_to<PaintNode>(p->get_parent());
	}

	return "This Node should be a child of a PaintProject!";
}

void PaintNode::_propagate_notification_resized() {
	notification(NOTIFICATION_PAINT_NODE_RESIZED);

	for (int i = 0; i < get_child_count(); ++i) {
		PaintNode *pn = Object::cast_to<PaintNode>(get_child(i));

		if (pn) {
			pn->_propagate_notification_resized();
		}
	}
}

void PaintNode::_propagate_notification_project_pre_save() {
	notification(NOTIFICATION_PAINT_PROJECT_PRE_SAVE);

	for (int i = 0; i < get_child_count(); ++i) {
		PaintNode *pn = Object::cast_to<PaintNode>(get_child(i));

		if (pn) {
			pn->_propagate_notification_project_pre_save();
		}
	}
}
void PaintNode::_propagate_notification_project_post_save() {
	notification(NOTIFICATION_PAINT_PROJECT_POST_SAVE);

	for (int i = 0; i < get_child_count(); ++i) {
		PaintNode *pn = Object::cast_to<PaintNode>(get_child(i));

		if (pn) {
			pn->_propagate_notification_project_post_save();
		}
	}
}

PaintNode::PaintNode() {
	_draw_outline = true;
}

PaintNode::~PaintNode() {
}

void PaintNode::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_DRAW: {
			if (_draw_outline) {
				draw_line(Point2(0, 0), Point2(_size.x, 0), Color(0, 0, 0, 1));
				draw_line(Point2(0, _size.y), Point2(_size.x, _size.y), Color(0, 0, 0, 1));
				draw_line(Point2(0, 0), Point2(0, _size.y), Color(0, 0, 0, 1));
				draw_line(Point2(_size.x, 0), Point2(_size.x, _size.y), Color(0, 0, 0, 1));
			}
		} break;
		default:
			break;
	}
}

void PaintNode::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_size"), &PaintNode::get_size);
	ClassDB::bind_method(D_METHOD("set_size", "size"), &PaintNode::set_size);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "size"), "set_size", "get_size");

	ClassDB::bind_method(D_METHOD("get_draw_outline"), &PaintNode::get_draw_outline);
	ClassDB::bind_method(D_METHOD("set_draw_outline", "val"), &PaintNode::set_draw_outline);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "draw_outline"), "set_draw_outline", "get_draw_outline");

	BIND_VMETHOD(MethodInfo("_save_image"));
	ClassDB::bind_method(D_METHOD("save_image"), &PaintNode::save_image);
	ClassDB::bind_method(D_METHOD("_save_image"), &PaintNode::_save_image);

	BIND_VMETHOD(MethodInfo("_get_save_image"));
	ClassDB::bind_method(D_METHOD("get_save_image"), &PaintNode::get_save_image);
	ClassDB::bind_method(D_METHOD("_get_save_image"), &PaintNode::_get_save_image);

	ClassDB::bind_method(D_METHOD("util_get_pixels_in_line", "from", "to"), &PaintNode::util_get_pixels_in_line);

	ClassDB::bind_method(D_METHOD("util_to_1d_v", "p", "w"), &PaintNode::util_to_1d_v);
	ClassDB::bind_method(D_METHOD("util_to_1d", "x", "y", "w"), &PaintNode::util_to_1d);
	ClassDB::bind_method(D_METHOD("util_to_2d", "idx", "w"), &PaintNode::util_to_2d);

	ClassDB::bind_method(D_METHOD("util_color_from_array", "color_array"), &PaintNode::util_color_from_array);
	ClassDB::bind_method(D_METHOD("util_random_color"), &PaintNode::util_random_color);
	ClassDB::bind_method(D_METHOD("util_random_color_alt"), &PaintNode::util_random_color_alt);

	BIND_VMETHOD(MethodInfo(Variant::BOOL, "_forward_canvas_gui_input", PropertyInfo(Variant::OBJECT, "event", PROPERTY_HINT_RESOURCE_TYPE, "InputEvent")));
	ClassDB::bind_method(D_METHOD("forward_canvas_gui_input", "event"), &PaintNode::forward_canvas_gui_input);
	ClassDB::bind_method(D_METHOD("_forward_canvas_gui_input", "event"), &PaintNode::_forward_canvas_gui_input);

	ClassDB::bind_method(D_METHOD("get_paint_project"), &PaintNode::get_paint_project);

	BIND_CONSTANT(NOTIFICATION_PAINT_NODE_RESIZED);
	BIND_CONSTANT(NOTIFICATION_PAINT_PROJECT_PRE_SAVE);
	BIND_CONSTANT(NOTIFICATION_PAINT_PROJECT_POST_SAVE);
}
