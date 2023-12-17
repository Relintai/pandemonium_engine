#ifndef MDR_UV_RECT_EDITOR_POPUP_H
#define MDR_UV_RECT_EDITOR_POPUP_H



#include "scene/gui/dialogs.h"

class MDRUVRectEditor;
class MDIEdPlugin;

class MDRUVRectEditorPopup : public ConfirmationDialog {
	GDCLASS(MDRUVRectEditorPopup, ConfirmationDialog);

public:
	void on_ok_pressed();
	void on_cancel_pressed();

	MDRUVRectEditor *get_editor();

	void set_plugin(MDIEdPlugin *plugin);

	MDRUVRectEditorPopup();
	~MDRUVRectEditorPopup();

protected:
	void _notification(int p_what);
	static void _bind_methods();

	MDRUVRectEditor *_editor;
};

#endif
