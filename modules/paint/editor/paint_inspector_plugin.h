#ifndef PAINT_INSPECTOR_PLUGIN_H
#define PAINT_INSPECTOR_PLUGIN_H



#include "core/object/reference.h"
#include "editor/editor_inspector.h"

class PaintInspectorPlugin : public EditorInspectorPlugin {
	GDCLASS(PaintInspectorPlugin, EditorInspectorPlugin);

public:
	bool can_handle(Object *p_object);
	void parse_begin(Object *p_object);

	PaintInspectorPlugin();
	~PaintInspectorPlugin();

protected:
	static void _bind_methods();
};

#endif
