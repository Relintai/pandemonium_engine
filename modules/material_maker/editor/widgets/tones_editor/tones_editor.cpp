
#include "tones_editor.h"

#include "scene/resources/texture.h"

#include "../../../nodes/filter/tones.h"

#include "scene/gui/button.h"
#include "scene/gui/option_button.h"
#include "scene/gui/texture_rect.h"

#include "../../../algos/mm_algos.h"

#include "tones_editor_cursor.h"

void MMTonesEditor::set_value(const Ref<MMTones> &v) {
	if (_node == v) {
		return;
	}

	if (_node.is_valid()) {
		_node->get_image()->disconnect("changed", this, "on_input_property_changed");
	}

	_node = v;

	if (_node.is_valid()) {
		_node->get_image()->connect("changed", this, "on_input_property_changed");

		set_mode(static_cast<Modes>(_node->get_current_mode()));
	}

	on_input_property_changed();
}

enum ParameterTypes {
	PARAMETER_TYPE_IN_MIN = 0,
	PARAMETER_TYPE_IN_MID,
	PARAMETER_TYPE_IN_MAX,
	PARAMETER_TYPE_OUT_MIN,
	PARAMETER_TYPE_OUT_MAX,
};

void MMTonesEditor::set_mode(Modes mode) {
	_current_mode = mode;

	_mode_ob->select(static_cast<int>(mode));
	_cursor_in_min->set_value(get_parameter_current_mode(PARAMETER_TYPE_IN_MIN));
	_cursor_in_mid->set_value(get_parameter_current_mode(PARAMETER_TYPE_IN_MID));
	_cursor_in_max->set_value(get_parameter_current_mode(PARAMETER_TYPE_IN_MAX));
	_cursor_out_min->set_value(get_parameter_current_mode(PARAMETER_TYPE_OUT_MIN));
	_cursor_out_max->set_value(get_parameter_current_mode(PARAMETER_TYPE_OUT_MAX));
}

Color MMTonesEditor::get_parameter(ParameterTypes type) {
	if (!_node.is_valid()) {
		return Color();
	}

	switch (type) {
		case PARAMETER_TYPE_IN_MIN:
			return _node->get_in_min();
			break;
		case PARAMETER_TYPE_IN_MID:
			return _node->get_in_mid();
			break;
		case PARAMETER_TYPE_IN_MAX:
			return _node->get_in_max();
			break;
		case PARAMETER_TYPE_OUT_MIN:
			return _node->get_out_min();
			break;
		case PARAMETER_TYPE_OUT_MAX:
			return _node->get_out_max();
			break;
		default:
			break;
	}

	return Color();
}

float MMTonesEditor::get_parameter_current_mode(ParameterTypes type) {
	Color color = get_parameter(type);

	switch (_current_mode) {
		case MODE_LUMINANCE:
			return (color.r + color.g + color.b) / 3.0;
			break;
		case MODE_RED:
			return color.r;
			break;
		case MODE_GREEN:
			return color.g;
			break;
		case MODE_BLUE:
			return color.b;
			break;
		case MODE_ALPHA:
			return color.a;
			break;
	}

	return 0;
}

void MMTonesEditor::set_parameter(ParameterTypes type, const Color &val) {
	if (!_node.is_valid()) {
		return;
	}

	switch (type) {
		case PARAMETER_TYPE_IN_MIN:
			_node->set_in_min(val);
			break;
		case PARAMETER_TYPE_IN_MID:
			_node->set_in_mid(val);
			break;
		case PARAMETER_TYPE_IN_MAX:
			_node->set_in_max(val);
			break;
		case PARAMETER_TYPE_OUT_MIN:
			_node->set_out_min(val);
			break;
		case PARAMETER_TYPE_OUT_MAX:
			_node->set_out_max(val);
			break;
		default:
			break;
	}
}

void MMTonesEditor::set_parameter_current_mode(ParameterTypes type, float val, float d) {
	if (!_node.is_valid()) {
		return;
	}

	Color color = get_parameter(type);

	switch (_current_mode) {
		case MODE_LUMINANCE:
			color.r = val;
			color.g = val;
			color.b = val;
			color.a = d;
			break;
		case MODE_RED:
			color.r = val;
			break;
		case MODE_GREEN:
			color.g = val;
			break;
		case MODE_BLUE:
			color.b = val;
			break;
		case MODE_ALPHA:
			color.a = val;
			break;
	}

	set_parameter(type, color);
}

Ref<ImageTexture> MMTonesEditor::make_histogram(const Ref<Image> &img) {
	ERR_FAIL_COND_V(!img.is_valid(), Ref<ImageTexture>());

	Ref<Image> histogram = MMAlgos::generate_histogram(img, HISTOGRAM_IMAGE_SIZE);

	Ref<ImageTexture> tex;
	tex.instance();
	tex->create_from_image(histogram, 0);

	return tex;
}

Ref<ImageTexture> MMTonesEditor::make_default_histogram() {
	Ref<Image> img;
	img.instance();
	img->create(HISTOGRAM_IMAGE_SIZE, HISTOGRAM_IMAGE_SIZE, false, Image::FORMAT_RGBA8);

	img->fill(Color(1, 1, 1, 1));

	return make_histogram(img);
}

MMTonesEditor::MMTonesEditor() {
	HBoxContainer *bar = memnew(HBoxContainer);
	add_child(bar);

	_mode_ob = memnew(OptionButton);
	bar->add_child(_mode_ob);
	_mode_ob->set_h_size_flags(Control::SIZE_EXPAND_FILL);

	_mode_ob->add_item("Luminance", MODE_LUMINANCE);
	_mode_ob->add_item("Red", MODE_RED);
	_mode_ob->add_item("Green", MODE_GREEN);
	_mode_ob->add_item("Blue", MODE_BLUE);
	_mode_ob->add_item("Alpha", MODE_ALPHA);

	_mode_ob->select(MODE_LUMINANCE);
	_mode_ob->connect("item_selected", this, "on_mode_item_selected");

	Button *auto_button = memnew(Button);
	bar->add_child(auto_button);
	auto_button->connect("pressed", this, "on_auto_levels_pressed");

	auto_button->set_text("AL");
	auto_button->set_tooltip("Set levels automatically");
	auto_button->set_v_size_flags(Control::SIZE_SHRINK_CENTER);

	/*
	Control *spacer = memnew(Control);
	spacer->set_custom_minimum_size(Vector2(0, 4));
	add_child(spacer);
	*/

	_histogram_tr = memnew(TextureRect);
	_histogram_tr->set_custom_minimum_size(Vector2(0, 100));
	_histogram_tr->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	add_child(_histogram_tr);

	_cursor_in_min = memnew(MMTonesEditorCursor);
	_cursor_in_mid = memnew(MMTonesEditorCursor);
	_cursor_in_max = memnew(MMTonesEditorCursor);
	_cursor_out_min = memnew(MMTonesEditorCursor);
	_cursor_out_max = memnew(MMTonesEditorCursor);

	_cursor_in_min->initialize(Color(0, 0, 0), 0, true);
	_cursor_in_mid->initialize(Color(0.5, 0.5, 0.5), 0.5, true);
	_cursor_in_max->initialize(Color(1, 1, 1), 1, true);
	_cursor_out_min->initialize(Color(0, 0, 0), 0, false);
	_cursor_out_max->initialize(Color(1, 1, 1), 1, false);

	_histogram_tr->add_child(_cursor_in_min);
	_histogram_tr->add_child(_cursor_in_mid);
	_histogram_tr->add_child(_cursor_in_max);
	_histogram_tr->add_child(_cursor_out_min);
	_histogram_tr->add_child(_cursor_out_max);

	_cursor_in_min->connect("cursor_value_changed", this, "on_cursor_value_changed");
	_cursor_in_mid->connect("cursor_value_changed", this, "on_cursor_value_changed");
	_cursor_in_max->connect("cursor_value_changed", this, "on_cursor_value_changed");
	_cursor_out_min->connect("cursor_value_changed", this, "on_cursor_value_changed");
	_cursor_out_max->connect("cursor_value_changed", this, "on_cursor_value_changed");

	/*
	spacer = memnew(Control);
	spacer->set_custom_minimum_size(Vector2(0, 4));
	add_child(spacer);
	*/
}

MMTonesEditor::~MMTonesEditor() {
}

void MMTonesEditor::on_input_property_changed() {
	if (!_node.is_valid()) {
		_histogram_tr->set_texture(make_default_histogram());
		return;
	}

	Ref<Image> img = _node->get_image()->get_active_image();

	if (!img.is_valid()) {
		_histogram_tr->set_texture(make_default_histogram());
		return;
	}

	_histogram_tr->set_texture(make_histogram(img));
}

void MMTonesEditor::on_auto_levels_pressed() {
	// TODO needs proper histogram generation
	/*
	var histogram = $Histogram.get_histogram_texture().get_data()
	histogram.lock()
	var in_min : int = -1
	var in_mid : int = -1
	var in_mid_value : float = 0
	var in_max : int = -1
	var histogram_size = histogram.get_size().x
	for i in range(histogram_size):
		var color : Color = histogram.get_pixel(i, 0)
		var value : float
		match $Bar/Mode.selected:
			0:
				value = (color.r+color.g+color.b)/3.0
			1:
				value = color.r
			2:
				value = color.g
			3:
				value = color.b
			4:
				value = color.a
		if value > 0.0:
			if in_min == -1:
				in_min = i
			in_max = i
			if in_mid_value < value:
				in_mid = i
				in_mid_value = value
	histogram.unlock()
	cursor_in_min.update_value(in_min/(histogram_size-1))
	cursor_in_mid.update_value(in_mid/(histogram_size-1))
	cursor_in_max.update_value(in_max/(histogram_size-1))
	*/
}

void MMTonesEditor::on_mode_item_selected(int id) {
	_node->set_current_mode(id);
	set_mode(static_cast<Modes>(id));
}

void MMTonesEditor::on_cursor_value_changed(Control *cursor, float position) {
	if (cursor == _cursor_in_min) {
		set_parameter_current_mode(PARAMETER_TYPE_IN_MIN, position, 0);
	} else if (cursor == _cursor_in_mid) {
		set_parameter_current_mode(PARAMETER_TYPE_IN_MID, position, 0.5);
	} else if (cursor == _cursor_in_max) {
		set_parameter_current_mode(PARAMETER_TYPE_IN_MAX, position, 1);
	} else if (cursor == _cursor_out_min) {
		set_parameter_current_mode(PARAMETER_TYPE_OUT_MIN, position, 0);
	} else if (cursor == _cursor_out_max) {
		set_parameter_current_mode(PARAMETER_TYPE_OUT_MAX, position, 1);
	}
}

void MMTonesEditor::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_RESIZED: {
			_cursor_in_min->resize();
			_cursor_in_mid->resize();
			_cursor_in_max->resize();
			_cursor_out_min->resize();
			_cursor_out_max->resize();
		} break;
		default:
			break;
	}
}

void MMTonesEditor::_bind_methods() {
	ClassDB::bind_method(D_METHOD("on_input_property_changed"), &MMTonesEditor::on_input_property_changed);
	ClassDB::bind_method(D_METHOD("on_auto_levels_pressed"), &MMTonesEditor::on_auto_levels_pressed);
	ClassDB::bind_method(D_METHOD("on_mode_item_selected"), &MMTonesEditor::on_mode_item_selected);
	ClassDB::bind_method(D_METHOD("on_cursor_value_changed"), &MMTonesEditor::on_cursor_value_changed);
}
