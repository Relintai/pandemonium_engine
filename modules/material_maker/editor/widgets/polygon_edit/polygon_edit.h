#ifndef POLYGON_EDIT_H
#define POLYGON_EDIT_H

#include "core/reference.h"

#include "scene/gui/control.h"

class PolygonBase;
class PolygonView;

class PolygonEdit : public Control {
	GDCLASS(PolygonEdit, Control);

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
	static void _bind_methods();

	bool closed;
	Ref<PolygonBase> value;

	PolygonView *_polygon_view;
};

#endif
