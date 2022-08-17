
#include "gradient_popup.h"

#include "core/object/object.h"
#include "gradient_editor.h"

#include "../../../nodes/bases/gradient_base.h"
#include "../../../nodes/mm_node_universal_property.h"
#include "scene/gui/panel.h"

void GradientPopup::init(const Ref<GradientBase> &value, MMGraphNode *graph_node, UndoRedo *undo_redo) {
	_gradient_editor->set_undo_redo(undo_redo);
	_gradient_editor->set_graph_node(graph_node);
	_gradient_editor->set_value(value);
}

void GradientPopup::_on_Control_updated(const Ref<GradientBase> &value) {
	emit_signal("updated", value);
}

void GradientPopup::_on_GradientPopup_popup_hide() {
	queue_delete();
}

GradientPopup::GradientPopup() {
	set_h_size_flags(0);
	set_v_size_flags(0);

	Panel *panel_gradientpopup = memnew(Panel);
	add_child(panel_gradientpopup);

	Ref<StyleBoxFlat> panel_gradientpopup_prop_custom_styles;
	panel_gradientpopup_prop_custom_styles.instance();
	panel_gradientpopup->set("custom_styles/panel", panel_gradientpopup_prop_custom_styles);

	_gradient_editor = memnew(MMGradientEditor);
	panel_gradientpopup->add_child(_gradient_editor);
}

GradientPopup::~GradientPopup() {
}

void GradientPopup::_bind_methods() {
	ADD_SIGNAL(MethodInfo("updated", PropertyInfo(Variant::OBJECT, "value", PROPERTY_HINT_RESOURCE_TYPE, "GradientBase")));

	//ClassDB::bind_method(D_METHOD("init", "value", " graph_node", " undo_redo"), &GradientPopup::init);
	ClassDB::bind_method(D_METHOD("_on_Control_updated", "value"), &GradientPopup::_on_Control_updated);
	ClassDB::bind_method(D_METHOD("_on_GradientPopup_popup_hide"), &GradientPopup::_on_GradientPopup_popup_hide);
}
