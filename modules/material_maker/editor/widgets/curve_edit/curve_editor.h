#ifndef MM_CURVE_EDITOR_H
#define MM_CURVE_EDITOR_H

#include "curve_view.h"

class MMCurveEditor : public CurveView {
	GDCLASS(MMCurveEditor, CurveView);

public:
	void set_curve(const Variant &c);
	void update_controls();

	void _on_ControlPoint_moved(const Variant &index);
	void _on_ControlPoint_removed(const Variant &index);
	void _on_MMCurveEditor_gui_input(const Variant &event);
	void _on_resize();

	MMCurveEditor();
	~MMCurveEditor();

protected:
	void _notification(int p_what);

	static void _bind_methods();
};

#endif
