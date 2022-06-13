#ifndef CURVE_EDIT_H
#define CURVE_EDIT_H

#include "scene/gui/control.h"

class CurveView;

class CurveEdit : public Control {
	GDCLASS(CurveEdit, Control);

public:
	Variant get_value();
	void set_value(const Variant &v);

	void _on_CurveEdit_pressed();
	void on_value_changed(const Variant &v);

	CurveEdit();
	~CurveEdit();

protected:
	static void _bind_methods();

	Variant value;

	CurveView *_curve_view;
};

#endif
