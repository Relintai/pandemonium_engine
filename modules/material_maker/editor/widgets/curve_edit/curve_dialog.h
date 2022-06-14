#ifndef CURVE_DIALOG_H
#define CURVE_DIALOG_H

#include "core/reference.h"

#include "scene/gui/dialogs.h"

#include "../../../nodes/bases/curve_base.h"

class Button;
class MMCurveEditor;

class CurveDialog : public WindowDialog {
	GDCLASS(CurveDialog, WindowDialog);

public:
	Ref<CurveBase> get_curve();
	void set_curve(const Ref<CurveBase> &val);

	Vector<CurveBase::Point> get_previous_points();
	void set_previous_points(const Vector<CurveBase::Point> &val);

	void edit_curve(const Ref<CurveBase> &c);

	void _on_CurveDialog_popup_hide();
	void _on_OK_pressed();
	void _on_Cancel_pressed();
	void _on_MMCurveEditor_value_changed(const Ref<CurveBase> &value);

	CurveDialog();
	~CurveDialog();

protected:
	void _notification(int p_what);

	static void _bind_methods();

	Vector<CurveBase::Point> previous_points;
	Ref<CurveBase> curve;

	MMCurveEditor *_curve_editor;
	Button *_ok_button;
	Button *_cancel_button;
};

#endif
