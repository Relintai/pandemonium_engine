#ifndef POLYGON_EDIT_H
#define POLYGON_EDIT_H

#include "core/object/reference.h"

#include "scene/gui/button.h"

class PolygonBase;
class PolygonView;

class PolygonEdit : public Button {
	GDCLASS(PolygonEdit, Button);

public:
	bool get_closed() const;
	void set_closed(const bool c = true);

	Ref<PolygonBase> get_value();
	void set_value(const Ref<PolygonBase> &v);

	void _on_PolygonEdit_pressed();
	void on_value_changed(const Variant &v);

	PolygonEdit();
	~PolygonEdit();

protected:
	void _notification(int p_what);

	static void _bind_methods();

	bool closed;
	Ref<PolygonBase> value;

	PolygonView *_polygon_view;
};

#endif
