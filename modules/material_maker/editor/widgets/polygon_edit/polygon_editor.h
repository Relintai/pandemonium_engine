#ifndef POLYGON_EDITOR_H
#define POLYGON_EDITOR_H

#include "core/input/input_event.h"
#include "core/object/reference.h"

#include "polygon_view.h"

class PolygonBase;

class PolygonEditor : public PolygonView {
	GDCLASS(PolygonEditor, PolygonView);

public:
	void set_polygon(const Ref<PolygonBase> &p);
	void update_controls();

	void _on_ControlPoint_moved(const int index);
	void _on_ControlPoint_removed(const int index);
	void _on_PolygonEditor_gui_input(const Ref<InputEvent> &event);
	void _on_resize();

	PolygonEditor();
	~PolygonEditor();

protected:
	void _notification(int p_what);

	static void _bind_methods();
};

#endif
