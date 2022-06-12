#ifndef CURVE_VIEW_H
#define CURVE_VIEW_H

#include "core/reference.h"

#include "scene/gui/control.h"

class CurveBase;

class CurveView : public Control {
	GDCLASS(CurveView, Control);

public:
	bool get_show_axes() const;
	void set_show_axes(const bool val);

	Ref<CurveBase> get_curve();
	void set_curve(const Ref<CurveBase> &val);

	Vector2 transform_point(const Vector2 &p);
	Vector2 reverse_transform_point(const Vector2 &p);

	void _draw();
	void _on_resize();

	CurveView();
	~CurveView();

protected:
	void _notification(int p_what);

	static void _bind_methods();

	bool show_axes = false;
	Ref<CurveBase> curve;
};

#endif
