#ifndef COLOR_PICKER_BUTTON_H
#define COLOR_PICKER_BUTTON_H


class ColorPickerButton : public ColorPickerButton {
 GDCLASS(ColorPickerButton, ColorPickerButton);

 public:

 void get_drag_data(const Variant &_position);
 bool can_drop_data(const Variant &_position, const Variant & data);
 void drop_data(const Variant &_position, const Variant & data);

 ColorPickerButton();
 ~ColorPickerButton();

 protected:
 static void _bind_methods();

 //tool
};


#endif
