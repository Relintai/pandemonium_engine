#ifndef GRADIENT_POPUP_H
#define GRADIENT_POPUP_H

#include "core/reference.h"

#include "scene/gui/popup.h"

class GradientBase;
class UndoRedo;
class MMGraphNode;
class GradientEditor;

class GradientPopup : public Popup {
	GDCLASS(GradientPopup, Popup);

public:
	void init(const Ref<GradientBase> &value, MMGraphNode *graph_node, UndoRedo *undo_redo);
	void _on_Control_updated(const Ref<GradientBase> &value);
	void _on_GradientPopup_popup_hide();

	GradientPopup();
	~GradientPopup();

protected:
	static void _bind_methods();

	GradientEditor *_gradient_editor;
};

#endif
