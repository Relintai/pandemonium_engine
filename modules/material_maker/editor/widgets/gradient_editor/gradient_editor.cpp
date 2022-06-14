
#include "gradient_editor.h"

#include "../../../nodes/bases/gradient_base.h"
#include "../../mm_graph_node.h"

#include "scene/gui/color_rect.h"
#include "scene/gui/label.h"
#include "scene/gui/option_button.h"
#include "scene/resources/material.h"

MMGraphNode *GradientEditor::get_graph_node() {
	return graph_node;
}

void GradientEditor::set_graph_node(MMGraphNode *val) {
	graph_node = val;
}

Ref<GradientBase> GradientEditor::get_value() {
	return value;
}

void GradientEditor::set_value(const Ref<GradientBase> &val) {
	value = val;
}

bool GradientEditor::get_embedded() const {
	return embedded;
}

void GradientEditor::set_embedded(const bool val) {
	embedded = val;
}

UndoRedo *GradientEditor::get_undo_redo() {
	return _undo_redo;
}

void GradientEditor::set_undo_redo(UndoRedo *val) {
	_undo_redo = val;
}

PoolRealArray GradientEditor::get_saved_points() {
	return _saved_points;
}

void GradientEditor::set_saved_points(const PoolRealArray &val) {
	_saved_points = val;
}

GradientCursor *GradientEditor::get_active_cursor() {
	return active_cursor;
}

void GradientEditor::set_active_cursor(GradientCursor *val) {
	active_cursor = val;
}

void GradientEditor::_init() {
	connect("resized", this, "on_resized");
}

void GradientEditor::ignore_changes(const bool val) {
	graph_node->ignore_changes(val);
}

void GradientEditor::save_color_state() {
	PoolRealArray p = value->get_points();
	_saved_points.resize(0);

	for (int i = 0; i < p.size(); ++i) {
		_saved_points.push_back(p[i]);
	}

	ignore_changes(true);
}

void GradientEditor::undo_redo_save_color_state() {
	PoolRealArray op;
	PoolRealArray np;

	for (int i = 0; i < _saved_points.size(); ++i) {
		op.push_back(_saved_points[i]);
	}

	for (v in value->get_points()) {
		np.push_back(v);
	}

	_undo_redo.create_action("MMGD: gradient colors changed");
	_undo_redo.add_do_method(value, "set_points", np);
	_undo_redo.add_undo_method(value, "set_points", op);
	_undo_redo.commit_action();
	ignore_changes(false);
}

void GradientEditor::set_undo_redo(const UndoRedo &ur) {
	_undo_redo = ur;
}

void GradientEditor::set_value(const Variant &v) {
	value = v;
	update_preview();
	call_deferred("update_cursors");
}

void GradientEditor::update_cursors() {
	for (c in get_children()) {
		if (c is GradientCursor) {
			remove_child(c);
			c.free();
		}
	}

	int vs = value.get_point_count();

	for (int i = 0; i < vs; ++i) { //i in range(vs)
		add_cursor(value.get_point_value(i) * (rect_size.x - GradientCursor.WIDTH), value.get_point_color(i));
	}

	$Interpolation.selected = value.interpolation_type;
}

void GradientEditor::update_value() {
	value.clear();
	Array sc = get_sorted_cursors();
	PoolRealArray points = PoolRealArray();

	for (c in sc) {
		points.push_back(c.rect_position.x / (rect_size.x - GradientCursor.WIDTH));
		Color color = c.color;
		points.push_back(color.r);
		points.push_back(color.g);
		points.push_back(color.b);
		points.push_back(color.a);
	}

	value.set_points(points);
	update_preview();
}

void GradientEditor::add_cursor(const Variant &x, const Variant &color) {
	GradientCursor *cursor = GradientCursor.new();
	cursor->set_label(label);

	add_child(cursor);
	cursor.rect_position.x = x;
	cursor.color = color;
}

void GradientEditor::_gui_input(const Ref<InputEvent> &ev) {
	Ref<InputEventMouseButton> iemb = ev;

	if (iemb.is_valid() && ev->get_button_index() == 1 && iemb->get_doubleclick()) {
		if (iemb->get_position().y > 15) {
			Variant = clamp(iemb->get_position().x, 0, rect_size.x - GradientCursor.WIDTH);
			save_color_state();
			add_cursor(p, get_gradient_color(p));
			update_value();
			undo_redo_save_color_state();
		} else if (embedded) {
			GradientPopup *popup = memnew(GradientPopup);
			add_child(popup);
			Vector2 popup_size = popup->get_size();
			popup->popup(Rect2(ev->get_global_position(), Vector2(0, 0)));
			popup->set_global_position(ev->get_global_position() - Vector2(popup_size.x / 2, popup_size.y));
			popup->init(value, graph_node, _undo_redo);
			popup->connect("updated", self, "set_value");
			popup->connect("popup_hide", popup, "queue_free");
		}

		// Showing a color picker popup to change a cursor's color;
	}
}

void GradientEditor::select_color(const GradientCursor *cursor, const Vector2 &position) {
	active_cursor = cursor;
	//var color_picker_popup = preload("res://addons/mat_maker_gd/widgets/color_picker_popup/color_picker_popup.tscn").instance();
	add_child(color_picker_popup);
	Variant = color_picker_popup.get_node("ColorPicker");
	color_picker.color = cursor.color;
	color_picker.connect("color_changed", cursor, "set_color");
	color_picker_popup.rect_position = position;
	color_picker_popup.connect("popup_hide", self, "undo_redo_save_color_state");
	color_picker_popup.connect("popup_hide", color_picker_popup, "queue_free");
	color_picker_popup.popup();
}

// Calculating a color from the gradient and generating the shader;

Array GradientEditor::get_sorted_cursors() {
	Variant = [];

	for (c in get_children()) {
		if (c is GradientCursor) {
			array.append(c);
		}
	}

	array.sort_custom(GradientCursor, "sort");
	return array;
}

void GradientEditor::generate_preview_image() {
	Ref<ImageTexture> tex = $Gradient.texture;

	if (!tex.is_valid()) {
		tex.instance();
		$Gradient.texture = tex;
	}

	Ref<Image> img = tex.get_data();
	float w = $Gradient.rect_size.x;
	float h = $Gradient.rect_size.y;

	if (!img.is_valid()) {
		img = memnew(Image);
	}

	if (img->get_size().x != w || img.get_size().y != h) {
		img->create(w, h, false, Image.FORMAT_RGBA8);
	}

	img->lock();

	for (int i = 0; i < w; ++i) { //i in range(w)
		float x = float(i) / float(w);
		Color col = value->get_gradient_color(x);

		for (int j = 0; j < h; ++j) { //j in range(h)
			img->set_pixel(i, j, col);
		}
	}

	img->unlock();
	tex->create_from_image(img, 0);
}

Color GradientEditor::get_gradient_color(const float x) {
	return value->get_gradient_color(x / (get_size().x - GradientCursor->WIDTH));
}

void GradientEditor::update_preview() {
	call_deferred("generate_preview_image");
}

void GradientEditor::_on_Interpolation_item_selected(const int ID) {
	ignore_changes(true);
	_undo_redo.create_action("MMGD: gradient interpolation_type changed");
	_undo_redo.add_do_method(value, "set_interpolation_type", ID);
	_undo_redo.add_undo_method(value, "set_interpolation_type", value->get_interpolation_type());
	_undo_redo.commit_action();
	ignore_changes(false);
	update_preview();
}

void GradientEditor::on_resized() {
	if (value.is_valid()) {
		update_preview();
		call_deferred("update_cursors");
	}
}

GradientEditor::GradientEditor() {
	graph_node = nullptr;
	embedded = false;
	_undo_redo = nullptr;
	active_cursor = nullptr;

	set_custom_minimum_size(Vector2(120, 32));
	set_focus_mode(FOCUS_CLICK);

	ColorRect *background_control = memnew(ColorRect);
	background_control->set_custom_minimum_size(Vector2(112, 17));
	background_control->set_mouse_filter(MOUSE_FILTER_IGNORE);

	String bg_shader_code = "shader_type canvas_item;\n"
							"\n"
							"void fragment() {\n"
							"	COLOR = vec4(vec3(2.0*fract(0.5*(floor(0.12*FRAGCOORD.x)+floor(0.125*FRAGCOORD.y)))), 1.0);/n"
							"}";

	Ref<Shader> bg_shader;
	bg_shader.instance();
	bg_shader->set_code(bg_shader_code);

	Ref<ShaderMaterial> background_control_prop_material;
	background_control_prop_material.instance();
	background_control_prop_material->set_shader(bg_shader_code);
	background_control->set_material(background_control_prop_material);

	add_child(background_control);

	TextureRect *gradient_control = memnew(TextureRect);
	add_child(gradient_control);
	gradient_control->set_rect_min_size(Vector2(112, 17));
	gradient_control->set_mouse_filter(MOUSE_FILTER_IGNORE);

	Ref<Theme> gradient_control_prop_theme;
	gradient_control_prop_theme.instance();
	gradient_control->set_theme(gradient_control_prop_theme);

	OptionButton *interpolation_control = memnew(OptionButton);
	interpolation_control->set_rect_scale(Vector2(0.5, 0.5));

	interpolation_control->add_item("0", 0);
	interpolation_control->add_item("1", 1);
	interpolation_control->add_item("2", 2);
	interpolation_control->add_item("3", 3);

	//Ref<AtlasTexture> interpolation_control_prop_icon;
	//interpolation_control_prop_icon.instance();
	//interpolation_control->set_icon(interpolation_control_prop_icon);
	//interpolation_control->set("icon", interpolation_control_prop_icon);
	//interpolation_control property items TYPE_ARRAY value: [, [AtlasTexture:19169], False, 0, Null, , [AtlasTexture:19168], False, 1, Null, , [AtlasTexture:19170], False, 2, Null, , [AtlasTexture:19171], False, 3, Null]
	interpolation_control->set_selected(1);
	interpolation_control->connect("item_selected", this, "_on_Interpolation_item_selected");
	add_child(interpolation_control);

	Label *value_control = memnew(Label);
	value_control->set_align(ALIGN_CENTER);
	add_child(value_control);

	value_control->set("custom_colors/font_color", Color(1, 1, 1, 1));
	value_control->set("custom_colors/font_color_shadow", Color(0, 0, 0, 1));
	value_control->set("custom_constants/shadow_offset_x", 1);
	value_control->set("custom_constants/shadow_offset_y", 1);
	value_control->set("custom_constants/shadow_as_outline", 1);
}

GradientEditor::~GradientEditor() {
}

void GradientEditor::_bind_methods() {
	ADD_SIGNAL(MethodInfo("updated", PropertyInfo(Variant::OBJECT, "value", PROPERTY_HINT_RESOURCE_TYPE, "GradientBase")));

	//ClassDB::bind_method(D_METHOD("get_graph_node"), &GradientEditor::get_graph_node);
	//ClassDB::bind_method(D_METHOD("set_graph_node", "value"), &GradientEditor::set_graph_node);
	//ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "graph_node", PROPERTY_HINT_RESOURCE_TYPE, "MMGraphNode "), "set_graph_node", "get_graph_node");

	ClassDB::bind_method(D_METHOD("get_value"), &GradientEditor::get_value);
	ClassDB::bind_method(D_METHOD("set_value", "value"), &GradientEditor::set_value);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "value", PROPERTY_HINT_RESOURCE_TYPE, "GradientBase"), "set_value", "get_value");

	ClassDB::bind_method(D_METHOD("get_embedded"), &GradientEditor::get_embedded);
	ClassDB::bind_method(D_METHOD("set_embedded", "value"), &GradientEditor::set_embedded);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "embedded"), "set_embedded", "get_embedded");

	ClassDB::bind_method(D_METHOD("get_undo_redo"), &GradientEditor::get_undo_redo);
	//ClassDB::bind_method(D_METHOD("set_undo_redo", "value"), &GradientEditor::set_undo_redo);
	//ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "undo_redo", PROPERTY_HINT_RESOURCE_TYPE, "UndoRedo"), "set_undo_redo", "get_undo_redo");

	ClassDB::bind_method(D_METHOD("get_saved_points"), &GradientEditor::get__saved_points);
	ClassDB::bind_method(D_METHOD("set_saved_points", "value"), &GradientEditor::set__saved_points);
	ADD_PROPERTY(PropertyInfo(Variant::POOL_REAL_ARRAY, "saved_points"), "set_saved_points", "get_saved_points");

	//ClassDB::bind_method(D_METHOD("get_active_cursor"), &GradientEditor::get_active_cursor);
	//ClassDB::bind_method(D_METHOD("set_active_cursor", "value"), &GradientEditor::set_active_cursor);
	//ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "active_cursor", PROPERTY_HINT_RESOURCE_TYPE, "GradientCursor "), "set_active_cursor", "get_active_cursor");

	ClassDB::bind_method(D_METHOD("ignore_changes", "val"), &GradientEditor::ignore_changes);

	ClassDB::bind_method(D_METHOD("save_color_state"), &GradientEditor::save_color_state);

	ClassDB::bind_method(D_METHOD("undo_redo_save_color_state"), &GradientEditor::undo_redo_save_color_state);

	//ClassDB::bind_method(D_METHOD("set_undo_redo", "ur"), &GradientEditor::set_undo_redo);

	ClassDB::bind_method(D_METHOD("set_value", "v"), &GradientEditor::set_value);
	ClassDB::bind_method(D_METHOD("update_cursors"), &GradientEditor::update_cursors);
	ClassDB::bind_method(D_METHOD("update_value"), &GradientEditor::update_value);
	ClassDB::bind_method(D_METHOD("add_cursor", "x", " color"), &GradientEditor::add_cursor);

	ClassDB::bind_method(D_METHOD("_gui_input", "ev"), &GradientEditor::_gui_input);

	ClassDB::bind_method(D_METHOD("select_color", "cursor", " position"), &GradientEditor::select_color);

	ClassDB::bind_method(D_METHOD("get_sorted_cursors"), &GradientEditor::get_sorted_cursors);

	ClassDB::bind_method(D_METHOD("generate_preview_image"), &GradientEditor::generate_preview_image);

	ClassDB::bind_method(D_METHOD("get_gradient_color", "x"), &GradientEditor::get_gradient_color);

	ClassDB::bind_method(D_METHOD("update_preview"), &GradientEditor::update_preview);

	ClassDB::bind_method(D_METHOD("_on_Interpolation_item_selected", "ID"), &GradientEditor::_on_Interpolation_item_selected);
	ClassDB::bind_method(D_METHOD("on_resized"), &GradientEditor::on_resized);
}
