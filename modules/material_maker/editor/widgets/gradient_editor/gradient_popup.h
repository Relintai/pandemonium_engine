#ifndef GRADIENT_POPUP_H
#define GRADIENT_POPUP_H


class GradientPopup : public Popup {
 GDCLASS(GradientPopup, Popup);

 public:

 void init(const Variant &value, const Variant & graph_node, const Variant & undo_redo);
 void _on_Control_updated(const Variant &value);
 void _on_GradientPopup_popup_hide();

 GradientPopup();
 ~GradientPopup();

 protected:
 static void _bind_methods();

 //tool
 signal updated(value);
};


#endif
