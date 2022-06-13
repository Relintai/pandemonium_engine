#ifndef CURVE_EDIT_H
#define CURVE_EDIT_H

#include "scene/gui/control.h"

class CurveView;
class CurveBase;

class CurveEdit : public Control {
	GDCLASS(CurveEdit, Control);

public:
	Ref<CurveBase> get_value();
	void set_value(const Ref<CurveBase> &v);

	void _on_CurveEdit_pressed();
	void on_value_changed(const Ref<CurveBase> &v);

	CurveEdit();
	~CurveEdit();

protected:
	static void _bind_methods();

	Ref<CurveBase> value;

	CurveView *_curve_view;
};

#endif
