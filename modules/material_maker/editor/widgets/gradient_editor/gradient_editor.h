#ifndef GRADIENT_EDITOR_H
#define GRADIENT_EDITOR_H


class GradientEditor : public Control {
 GDCLASS(GradientEditor, Control);

 public:

 Variant get_Variant();
 void set_Variant(const Variant &val);

 Variant get_Variant();
 void set_Variant(const Variant &val);

 bool get_embedded() const;
 void set_embedded(const bool val);

 UndoRedo get_*_undo_redo();
 void set_*_undo_redo(const UndoRedo &val);

 PoolRealArray get__saved_points();
 void set__saved_points(const PoolRealArray &val);

 Variant get_Variant();
 void set_Variant(const Variant &val);

 class GradientCursor {
  public:

  Color get_color();
  void set_color(const Color &val);

  bool get_sliding() const;
  void set_sliding(const bool val);

  void _ready();
  void _draw();
  void _gui_input(const Variant &ev);
  float get_cursor_position();
  void set_color(const Variant &c);
  static bool sort(const Variant &a, const Variant & b);
  bool can_drop_data(const Variant &_position, const Variant & data);
  void drop_data(const Variant &_position, const Variant & data);

  GradientCursor();
  ~GradientCursor();

  protected:
  static void _bind_methods();

  extends Control;
  Color color = ;
  bool sliding = false;
  onready var label : Label = get_parent().get_node("Value");
  const WIDTH : int = 10;
};

 void _init();
 void ignore_changes(const Variant &val);
 void save_color_state();
 void undo_redo_save_color_state();
 void set_undo_redo(const UndoRedo &ur);
 void set_value(const Variant &v);
 void update_cursors();
 void update_value();
 void add_cursor(const Variant &x, const Variant & color);
 void _gui_input(const Variant &ev);
 void select_color(const Variant &cursor, const Variant & position);
 Array get_sorted_cursors();
 void generate_preview_image();
 Color get_gradient_color(const Variant &x);
 void update_preview();
 void _on_Interpolation_item_selected(const Variant &ID);
 void on_resized();

 GradientEditor();
 ~GradientEditor();

 protected:
 static void _bind_methods();

 //tool
 Variant  = null;
 // setget set_value
 Variant  = null;
 //export 
 bool embedded = true;
 UndoRedo *_undo_redo = null;
 signal updated(value);
 PoolRealArray _saved_points = PoolRealArray();
 //func get_gradient_from_data(data):
 //	if typeof(data) == TYPE_ARRAY:
 //		return data
 //	elif typeof(data) == TYPE_DICTIONARY:
 //		if data.has("parameters") and data.parameters.has("gradient"):
 //			return data.parameters.gradient
 //		if data.has("type") and data.type == "Gradient":
 //			return data
 //	return null
 //func get_drag_data(_position : Vector2):
 //	var data = 0//MMType.serialize_value(value)
 //	var preview = ColorRect.new()
 //	preview.rect_size = Vector2(64, 24)
 //	preview.material = $Gradient.material
 //	set_drag_preview(preview)
 //	return data
 //
 //func can_drop_data(_position : Vector2, data) -> bool:
 //	return get_gradient_from_data(data) != null
 //
 //func drop_data(_position : Vector2, data) -> void:
 //	var gradient = get_gradient_from_data(data)
 //	//if gradient != null:
 //		//set_value(MMType.deserialize_value(gradient))
 Variant ;
 // Calculating a color from the gradient and generating the shader
};


#endif
