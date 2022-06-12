#ifndef CURVE_EDITOR_H
#define CURVE_EDITOR_H

#include "curve_view.h"

class CurveEditor : public CurveView {
	GDCLASS(CurveEditor, CurveView);

public:
	void set_curve(const Variant &c);
	void update_controls();

	void _on_ControlPoint_moved(const Variant &index);
	void _on_ControlPoint_removed(const Variant &index);
	void _on_CurveEditor_gui_input(const Variant &event);
	void _on_resize();

	CurveEditor();
	~CurveEditor();

protected:
	void _notification(int p_what);

	static void _bind_methods();
};

#endif
