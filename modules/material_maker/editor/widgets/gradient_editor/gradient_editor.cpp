
#include "gradient_editor.h"

#include "../../../nodes/bases/gradient_base.h"
#include "../../../nodes/mm_material.h"
#include "../../../nodes/mm_node_universal_property.h"
#include "../../mm_graph_node.h"

#include "gradient_cursor.h"
#include "gradient_popup.h"

#include "../color_picker_popup/color_picker_popup.h"
#include "scene/gui/color_picker.h"

#include "scene/gui/color_rect.h"
#include "scene/gui/label.h"
#include "scene/gui/option_button.h"
#include "scene/gui/texture_rect.h"
#include "scene/resources/material.h"

MMGraphNode *MMGradientEditor::get_graph_node() {
	return graph_node;
}

void MMGradientEditor::set_graph_node(MMGraphNode *val) {
	graph_node = val;

	_material = graph_node->get_mm_material();
}

Ref<GradientBase> MMGradientEditor::get_value() {
	return value;
}

void MMGradientEditor::set_value(const Ref<GradientBase> &val) {
	if (value == val) {
		return;
	}

	if (value.is_valid()) {
		value->disconnect("changed", this, "on_value_changed");
	}

	value = val;

	if (value.is_valid()) {
		value->connect("changed", this, "on_value_changed");
	}

	_update_preview_queued = true;
	_update_cursors_queued = true;

	//update_preview();
	//call_deferred("update_cursors");
}

bool MMGradientEditor::get_embedded() const {
	return embedded;
}

void MMGradientEditor::set_embedded(const bool val) {
	embedded = val;
}

UndoRedo *MMGradientEditor::get_undo_redo() {
	return _undo_redo;
}

void MMGradientEditor::set_undo_redo(UndoRedo *val) {
	_undo_redo = val;
}

PoolRealArray MMGradientEditor::get_saved_points() {
	return _saved_points;
}

void MMGradientEditor::set_saved_points(const PoolRealArray &val) {
	_saved_points = val;
}

void MMGradientEditor::ignore_changes(const bool val) {
	graph_node->ignore_changes(val);
}

void MMGradientEditor::save_color_state() {
	PoolRealArray p = value->get_points();
	_saved_points.resize(0);

	for (int i = 0; i < p.size(); ++i) {
		_saved_points.push_back(p[i]);
	}

	ignore_changes(true);
}

void MMGradientEditor::undo_redo_save_color_state() {
	PoolRealArray op;
	PoolRealArray np;

	for (int i = 0; i < _saved_points.size(); ++i) {
		op.push_back(_saved_points[i]);
	}

	PoolRealArray array = value->get_points();

	for (int i = 0; i < array.size(); ++i) {
		np.push_back(array[i]);
	}

	_undo_redo->create_action("MMGD: gradient colors changed");
	_undo_redo->add_do_method(*value, "set_points", np);
	_undo_redo->add_undo_method(*value, "set_points", op);
	_undo_redo->commit_action();
	ignore_changes(false);
}

void MMGradientEditor::cursor_move_started(GradientCursor *c) {
	_cursor_started_moving = true;
	_cursor_moving = true;
}
void MMGradientEditor::cursor_move_ended() {
	_cursor_stopped_moving = true;
	_cursor_moving = false;
}
void MMGradientEditor::cursor_delete_request(GradientCursor *c) {
	_cursor_delete_request = c;
}
void MMGradientEditor::cursor_doubleclicked(GradientCursor *c, const Vector2 &position) {
	_cursor_doubleclicked = c;
	_cursor_doubleclick_position = position;
}

void MMGradientEditor::update_cursors() {
	LocalVector<GradientCursor *> cursors;

	for (int i = 0; i < get_child_count(); ++i) {
		GradientCursor *c = Object::cast_to<GradientCursor>(get_child(i));

		if (c) {
			cursors.push_back(c);
			c->queue_delete();
		}
	}

	for (uint32_t i = 0; i < cursors.size(); ++i) {
		remove_child(cursors[i]);
	}

	cursors.clear();

	int vs = value->get_point_count();

	for (int i = 0; i < vs; ++i) { //i in range(vs)
		add_cursor(value->get_point_value(i) * (get_size().x - GradientCursor::WIDTH), value->get_point_color(i));
	}

	interpolation->select(value->get_interpolation_type());
}

void MMGradientEditor::update_value() {
	value->clear();
	Vector<GradientCursor *> sc = get_sorted_cursors();
	PoolRealArray points;

	for (int i = 0; i < sc.size(); ++i) {
		GradientCursor *c = sc[i];

		points.push_back(c->get_position().x / (get_size().x - GradientCursor::WIDTH));
		Color color = c->get_cursor_color();
		points.push_back(color.r);
		points.push_back(color.g);
		points.push_back(color.b);
		points.push_back(color.a);
	}

	value->set_points(points);
	//_update_cursors_queued = true;
	//update_preview();
}

void MMGradientEditor::add_cursor(const float x, const Color &color) {
	GradientCursor *cursor = memnew(GradientCursor);
	cursor->set_label(cursor_value_label);
	add_child(cursor);

	Vector2 rp = cursor->get_position();
	rp.x = x;

	cursor->set_position(rp);
	cursor->set_cursor_color(color);
}

void MMGradientEditor::_gui_input(const Ref<InputEvent> &ev) {
	Ref<InputEventMouseButton> iemb = ev;

	if (iemb.is_valid() && iemb->get_button_index() == 1 && iemb->is_doubleclick()) {
		if (iemb->get_position().y > 15) {
			float p = CLAMP(iemb->get_position().x, 0, get_size().x - GradientCursor::WIDTH);

			_cursor_add_queued = true;
			_cursor_add_x = p;

		} else if (embedded) {
			GradientPopup *popup = memnew(GradientPopup);
			add_child(popup);
			Vector2 popup_size = popup->get_size();
			popup->popup(Rect2(iemb->get_global_position(), Vector2(0, 0)));
			popup->set_global_position(iemb->get_global_position() - Vector2(popup_size.x / 2, popup_size.y));
			popup->init(value, graph_node, _undo_redo);
			popup->connect("updated", this, "set_value");
			popup->connect("popup_hide", popup, "queue_free");
		}

		// Showing a color picker popup to change a cursor's color;
	}
}

void MMGradientEditor::select_color(GradientCursor *cursor, const Vector2 &position) {
	ColorPickerPopup *color_picker_popup = memnew(ColorPickerPopup);
	add_child(color_picker_popup);

	ColorPicker *color_picker = color_picker_popup->color_picker;
	color_picker->set_pick_color(cursor->get_cursor_color());
	color_picker->connect("color_changed", cursor, "set_cursor_color_notify");

	color_picker_popup->set_position(position);
	color_picker_popup->connect("popup_hide", this, "on_color_selector_closed");
	color_picker_popup->connect("popup_hide", color_picker_popup, "queue_free");
	color_picker_popup->popup();
}

// Calculating a color from the gradient and generating the shader;

struct GradientCursorCustomSorter {
	_FORCE_INLINE_ bool operator()(const GradientCursor *a, const GradientCursor *b) const { return a->operator<(*b); }
};

Vector<GradientCursor *> MMGradientEditor::get_sorted_cursors() {
	Vector<GradientCursor *> array;

	for (int i = 0; i < get_child_count(); ++i) {
		GradientCursor *c = Object::cast_to<GradientCursor>(get_child(i));

		if (c) {
			array.push_back(c);
		}
	}

	array.sort_custom<GradientCursorCustomSorter>();

	return array;
}

void MMGradientEditor::generate_preview_image() {
	Ref<ImageTexture> tex = gradient->get_texture();

	if (!tex.is_valid()) {
		tex.instance();
		gradient->set_texture(tex);
	}

	Ref<Image> img = tex->get_data();
	float w = gradient->get_size().x;
	float h = gradient->get_size().y;

	if (!img.is_valid()) {
		img.instance();
	}

	if (img->get_size().x != w || img->get_size().y != h) {
		img->create(w, h, false, Image::FORMAT_RGBA8);
	}

	img->lock();

	for (int i = 0; i < w; ++i) {
		float x = float(i) / float(w);
		Color col = value->get_gradient_color(x);

		for (int j = 0; j < h; ++j) {
			img->set_pixel(i, j, col);
		}
	}

	img->unlock();
	tex->create_from_image(img, 0);
}

Color MMGradientEditor::get_gradient_color(const float x) {
	return value->get_gradient_color(x / (get_size().x - GradientCursor::WIDTH));
}

void MMGradientEditor::update_preview() {
	//call_deferred("generate_preview_image");
	generate_preview_image();
}

void MMGradientEditor::_on_Interpolation_item_selected(const int ID) {
	_new_interpolation = ID;
	_interpolation_change_queued = true;
}

void MMGradientEditor::apply_new_interpolation() {
	ignore_changes(true);
	_undo_redo->create_action("MMGD: gradient interpolation_type changed");
	_undo_redo->add_do_method(value.ptr(), "set_interpolation_type", _new_interpolation);
	_undo_redo->add_undo_method(value.ptr(), "set_interpolation_type", value->get_interpolation_type());
	_undo_redo->commit_action();
	ignore_changes(false);
	update_preview();
}

void MMGradientEditor::on_resized() {
	_update_preview_queued = true;
	_update_cursors_queued = true;
}

void MMGradientEditor::on_color_selector_closed() {
	_color_selection_done = true;
	_selecting_color = false;
}

void MMGradientEditor::on_value_changed() {
	if (!graph_node->get_ignore_change_event()) {
		_color_selection_done = true;
		_selecting_color = false;
	}
}

MMGradientEditor::MMGradientEditor() {
	graph_node = nullptr;
	embedded = false;
	_undo_redo = nullptr;
	_update_preview_queued = false;
	_update_cursors_queued = false;

	_cursor_moving = false;
	_cursor_started_moving = false;
	_cursor_stopped_moving = false;
	_cursor_doubleclicked = nullptr;
	_cursor_delete_request = nullptr;
	_cursor_add_queued = false;
	_cursor_add_x = 0;
	_selecting_color = false;
	_color_selection_done = false;

	_interpolation_change_queued = false;
	_new_interpolation = 0;

	set_custom_minimum_size(Vector2(120, 32));
	set_focus_mode(FOCUS_CLICK);
	set_h_size_flags(SIZE_EXPAND_FILL);

	ColorRect *background_control = memnew(ColorRect);
	background_control->set_custom_minimum_size(Vector2(112, 17));
	background_control->set_mouse_filter(MOUSE_FILTER_IGNORE);

	String bg_shader_code = "shader_type canvas_item;\n"
							"\n"
							"void fragment() {\n"
							"	COLOR = vec4(vec3(2.0*fract(0.5*(floor(0.12*FRAGCOORD.x)+floor(0.125*FRAGCOORD.y)))), 1.0);\n"
							"}\n";

	Ref<Shader> bg_shader;
	bg_shader.instance();
	bg_shader->set_code(bg_shader_code);

	Ref<ShaderMaterial> background_control_prop_material;
	background_control_prop_material.instance();
	background_control_prop_material->set_shader(bg_shader);
	background_control->set_material(background_control_prop_material);

	add_child(background_control);

	gradient = memnew(TextureRect);
	gradient->set_custom_minimum_size(Vector2(112, 17));
	gradient->set_mouse_filter(MOUSE_FILTER_IGNORE);
	gradient->set_anchors_and_margins_preset(PRESET_TOP_WIDE);
	add_child(gradient);

	Ref<Theme> gradient_control_prop_theme;
	gradient_control_prop_theme.instance();
	gradient->set_theme(gradient_control_prop_theme);

	interpolation = memnew(OptionButton);
	interpolation->set_scale(Vector2(0.5, 0.5));

	interpolation->add_item("0", 0);
	interpolation->add_item("1", 1);
	interpolation->add_item("2", 2);
	interpolation->add_item("3", 3);

	//Ref<AtlasTexture> interpolation_control_prop_icon;
	//interpolation_control_prop_icon.instance();
	//interpolation->set_icon(interpolation_control_prop_icon);
	//interpolation->set("icon", interpolation_control_prop_icon);
	//interpolation_control property items TYPE_ARRAY value: [, [AtlasTexture:19169], False, 0, Null, , [AtlasTexture:19168], False, 1, Null, , [AtlasTexture:19170], False, 2, Null, , [AtlasTexture:19171], False, 3, Null]
	interpolation->select(1);
	interpolation->connect("item_selected", this, "_on_Interpolation_item_selected");
	add_child(interpolation);

	cursor_value_label = memnew(Label);
	cursor_value_label->set_align(Label::ALIGN_CENTER);

	cursor_value_label->set("custom_colors/font_color", Color(1, 1, 1, 1));
	cursor_value_label->set("custom_colors/font_color_shadow", Color(0, 0, 0, 1));
	cursor_value_label->set("custom_constants/shadow_offset_x", 1);
	cursor_value_label->set("custom_constants/shadow_offset_y", 1);
	cursor_value_label->set("custom_constants/shadow_as_outline", 1);

	add_child(cursor_value_label);

	set_process_internal(true);
}

MMGradientEditor::~MMGradientEditor() {
}

void MMGradientEditor::_notification(int p_what) {
	if (p_what == NOTIFICATION_POSTINITIALIZE) {
		connect("resized", this, "on_resized");
	} else if (p_what == NOTIFICATION_INTERNAL_PROCESS) {
		if (value.is_valid() && _material.is_valid()) {
			if (_material->get_rendering()) {
				return;
			}

			if (_selecting_color) {
				//update_preview();
				return;
			}

			if (_color_selection_done) {
				_color_selection_done = false;

				save_color_state();
				update_value();
				undo_redo_save_color_state();
			}

			if (_cursor_started_moving) {
				_cursor_started_moving = false;
				save_color_state();
			}

			if (_cursor_moving) {
				//update_preview();
				return;
			}

			if (_cursor_stopped_moving) {
				_cursor_stopped_moving = false;

				update_value();
				undo_redo_save_color_state();
				_update_preview_queued = true;
			}

			if (_cursor_doubleclicked) {
				if (ObjectDB::instance_validate(_cursor_doubleclicked)) {
					select_color(_cursor_doubleclicked, _cursor_doubleclick_position);
				}

				_cursor_doubleclicked = nullptr;
				return;
			}

			if (_interpolation_change_queued) {
				_interpolation_change_queued = false;
				apply_new_interpolation();
			}

			if (_cursor_add_queued) {
				_cursor_add_queued = false;

				save_color_state();
				add_cursor(_cursor_add_x, get_gradient_color(_cursor_add_x));
				update_value();
				undo_redo_save_color_state();

				_update_preview_queued = true;
			}

			if (_cursor_delete_request) {
				if (ObjectDB::instance_validate(_cursor_delete_request)) {
					save_color_state();
					_cursor_delete_request->queue_delete();
					remove_child(_cursor_delete_request);
					update_value();
					undo_redo_save_color_state();
				}

				_cursor_delete_request = nullptr;
			}

			if (_update_cursors_queued) {
				_update_cursors_queued = false;
				update_cursors();
			}

			if (_update_preview_queued) {
				_update_preview_queued = false;
				update_preview();
			}
		}
	}
}

void MMGradientEditor::_bind_methods() {
	ADD_SIGNAL(MethodInfo("updated", PropertyInfo(Variant::OBJECT, "value", PROPERTY_HINT_RESOURCE_TYPE, "GradientBase")));

	//ClassDB::bind_method(D_METHOD("get_graph_node"), &MMGradientEditor::get_graph_node);
	//ClassDB::bind_method(D_METHOD("set_graph_node", "value"), &MMGradientEditor::set_graph_node);
	//ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "graph_node", PROPERTY_HINT_RESOURCE_TYPE, "MMGraphNode "), "set_graph_node", "get_graph_node");

	ClassDB::bind_method(D_METHOD("get_value"), &MMGradientEditor::get_value);
	ClassDB::bind_method(D_METHOD("set_value", "value"), &MMGradientEditor::set_value);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "value", PROPERTY_HINT_RESOURCE_TYPE, "GradientBase"), "set_value", "get_value");

	ClassDB::bind_method(D_METHOD("get_embedded"), &MMGradientEditor::get_embedded);
	ClassDB::bind_method(D_METHOD("set_embedded", "value"), &MMGradientEditor::set_embedded);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "embedded"), "set_embedded", "get_embedded");

	ClassDB::bind_method(D_METHOD("get_undo_redo"), &MMGradientEditor::get_undo_redo);
	//ClassDB::bind_method(D_METHOD("set_undo_redo", "value"), &MMGradientEditor::set_undo_redo);
	//ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "undo_redo", PROPERTY_HINT_RESOURCE_TYPE, "UndoRedo"), "set_undo_redo", "get_undo_redo");

	ClassDB::bind_method(D_METHOD("get_saved_points"), &MMGradientEditor::get_saved_points);
	ClassDB::bind_method(D_METHOD("set_saved_points", "value"), &MMGradientEditor::set_saved_points);
	ADD_PROPERTY(PropertyInfo(Variant::POOL_REAL_ARRAY, "saved_points"), "set_saved_points", "get_saved_points");

	ClassDB::bind_method(D_METHOD("ignore_changes", "val"), &MMGradientEditor::ignore_changes);

	ClassDB::bind_method(D_METHOD("save_color_state"), &MMGradientEditor::save_color_state);

	ClassDB::bind_method(D_METHOD("undo_redo_save_color_state"), &MMGradientEditor::undo_redo_save_color_state);

	//ClassDB::bind_method(D_METHOD("set_undo_redo", "ur"), &MMGradientEditor::set_undo_redo);

	ClassDB::bind_method(D_METHOD("update_cursors"), &MMGradientEditor::update_cursors);
	ClassDB::bind_method(D_METHOD("update_value"), &MMGradientEditor::update_value);
	ClassDB::bind_method(D_METHOD("add_cursor", "x", " color"), &MMGradientEditor::add_cursor);

	ClassDB::bind_method(D_METHOD("_gui_input", "ev"), &MMGradientEditor::_gui_input);

	//ClassDB::bind_method(D_METHOD("select_color", "cursor", " position"), &MMGradientEditor::select_color);

	//ClassDB::bind_method(D_METHOD("get_sorted_cursors"), &MMGradientEditor::get_sorted_cursors);

	ClassDB::bind_method(D_METHOD("generate_preview_image"), &MMGradientEditor::generate_preview_image);

	ClassDB::bind_method(D_METHOD("get_gradient_color", "x"), &MMGradientEditor::get_gradient_color);

	ClassDB::bind_method(D_METHOD("update_preview"), &MMGradientEditor::update_preview);

	ClassDB::bind_method(D_METHOD("_on_Interpolation_item_selected", "ID"), &MMGradientEditor::_on_Interpolation_item_selected);
	ClassDB::bind_method(D_METHOD("on_resized"), &MMGradientEditor::on_resized);
	ClassDB::bind_method(D_METHOD("on_color_selector_closed"), &MMGradientEditor::on_color_selector_closed);
	ClassDB::bind_method(D_METHOD("on_value_changed"), &MMGradientEditor::on_value_changed);
}
