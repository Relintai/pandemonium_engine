#ifndef PAINT_CUSTOM_PROPERTY_INSPECTOR_H
#define PAINT_CUSTOM_PROPERTY_INSPECTOR_H



#include "scene/gui/panel_container.h"

class PaintNode;

class PaintCustomPropertyInspector : public PanelContainer {
	GDCLASS(PaintCustomPropertyInspector, PanelContainer);

public:
	void set_paint_node(PaintNode *paint_node);
	void set_paint_node_bind(Node *paint_node);
	
	virtual void _set_paint_node(Node *paint_node);

	PaintCustomPropertyInspector();
	~PaintCustomPropertyInspector();

protected:
	static void _bind_methods();
};

#endif
