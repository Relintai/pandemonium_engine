
#include "tones_editor.h"

#include "scene/resources/texture.h"

#include "../../../nodes/filter/tones.h"

#include "scene/gui/button.h"
#include "scene/gui/option_button.h"
#include "scene/gui/texture_rect.h"

#include "../../../algos/mm_algos.h"

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
	}

	on_input_property_changed();
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

	Button *auto_button = memnew(Button);
	bar->add_child(auto_button);

	auto_button->set_text("AL");
	auto_button->set_tooltip("Set levels automatically");
	auto_button->set_v_size_flags(Control::SIZE_SHRINK_CENTER);

	Control *spacer = memnew(Control);
	spacer->set_custom_minimum_size(Vector2(0, 4));
	add_child(spacer);

	_histogram_tr = memnew(TextureRect);
	_histogram_tr->set_custom_minimum_size(Vector2(0, 100));
	_histogram_tr->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	add_child(_histogram_tr);

	spacer = memnew(Control);
	spacer->set_custom_minimum_size(Vector2(0, 4));
	add_child(spacer);
}

MMTonesEditor::~MMTonesEditor() {
}

void MMTonesEditor::on_input_property_changed() {
	if (!_node.is_valid()) {
		_histogram_tr->set_texture(make_default_histogram());
		ERR_PRINT("asdasd");
		return;
	}

	Ref<Image> img = _node->get_image()->get_active_image();

	if (!img.is_valid()) {
		_histogram_tr->set_texture(make_default_histogram());
		ERR_PRINT("asdasd2");
		return;
	}

	ERR_PRINT("asdasd3");

	_histogram_tr->set_texture(make_histogram(img));
}

void MMTonesEditor::_bind_methods() {
	ClassDB::bind_method(D_METHOD("on_input_property_changed"), &MMTonesEditor::on_input_property_changed);
}
