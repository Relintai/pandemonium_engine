#ifndef POLYGON_DIALOG_H
#define POLYGON_DIALOG_H

#include "core/object/reference.h"

#include "scene/gui/dialogs.h"

class Button;
class PolygonEditor;
class PolygonBase;

class PolygonDialog : public WindowDialog {
	GDCLASS(PolygonDialog, WindowDialog);

public:
	bool get_closed() const;
	void set_closed(const bool c = true);

	PoolVector2Array get_previous_points();
	void set_previous_points(const PoolVector2Array &val);

	Ref<PolygonBase> get_polygon();
	void set_polygon(const Ref<PolygonBase> &val);
	void edit_polygon(const Ref<PolygonBase> &poly);

	void _on_CurveDialog_popup_hide();
	void _on_OK_pressed();
	void _on_Cancel_pressed();
	void _on_PolygonEditor_value_changed(const Variant &value);

	PolygonDialog();
	~PolygonDialog();

protected:
	void _notification(int p_what);

	static void _bind_methods();

	bool closed;
	PoolVector2Array previous_points;
	Ref<PolygonBase> polygon;

	PolygonEditor *_polygon_editor;
	Button *_ok_button;
	Button *_cancel_button;
};

#endif
