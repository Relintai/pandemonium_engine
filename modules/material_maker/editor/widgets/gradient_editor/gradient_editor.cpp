
#include "gradient_editor.h"

Variant GradientEditor::get_Variant() {
	return Variant;
}

void GradientEditor::set_Variant(const Variant &val) {
	Variant = val;
}

Variant GradientEditor::get_Variant() {
	return Variant;
}

void GradientEditor::set_Variant(const Variant &val) {
	Variant = val;
}

bool GradientEditor::get_embedded() const {
	return embedded;
}

void GradientEditor::set_embedded(const bool val) {
	embedded = val;
}

UndoRedo GradientEditor::get_ *_undo_redo() {
	return *_undo_redo;
}

void GradientEditor::set_ *_undo_redo(const UndoRedo &val) {
	*_undo_redo = val;
}

PoolRealArray GradientEditor::get__saved_points() {
	return _saved_points;
}

void GradientEditor::set__saved_points(const PoolRealArray &val) {
	_saved_points = val;
}

Variant GradientEditor::get_Variant() {
	return Variant;
}

void GradientEditor::set_Variant(const Variant &val) {
	Variant = val;
}

//tool;
Variant = null;
// setget set_value;
Variant = null;
//export ;
bool embedded = true;
UndoRedo *_undo_redo = null;
signal updated(value);
PoolRealArray _saved_points = PoolRealArray();

void GradientEditor::_init() {
	connect("resized", self, "on_resized");
}

void GradientEditor::ignore_changes(const Variant &val) {
	graph_node.ignore_changes(val);
}

void GradientEditor::save_color_state() {
	PoolRealArray p = value.points;
	_saved_points.resize(0);

	for (v in p) {
		_saved_points.push_back(v);
	}

	ignore_changes(true);
}

void GradientEditor::undo_redo_save_color_state() {
	PoolRealArray op = ;
	PoolRealArray np = ;

	for (v in _saved_points) {
		op.push_back(v);
	}

	for (v in value.get_points()) {
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

//func get_gradient_from_data(data):;
//	if typeof(data) == TYPE_ARRAY:;
//		return data;
//	elif typeof(data) == TYPE_DICTIONARY:;
//		if data.has("parameters") and data.parameters.has("gradient"):;
//			return data.parameters.gradient;
//		if data.has("type") and data.type == "Gradient":;
//			return data;
//	return null;
//func get_drag_data(_position : Vector2):;
//	var data = 0//MMType.serialize_value(value);
//	var preview = ColorRect.new();
//	preview.rect_size = Vector2(64, 24);
//	preview.material = $Gradient.material;
//	set_drag_preview(preview);
//	return data;
//;
//func can_drop_data(_position : Vector2, data) -> bool:;
//	return get_gradient_from_data(data) != null;
//;
//func drop_data(_position : Vector2, data) -> void:;
//	var gradient = get_gradient_from_data(data);
//	//if gradient != null:;
//		//set_value(MMType.deserialize_value(gradient));

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

void GradientEditor::_gui_input(const Variant &ev) {
	if (ev is InputEventMouseButton && ev.button_index == 1 && ev.doubleclick) {
		if (ev.position.y > 15) {
			Variant = clamp(ev.position.x, 0, rect_size.x - GradientCursor.WIDTH);
			save_color_state();
			add_cursor(p, get_gradient_color(p));
			update_value();
			undo_redo_save_color_state();
		}

		else if (embedded) {
			Variant = load("res://addons/mat_maker_gd/widgets/gradient_editor/gradient_popup.tscn").instance();
			add_child(popup);
			Variant = popup.rect_size;
			popup.popup(Rect2(ev.global_position, Vector2(0, 0)));
			popup.set_global_position(ev.global_position - Vector2(popup_size.x / 2, popup_size.y));
			popup.init(value, graph_node, _undo_redo);
			popup.connect("updated", self, "set_value");
			popup.connect("popup_hide", popup, "queue_free");
		}

		// Showing a color picker popup to change a cursor's color;
	}
}

Variant;

void GradientEditor::select_color(const Variant &cursor, const Variant &position) {
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

	if (!tex) {
		tex = ImageTexture.new();
		$Gradient.texture = tex;
	}

	Ref<Image> img = tex.get_data();
	float w = $Gradient.rect_size.x;
	float h = $Gradient.rect_size.y;

	if (!img) {
		img = Image.new();
	}

	if (img.get_size().x != w || img.get_size().y != h) {
		img.create(w, h, false, Image.FORMAT_RGBA8);
	}

	img.lock();

	for (int i = 0; i < w; ++i) { //i in range(w)
		float x = float(i) / float(w);
		Color col = value.get_gradient_color(x);

		for (int j = 0; j < h; ++j) { //j in range(h)
			img.set_pixel(i, j, col);
		}
	}

	img.unlock();
	tex.create_from_image(img, 0);
}

Color GradientEditor::get_gradient_color(const Variant &x) {
	return value.get_gradient_color(x / (rect_size.x - GradientCursor.WIDTH));
}

void GradientEditor::update_preview() {
	call_deferred("generate_preview_image");
}

void GradientEditor::_on_Interpolation_item_selected(const Variant &ID) {
	ignore_changes(true);
	_undo_redo.create_action("MMGD: gradient interpolation_type changed");
	_undo_redo.add_do_method(value, "set_interpolation_type", ID);
	_undo_redo.add_undo_method(value, "set_interpolation_type", value.interpolation_type);
	_undo_redo.commit_action();
	ignore_changes(false);
	update_preview();
}

void GradientEditor::on_resized() {
	if (value) {
		update_preview();
		call_deferred("update_cursors");
	}
}

GradientEditor::GradientEditor() {
	= null;
	= null;
	embedded = true;
	*_undo_redo = null;
	_saved_points = PoolRealArray();
	;

	//Script: res://addons/mat_maker_gd/widgets/gradient_editor/gradient_editor.gd
	Control *control = memnew(Control);
	control->set_name("Control");

	control->set_name("Control");
	//control->set("name", Control));

	control->set_filename("res://addons/mat_maker_gd/widgets/gradient_editor/gradient_editor.tscn");
	//control->set("filename", "res://addons/mat_maker_gd/widgets/gradient_editor/gradient_editor.tscn");

	control->set_margin_right(120);
	//control->set("margin_right", 120);

	control->set_margin_bottom(30);
	//control->set("margin_bottom", 30);

	control->set_rect_size(Vector2(120, 30));
	//control->set("rect_size", Vector2(120, 30));

	control->set_rect_min_size(Vector2(120, 32));
	//control->set("rect_min_size", Vector2(120, 32));

	control->set_focus_mode(1);
	//control->set("focus_mode", 1);

	ColorRect *background_control = memnew(ColorRect);
	background_control->set_name("Background");
	control->add_child(background_control);

	background_control->set_name("Background");
	//background_control->set("name", Background));

	//background_control property owner TYPE_OBJECT value: Control:[Control:51414]

	//background_control property material TYPE_OBJECT value: [ShaderMaterial:19166]
	Ref<ShaderMaterial> background_control_prop_material;
	background_control_prop_material.instance();
	background_control->set_material(background_control_prop_material);
	//background_control->set("material", background_control_prop_material);

	background_control->set_anchor_right(1);
	//background_control->set("anchor_right", 1);

	background_control->set_margin_left(4);
	//background_control->set("margin_left", 4);

	background_control->set_margin_right(-4);
	//background_control->set("margin_right", -4);

	background_control->set_margin_bottom(15);
	//background_control->set("margin_bottom", 15);

	background_control->set_rect_position(Vector2(4, 0));
	//background_control->set("rect_position", Vector2(4, 0));

	background_control->set_rect_global_position(Vector2(4, 0));
	//background_control->set("rect_global_position", Vector2(4, 0));

	background_control->set_rect_size(Vector2(0, 15));
	//background_control->set("rect_size", Vector2(0, 15));

	background_control->set_rect_min_size(Vector2(112, 17));
	//background_control->set("rect_min_size", Vector2(112, 17));

	background_control->set_mouse_filter(2);
	//background_control->set("mouse_filter", 2);

	//background_control property __meta__ TYPE_DICTIONARY value: {_edit_use_anchors_:False}

	TextureRect *gradient_control = memnew(TextureRect);
	gradient_control->set_name("Gradient");
	control->add_child(gradient_control);

	gradient_control->set_name("Gradient");
	//gradient_control->set("name", Gradient));

	//gradient_control property owner TYPE_OBJECT value: Control:[Control:51414]

	gradient_control->set_anchor_right(1);
	//gradient_control->set("anchor_right", 1);

	gradient_control->set_margin_left(4);
	//gradient_control->set("margin_left", 4);

	gradient_control->set_margin_right(-4);
	//gradient_control->set("margin_right", -4);

	gradient_control->set_margin_bottom(15);
	//gradient_control->set("margin_bottom", 15);

	gradient_control->set_rect_position(Vector2(4, 0));
	//gradient_control->set("rect_position", Vector2(4, 0));

	gradient_control->set_rect_global_position(Vector2(4, 0));
	//gradient_control->set("rect_global_position", Vector2(4, 0));

	gradient_control->set_rect_size(Vector2(0, 15));
	//gradient_control->set("rect_size", Vector2(0, 15));

	gradient_control->set_rect_min_size(Vector2(112, 17));
	//gradient_control->set("rect_min_size", Vector2(112, 17));

	gradient_control->set_mouse_filter(2);
	//gradient_control->set("mouse_filter", 2);

	//gradient_control property theme TYPE_OBJECT value: [Theme:19167]
	Ref<Theme> gradient_control_prop_theme;
	gradient_control_prop_theme.instance();
	gradient_control->set_theme(gradient_control_prop_theme);
	//gradient_control->set("theme", gradient_control_prop_theme);

	//gradient_control property __meta__ TYPE_DICTIONARY value: {_edit_use_anchors_:False}

	OptionButton *interpolation_control = memnew(OptionButton);
	interpolation_control->set_name("Interpolation");
	control->add_child(interpolation_control);

	interpolation_control->set_name("Interpolation");
	//interpolation_control->set("name", Interpolation));

	//interpolation_control property owner TYPE_OBJECT value: Control:[Control:51414]

	interpolation_control->set_margin_left(0.418457);
	//interpolation_control->set("margin_left", 0.418457);

	interpolation_control->set_margin_top(-2.90374);
	//interpolation_control->set("margin_top", -2.90374);

	interpolation_control->set_margin_right(73.418503);
	//interpolation_control->set("margin_right", 73.418503);

	interpolation_control->set_margin_bottom(19.0963);
	//interpolation_control->set("margin_bottom", 19.0963);

	interpolation_control->set_rect_position(Vector2(0.418457, -2.90374));
	//interpolation_control->set("rect_position", Vector2(0.418457, -2.90374));

	interpolation_control->set_rect_global_position(Vector2(0.418457, -2.90374));
	//interpolation_control->set("rect_global_position", Vector2(0.418457, -2.90374));

	interpolation_control->set_rect_size(Vector2(73.000046, 22.00004));
	//interpolation_control->set("rect_size", Vector2(73.000046, 22.00004));

	interpolation_control->set_rect_scale(Vector2(0.5, 0.5));
	//interpolation_control->set("rect_scale", Vector2(0.5, 0.5));

	//interpolation_control property icon TYPE_OBJECT value: [AtlasTexture:19168]
	Ref<AtlasTexture> interpolation_control_prop_icon;
	interpolation_control_prop_icon.instance();
	interpolation_control->set_icon(interpolation_control_prop_icon);
	//interpolation_control->set("icon", interpolation_control_prop_icon);

	//interpolation_control property items TYPE_ARRAY value: [, [AtlasTexture:19169], False, 0, Null, , [AtlasTexture:19168], False, 1, Null, , [AtlasTexture:19170], False, 2, Null, , [AtlasTexture:19171], False, 3, Null]

	interpolation_control->set_selected(1);
	//interpolation_control->set("selected", 1);

	PopupMenu *popupmenu_interpolation_control = memnew(PopupMenu);
	popupmenu_interpolation_control->set_name("PopupMenu");
	interpolation_control->add_child(popupmenu_interpolation_control);

	popupmenu_interpolation_control->set_name("PopupMenu");
	//popupmenu_interpolation_control->set("name", PopupMenu));

	popupmenu_interpolation_control->set_input_pass_on_modal_close_click(False);
	//popupmenu_interpolation_control->set("input_pass_on_modal_close_click", False);

	//popupmenu_interpolation_control property items TYPE_ARRAY value: [, [AtlasTexture:19169], 2, False, False, 0, 0, Null, , False, , [AtlasTexture:19168], 2, True, False, 1, 0, Null, , False, , [AtlasTexture:19170], 2, False, False, 2, 0, Null, , False, , [AtlasTexture:19171], 2, False, False, 3, 0, Null, , False]

	popupmenu_interpolation_control->set_allow_search(True);
	//popupmenu_interpolation_control->set("allow_search", True);

	Timer *timer_popupmenu_interpolation_control = memnew(Timer);
	timer_popupmenu_interpolation_control->set_name("Timer");
	popupmenu_interpolation_control->add_child(timer_popupmenu_interpolation_control);

	timer_popupmenu_interpolation_control->set_name("Timer");
	//timer_popupmenu_interpolation_control->set("name", Timer));

	timer_popupmenu_interpolation_control->set_wait_time(0.3);
	//timer_popupmenu_interpolation_control->set("wait_time", 0.3);

	timer_popupmenu_interpolation_control->set_one_shot(True);
	//timer_popupmenu_interpolation_control->set("one_shot", True);

	Label *value_control = memnew(Label);
	value_control->set_name("Value");
	control->add_child(value_control);

	value_control->set_name("Value");
	//value_control->set("name", Value));

	//value_control property owner TYPE_OBJECT value: Control:[Control:51414]

	value_control->set_anchor_right(1);
	//value_control->set("anchor_right", 1);

	value_control->set_margin_top(-1);
	//value_control->set("margin_top", -1);

	value_control->set_margin_bottom(14);
	//value_control->set("margin_bottom", 14);

	value_control->set_rect_position(Vector2(0, -1));
	//value_control->set("rect_position", Vector2(0, -1));

	value_control->set_rect_global_position(Vector2(0, -1));
	//value_control->set("rect_global_position", Vector2(0, -1));

	value_control->set_rect_size(Vector2(0, 15));
	//value_control->set("rect_size", Vector2(0, 15));

	value_control->set_custom_colors / font_color(Color(1, 1, 1, 1));
	//value_control->set("custom_colors/font_color", Color(1, 1, 1, 1));

	value_control->set_custom_colors / font_color_shadow(Color(0, 0, 0, 1));
	//value_control->set("custom_colors/font_color_shadow", Color(0, 0, 0, 1));

	value_control->set_custom_constants / shadow_offset_x(1);
	//value_control->set("custom_constants/shadow_offset_x", 1);

	value_control->set_custom_constants / shadow_offset_y(1);
	//value_control->set("custom_constants/shadow_offset_y", 1);

	value_control->set_custom_constants / shadow_as_outline(1);
	//value_control->set("custom_constants/shadow_as_outline", 1);

	value_control->set_align(1);
	//value_control->set("align", 1);

	//value_control property __meta__ TYPE_DICTIONARY value: {_edit_use_anchors_:False}
}

GradientEditor::~GradientEditor() {
}

static void GradientEditor::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_Variant"), &GradientEditor::get_Variant);
	ClassDB::bind_method(D_METHOD("set_Variant", "value"), &GradientEditor::set_Variant);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "Variant", PROPERTY_HINT_RESOURCE_TYPE, "Variant"), "set_Variant", "get_Variant");

	ClassDB::bind_method(D_METHOD("get_Variant"), &GradientEditor::get_Variant);
	ClassDB::bind_method(D_METHOD("set_Variant", "value"), &GradientEditor::set_Variant);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "Variant", PROPERTY_HINT_RESOURCE_TYPE, "Variant"), "set_Variant", "get_Variant");

	ClassDB::bind_method(D_METHOD("get_embedded"), &GradientEditor::get_embedded);
	ClassDB::bind_method(D_METHOD("set_embedded", "value"), &GradientEditor::set_embedded);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "embedded"), "set_embedded", "get_embedded");

	ClassDB::bind_method(D_METHOD("get_*_undo_redo"), &GradientEditor::get_ * _undo_redo);
	ClassDB::bind_method(D_METHOD("set_*_undo_redo", "value"), &GradientEditor::set_ * _undo_redo);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*_undo_redo", PROPERTY_HINT_RESOURCE_TYPE, "UndoRedo"), "set_*_undo_redo", "get_*_undo_redo");

	ClassDB::bind_method(D_METHOD("get__saved_points"), &GradientEditor::get__saved_points);
	ClassDB::bind_method(D_METHOD("set__saved_points", "value"), &GradientEditor::set__saved_points);
	ADD_PROPERTY(PropertyInfo(Variant::POOL_REAL_ARRAY, "_saved_points"), "set__saved_points", "get__saved_points");

	ClassDB::bind_method(D_METHOD("get_Variant"), &GradientEditor::get_Variant);
	ClassDB::bind_method(D_METHOD("set_Variant", "value"), &GradientEditor::set_Variant);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "Variant", PROPERTY_HINT_RESOURCE_TYPE, "Variant"), "set_Variant", "get_Variant");

	ClassDB::bind_method(D_METHOD("_init"), &GradientEditor::_init);
	ClassDB::bind_method(D_METHOD("ignore_changes", "val"), &GradientEditor::ignore_changes);
	ClassDB::bind_method(D_METHOD("save_color_state"), &GradientEditor::save_color_state);
	ClassDB::bind_method(D_METHOD("undo_redo_save_color_state"), &GradientEditor::undo_redo_save_color_state);
	ClassDB::bind_method(D_METHOD("set_undo_redo", "ur"), &GradientEditor::set_undo_redo);
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
