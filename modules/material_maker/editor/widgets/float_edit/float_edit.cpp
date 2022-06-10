
#include "float_edit.h"

#include "scene/gui/color_rect.h"

float FloatEdit::get_value() const {
	return value;
}

void FloatEdit::set_value_float(const float val) {
	value = val;
	do_update();
	slider->set_visible(true);
}

void FloatEdit::set_value(const Variant &v) {
	if (v.get_type() == Variant::REAL) {
		value = v;
		do_update();
		slider->set_visible(true);
	} else if (v.get_type() == Variant::STRING && !float_only) {
		set_text(v);
		slider->set_visible(false);
	}
}

float FloatEdit::get_min_value() const {
	return min_value;
}

void FloatEdit::set_min_value(const float v) {
	min_value = v;
	do_update();
}

float FloatEdit::get_max_value() const {
	return max_value;
}

void FloatEdit::set_max_value(const float v) {
	max_value = v;
	do_update();
}

float FloatEdit::get_step() const {
	return step;
}

void FloatEdit::set_step(const float v) {
	step = v;
	do_update();
}

bool FloatEdit::get_float_only() const {
	return float_only;
}

void FloatEdit::set_float_only(const bool val) {
	float_only = val;
}

bool FloatEdit::get_sliding() const {
	return sliding;
}

void FloatEdit::set_sliding(const bool val) {
	sliding = val;
}

float FloatEdit::get_start_position() const {
	return start_position;
}

void FloatEdit::set_start_position(const float val) {
	start_position = val;
}

float FloatEdit::get_last_position() const {
	return last_position;
}

void FloatEdit::set_last_position(const float val) {
	last_position = val;
}

float FloatEdit::get_start_value() const {
	return start_value;
}

void FloatEdit::set_start_value(const float val) {
	start_value = val;
}

bool FloatEdit::get_from_lower_bound() const {
	return from_lower_bound;
}

void FloatEdit::set_from_lower_bound(const bool val) {
	from_lower_bound = val;
}

bool FloatEdit::get_from_upper_bound() const {
	return from_upper_bound;
}

void FloatEdit::set_from_upper_bound(const bool val) {
	from_upper_bound = val;
}

void FloatEdit::do_update(const bool update_text) {
	if (update_text && slider->is_visible()) {
		set_text(String::num(value));

		Point2 pos = cursor->get_position();

		if (max_value != min_value) {
			pos.x = (CLAMP(value, min_value, max_value) - min_value) * (slider->get_size().x - cursor->get_size().x) / (max_value - min_value);
		} else {
			pos.x = 0;
		}

		cursor->set_position(pos);
	}
}

int FloatEdit::get_modifiers(const Ref<InputEventMouseButton> &event) {
	int new_modifiers = 0;

	if (event->get_shift()) {
		new_modifiers |= 1;
	}

	if (event->get_control()) {
		new_modifiers |= 2;
	}

	if (event->get_alt()) {
		new_modifiers |= 4;
	}

	return new_modifiers;
}

void FloatEdit::_on_LineEdit_gui_input(const Ref<InputEvent> &event) {
	if (!slider->is_visible() || !is_editable()) {
		return;
	}

	Ref<InputEventMouseButton> iemb = event;
	Ref<InputEventMouseMotion> iemm = event;
	Ref<InputEventKey> iek = event;

	if (iemb.is_valid() && iemb->get_button_index() == BUTTON_LEFT) {
		if (iemb->is_pressed()) {
			last_position = iemb->get_position().x;
			start_position = last_position;
			start_value = value;
			sliding = true;
			from_lower_bound = value <= min_value;
			from_upper_bound = value >= max_value;
			modifiers = get_modifiers(event);
		} else {
			sliding = false;
		}
	} else if (sliding && iemm.is_valid() && iemm->get_button_mask() == BUTTON_MASK_LEFT) {
		int new_modifiers = get_modifiers(event);

		if (new_modifiers != modifiers) {
			start_position = last_position;
			start_value = value;
			modifiers = new_modifiers;
		} else {
			last_position = iemm->get_position().x;
			float delta = last_position - start_position;
			int current_step = step;

			if (iemm->get_control()) {
				delta *= 0.2;
			} else if (iemm->get_shift()) {
				delta *= 5.0;
			}

			if (iemm->get_alt()) {
				current_step *= 0.01;
			}

			float v = start_value + SGN(delta) * pow(abs(delta) * 0.005, 2) * abs(max_value - min_value);

			if (current_step != 0) {
				v = min_value + floor((v - min_value) / current_step) * current_step;
			}

			if (!from_lower_bound && v < min_value) {
				v = min_value;
			}

			if (!from_upper_bound && v > max_value) {
				v = max_value;
			}

			set_value(v);
			select(0, 0);
			emit_signal("value_changed", value);
			release_focus();
		}
	} else if (iek.is_valid() && !event->is_echo()) {
		start_position = last_position;
		start_value = value;
		modifiers = get_modifiers(event);
	}
}

void FloatEdit::_on_LineEdit_text_changed(const String &new_text) {
	if (new_text.is_valid_float()) {
		value = new_text.to_float();
		do_update(false);
	}
}

void FloatEdit::_on_LineEdit_text_entered(const String &new_text, const Variant &release) {
	if (new_text.is_valid_float()) {
		value = new_text.to_float();
		do_update();
		emit_signal("value_changed", value);
		slider->set_visible(true);
	} else if (float_only) {
		do_update();
		emit_signal("value_changed", value);
		slider->set_visible(true);
	} else {
		emit_signal("value_changed", new_text);
		slider->set_visible(false);
	}

	if (release) {
		release_focus();
	}
}

void FloatEdit::_on_FloatEdit_focus_entered() {
	select_all();
}

void FloatEdit::_on_LineEdit_focus_exited() {
	_on_LineEdit_text_entered(get_text(), false);
}

FloatEdit::FloatEdit() {
	value = 0.5;
	min_value = 0.0;
	max_value = 1.0;
	step = 0.0;
	float_only = false;
	sliding = false;
	start_position = 0;
	last_position = 0;
	start_value = 0;
	modifiers = 0;
	from_lower_bound = false;
	from_upper_bound = false;

	set_focus_mode(FOCUS_CLICK);
	set_text("0.5");
	set_max_length(100);
	set_context_menu_enabled(false);
	cursor_set_blink_enabled(true);

	slider = memnew(ColorRect);
	slider->set_name("Slider");
	add_child(slider);
	slider->set_custom_minimum_size(Vector2(0, 2));
	slider->set_mouse_filter(MOUSE_FILTER_IGNORE);
	slider->set_frame_color(Color(0.501961, 0.501961, 0.501961, 1));

	cursor = memnew(ColorRect);
	cursor->set_name("Cursor");
	slider->add_child(cursor);
	cursor->set_custom_minimum_size(Vector2(3, 2));
	cursor->set_mouse_filter(MOUSE_FILTER_IGNORE);
}

FloatEdit::~FloatEdit() {
}

void FloatEdit::_notification(int p_what) {
	if (p_what == NOTIFICATION_POSTINITIALIZE) {
		do_update();
	}
}

void FloatEdit::_bind_methods() {
	ADD_SIGNAL(MethodInfo("value_changed", PropertyInfo(Variant::INT, "value")));

	ClassDB::bind_method(D_METHOD("get_value"), &FloatEdit::get_value);
	ClassDB::bind_method(D_METHOD("set_value_float", "value"), &FloatEdit::set_value_float);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "value"), "set_value", "get_value");

	ClassDB::bind_method(D_METHOD("get_min_value"), &FloatEdit::get_min_value);
	ClassDB::bind_method(D_METHOD("set_min_value", "value"), &FloatEdit::set_min_value);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "min_value"), "set_min_value", "get_min_value");

	ClassDB::bind_method(D_METHOD("get_max_value"), &FloatEdit::get_max_value);
	ClassDB::bind_method(D_METHOD("set_max_value", "value"), &FloatEdit::set_max_value);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "max_value"), "set_max_value", "get_max_value");

	ClassDB::bind_method(D_METHOD("get_step"), &FloatEdit::get_step);
	ClassDB::bind_method(D_METHOD("set_step", "value"), &FloatEdit::set_step);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "step"), "set_step", "get_step");

	ClassDB::bind_method(D_METHOD("get_float_only"), &FloatEdit::get_float_only);
	ClassDB::bind_method(D_METHOD("set_float_only", "value"), &FloatEdit::set_float_only);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "float_only"), "set_float_only", "get_float_only");

	ClassDB::bind_method(D_METHOD("get_sliding"), &FloatEdit::get_sliding);
	ClassDB::bind_method(D_METHOD("set_sliding", "value"), &FloatEdit::set_sliding);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "sliding"), "set_sliding", "get_sliding");

	ClassDB::bind_method(D_METHOD("get_start_position"), &FloatEdit::get_start_position);
	ClassDB::bind_method(D_METHOD("set_start_position", "value"), &FloatEdit::set_start_position);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "start_position"), "set_start_position", "get_start_position");

	ClassDB::bind_method(D_METHOD("get_last_position"), &FloatEdit::get_last_position);
	ClassDB::bind_method(D_METHOD("set_last_position", "value"), &FloatEdit::set_last_position);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "last_position"), "set_last_position", "get_last_position");

	ClassDB::bind_method(D_METHOD("get_start_value"), &FloatEdit::get_start_value);
	ClassDB::bind_method(D_METHOD("set_start_value", "value"), &FloatEdit::set_start_value);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "start_value"), "set_start_value", "get_start_value");

	ClassDB::bind_method(D_METHOD("get_from_lower_bound"), &FloatEdit::get_from_lower_bound);
	ClassDB::bind_method(D_METHOD("set_from_lower_bound", "value"), &FloatEdit::set_from_lower_bound);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "from_lower_bound"), "set_from_lower_bound", "get_from_lower_bound");

	ClassDB::bind_method(D_METHOD("get_from_upper_bound"), &FloatEdit::get_from_upper_bound);
	ClassDB::bind_method(D_METHOD("set_from_upper_bound", "value"), &FloatEdit::set_from_upper_bound);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "from_upper_bound"), "set_from_upper_bound", "get_from_upper_bound");

	ClassDB::bind_method(D_METHOD("do_update", "update_text"), &FloatEdit::do_update, true);
	ClassDB::bind_method(D_METHOD("get_modifiers", "event"), &FloatEdit::get_modifiers);

	ClassDB::bind_method(D_METHOD("_on_LineEdit_gui_input", "event"), &FloatEdit::_on_LineEdit_gui_input);
	ClassDB::bind_method(D_METHOD("_on_LineEdit_text_changed", "new_text"), &FloatEdit::_on_LineEdit_text_changed);
	ClassDB::bind_method(D_METHOD("_on_LineEdit_text_entered", "new_text", "release"), &FloatEdit::_on_LineEdit_text_entered, true);
	ClassDB::bind_method(D_METHOD("_on_FloatEdit_focus_entered"), &FloatEdit::_on_FloatEdit_focus_entered);
	ClassDB::bind_method(D_METHOD("_on_LineEdit_focus_exited"), &FloatEdit::_on_LineEdit_focus_exited);
}
