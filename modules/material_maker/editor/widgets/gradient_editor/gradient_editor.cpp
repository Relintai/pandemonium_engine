
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


UndoRedo GradientEditor::get_*_undo_redo() {
 return *_undo_redo;
}

void GradientEditor::set_*_undo_redo(const UndoRedo &val) {
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

  Color GradientCursor::get_color() {
  return color;
 }

 void GradientCursor::set_color(const Color &val) {
 color = val;
 }


 bool GradientCursor::get_sliding() const {
  return sliding;
 }

 void GradientCursor::set_sliding(const bool val) {
 sliding = val;
 }



  extends Control;
   Color color = ;
   bool sliding = false;
  onready var label : Label = get_parent().get_node("Value");
  const WIDTH : int = 10;

  void GradientCursor::_ready() {
   rect_position = Vector2(0, 15);
   rect_size = Vector2(WIDTH, 15);
}


  void GradientCursor::_draw() {
   // warning-ignore:integer_division;
    PoolVector2Array polygon = PoolVector2Array([Vector2(0, 5), Vector2(WIDTH/2, 0), Vector2(WIDTH, 5), Vector2(WIDTH, 15), Vector2(0, 15), Vector2(0, 5)]);
    Variant  = color;
   c.a = 1.0;
   draw_colored_polygon(polygon, c);
   draw_polyline(polygon, Color(0.0, 0.0, 0.0) if color.v > 0.5 else Color(1.0, 1.0, 1.0));
}


  void GradientCursor::_gui_input(const Variant &ev) {

   if (ev is InputEventMouseButton) {

    if (ev.button_index == BUTTON_LEFT) {

     if (ev.doubleclick) {
      get_parent().save_color_state();
      get_parent().select_color(self, ev.global_position);
}


     else if (ev.pressed) {
      get_parent().save_color_state();
      sliding = true;
      label.visible = true;
      label.text = "%.03f" % get_cursor_position();
}


     else {

      if (sliding) {
       get_parent().undo_redo_save_color_state();
}

      sliding = false;
      label.visible = false;
}

}


    else if (ev.button_index == BUTTON_RIGHT && get_parent().get_sorted_cursors().size() > 2) {
      Variant  = get_parent();
     parent.save_color_state();
     parent.remove_child(self);
     parent.update_value();
     parent.undo_redo_save_color_state();
     queue_free();
}

}


   else if (ev is InputEventMouseMotion && (ev.button_mask & BUTTON_MASK_LEFT) != 0 && sliding) {
    rect_position.x += get_local_mouse_position().x;

    if (ev.control) {
     rect_position.x = round(get_cursor_position()*20.0)*0.05*(get_parent().rect_size.x - WIDTH);
}

    rect_position.x = min(max(0, rect_position.x), get_parent().rect_size.x-rect_size.x);
    get_parent().update_value();
    label.text = "%.03f" % get_cursor_position();
}

}


  float GradientCursor::get_cursor_position() {
   return rect_position.x / (get_parent().rect_size.x - WIDTH);
}


  void GradientCursor::set_color(const Variant &c) {
   color = c;
   get_parent().update_value();
   update();
}


  bool GradientCursor::sort(const Variant &a, const Variant & b) {
   return a.get_position() < b.get_position();
}


  bool GradientCursor::can_drop_data(const Variant &_position, const Variant & data) {
   return typeof(data) == TYPE_COLOR;
}


  void GradientCursor::drop_data(const Variant &_position, const Variant & data) {
   set_color(data);
}

}

  GradientCursor::GradientCursor() {
   color = ;
   sliding = false;
  }

  GradientCursor::~GradientCursor() {
  }


  static void GradientCursor::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_color"), &GradientCursor::get_color);
    ClassDB::bind_method(D_METHOD("set_color", "value"), &GradientCursor::set_color);
    ADD_PROPERTY(PropertyInfo(Variant::COLOR, "color"), "set_color", "get_color");


    ClassDB::bind_method(D_METHOD("get_sliding"), &GradientCursor::get_sliding);
    ClassDB::bind_method(D_METHOD("set_sliding", "value"), &GradientCursor::set_sliding);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "sliding"), "set_sliding", "get_sliding");


   ClassDB::bind_method(D_METHOD("_ready"), &GradientCursor::_ready);
   ClassDB::bind_method(D_METHOD("_draw"), &GradientCursor::_draw);
   ClassDB::bind_method(D_METHOD("_gui_input", "ev"), &GradientCursor::_gui_input);
   ClassDB::bind_method(D_METHOD("get_cursor_position"), &GradientCursor::get_cursor_position);
   ClassDB::bind_method(D_METHOD("set_color", "c"), &GradientCursor::set_color);
   ClassDB::bind_method(D_METHOD("can_drop_data", "_position", " data"), &GradientCursor::can_drop_data);
   ClassDB::bind_method(D_METHOD("drop_data", "_position", " data"), &GradientCursor::drop_data);

  }


  Variant  = null;
 // setget set_value;
  Variant  = null;
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
   add_cursor(value.get_point_value(i) * (rect_size.x-GradientCursor.WIDTH), value.get_point_color(i));
}

  $Interpolation.selected = value.interpolation_type;
}


 void GradientEditor::update_value() {
  value.clear();
   Array sc = get_sorted_cursors();
   PoolRealArray points = PoolRealArray();

  for (c in sc) {
   points.push_back(c.rect_position.x/(rect_size.x-GradientCursor.WIDTH));
    Color color = c.color;
   points.push_back(color.r);
   points.push_back(color.g);
   points.push_back(color.b);
   points.push_back(color.a);
}

  value.set_points(points);
  update_preview();
}


 void GradientEditor::add_cursor(const Variant &x, const Variant & color) {
   Variant  = GradientCursor.new();
  add_child(cursor);
  cursor.rect_position.x = x;
  cursor.color = color;
}


 void GradientEditor::_gui_input(const Variant &ev) {

  if (ev is InputEventMouseButton && ev.button_index == 1 && ev.doubleclick) {

   if (ev.position.y > 15) {
     Variant  = clamp(ev.position.x, 0, rect_size.x-GradientCursor.WIDTH);
    save_color_state();
    add_cursor(p, get_gradient_color(p));
    update_value();
    undo_redo_save_color_state();
}


   else if (embedded) {
     Variant  = load("res://addons/mat_maker_gd/widgets/gradient_editor/gradient_popup.tscn").instance();
    add_child(popup);
     Variant  = popup.rect_size;
    popup.popup(Rect2(ev.global_position, Vector2(0, 0)));
    popup.set_global_position(ev.global_position-Vector2(popup_size.x / 2, popup_size.y));
    popup.init(value, graph_node, _undo_redo);
    popup.connect("updated", self, "set_value");
    popup.connect("popup_hide", popup, "queue_free");
}

   // Showing a color picker popup to change a cursor's color;
}

}

  Variant ;

 void GradientEditor::select_color(const Variant &cursor, const Variant & position) {
  active_cursor = cursor;
  //var color_picker_popup = preload("res://addons/mat_maker_gd/widgets/color_picker_popup/color_picker_popup.tscn").instance();
  add_child(color_picker_popup);
   Variant  = color_picker_popup.get_node("ColorPicker");
  color_picker.color = cursor.color;
  color_picker.connect("color_changed", cursor, "set_color");
  color_picker_popup.rect_position = position;
  color_picker_popup.connect("popup_hide", self, "undo_redo_save_color_state");
  color_picker_popup.connect("popup_hide", color_picker_popup, "queue_free");
  color_picker_popup.popup();
}

 // Calculating a color from the gradient and generating the shader;

 Array GradientEditor::get_sorted_cursors() {
   Variant  = [];

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

}

 GradientEditor::GradientEditor() {
   = null;
   = null;
  embedded = true;
  *_undo_redo = null;
  _saved_points = PoolRealArray();
  ;
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


   ClassDB::bind_method(D_METHOD("get_*_undo_redo"), &GradientEditor::get_*_undo_redo);
   ClassDB::bind_method(D_METHOD("set_*_undo_redo", "value"), &GradientEditor::set_*_undo_redo);
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



