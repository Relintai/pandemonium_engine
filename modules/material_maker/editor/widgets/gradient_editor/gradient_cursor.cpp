#include "gradient_cursor.h"

#include "gradient_editor.h"
#include "scene/gui/label.h"

Color GradientCursor::get_color() {
	return color;
}

void GradientCursor::set_color(const Color &val) {
	color = val;
	GradientEditor *ged = Object::cast_to<GradientEditor>(get_parent());
	ERR_FAIL_COND(!ged);
	ged->update_value();
	update();
}

bool GradientCursor::get_sliding() const {
	return sliding;
}

void GradientCursor::set_sliding(const bool val) {
	sliding = val;
}

void GradientCursor::set_label(Label *p_label) {
	label = p_label;
}

void GradientCursor::_draw() {
	Vector<Vector2> polygon;
	polygon.push_back(Vector2(0, 5));
	polygon.push_back(Vector2(WIDTH / 2, 0));
	polygon.push_back(Vector2(WIDTH, 5));
	polygon.push_back(Vector2(WIDTH, 15));
	polygon.push_back(Vector2(0, 15));
	polygon.push_back(Vector2(0, 5));

	Color c = color;
	c.a = 1.0;
	draw_colored_polygon(polygon, c);
	draw_polyline(polygon, color.get_v() > 0.5 ? Color(0.0, 0.0, 0.0) : Color(1.0, 1.0, 1.0));
}

void GradientCursor::_gui_input(const Ref<InputEvent> &ev) {
	Ref<InputEventMouseButton> iemb = ev;
	Ref<InputEventMouseMotion> iemm = ev;

	GradientEditor *ged = Object::cast_to<GradientEditor>(get_parent());
	ERR_FAIL_COND(!ged);

	if (iemb.is_valid()) {
		if (iemb->get_button_index() == BUTTON_LEFT) {
			if (iemb->is_doubleclick()) {
				ged->save_color_state();
				ged->select_color(this, iemb->get_global_position());
			} else if (iemb->is_pressed()) {
				ged->save_color_state();
				sliding = true;
				label->set_visible(true);
				label->set_text(String::num(get_cursor_position(), 3));
			} else {
				if (sliding) {
					ged->undo_redo_save_color_state();
				}

				sliding = false;
				label->set_visible(false);
			}
		} else if (iemb->get_button_index() == BUTTON_RIGHT && ged->get_sorted_cursors().size() > 2) {
			ged->save_color_state();
			ged->remove_child(this);
			ged->update_value();
			ged->undo_redo_save_color_state();
			queue_delete();
		}
	} else if (iemm.is_valid() && (iemm->get_button_mask() & BUTTON_MASK_LEFT) != 0 && sliding) {
		Vector2 position = get_position();

		position.x += get_local_mouse_position().x;

		if (iemm->get_control()) {
			position.x = Math::round(get_cursor_position() * 20.0) * 0.05 * (ged->get_size().x - WIDTH);
		}

		position.x = MIN(MAX(0, position.x), ged->get_size().x - get_size().x);
		set_position(position);

		ged->update_value();
		label->set_text(String::num(get_cursor_position(), 3));
	}
}

float GradientCursor::get_cursor_position() {
	return get_position().x / (get_parent_control()->get_size().x - WIDTH);
}

bool GradientCursor::sort(const Variant &a, const Variant &b) {
	return a.get_position() < b.get_position();
}

bool GradientCursor::can_drop_data_fw(const Point2 &p_point, const Variant &p_data, Control *p_from) const {
	return p_data.get_type() == Variant::COLOR;
}

void GradientCursor::drop_data_fw(const Point2 &p_point, const Variant &p_data, Control *p_from) {
	set_color(p_data);
}

GradientCursor::GradientCursor() {
	sliding = false;
	label = nullptr;
}

GradientCursor::~GradientCursor() {
}

void GradientCursor::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_POSTINITIALIZE: {
			set_position(Vector2(0, 15));
			set_size(Vector2(WIDTH, 15));
		} break;
		case NOTIFICATION_DRAW: {
			_draw();
		} break;
	}
}

void GradientCursor::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_color"), &GradientCursor::get_color);
	ClassDB::bind_method(D_METHOD("set_color", "value"), &GradientCursor::set_color);
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "color"), "set_color", "get_color");

	ClassDB::bind_method(D_METHOD("get_sliding"), &GradientCursor::get_sliding);
	ClassDB::bind_method(D_METHOD("set_sliding", "value"), &GradientCursor::set_sliding);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "sliding"), "set_sliding", "get_sliding");

	ClassDB::bind_method(D_METHOD("_gui_input", "ev"), &GradientCursor::_gui_input);

	ClassDB::bind_method(D_METHOD("get_cursor_position"), &GradientCursor::get_cursor_position);
	ClassDB::bind_method(D_METHOD("set_color", "c"), &GradientCursor::set_color);

	ClassDB::bind_method(D_METHOD("can_drop_data_fw", "point", "data", "from"), &GradientCursor::can_drop_data_fw);
	ClassDB::bind_method(D_METHOD("drop_data_fw", "point", "data", "from"), &GradientCursor::drop_data_fw);
}
