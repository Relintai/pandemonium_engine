
#include "gradient_popup.h"



 //tool;
 signal updated(value);

 void GradientPopup::init(const Variant &value, const Variant & graph_node, const Variant & undo_redo) {
  $Panel/Control.set_undo_redo(undo_redo);
  $Panel/Control.graph_node = graph_node;
  $Panel/Control.set_value(value);
}


 void GradientPopup::_on_Control_updated(const Variant &value) {
  emit_signal("updated", value);
}


 void GradientPopup::_on_GradientPopup_popup_hide() {
  queue_free();
}

}

 GradientPopup::GradientPopup() {
 }

 GradientPopup::~GradientPopup() {
 }


 static void GradientPopup::_bind_methods() {
  ClassDB::bind_method(D_METHOD("init", "value", " graph_node", " undo_redo"), &GradientPopup::init);
  ClassDB::bind_method(D_METHOD("_on_Control_updated", "value"), &GradientPopup::_on_Control_updated);
  ClassDB::bind_method(D_METHOD("_on_GradientPopup_popup_hide"), &GradientPopup::_on_GradientPopup_popup_hide);

 }



