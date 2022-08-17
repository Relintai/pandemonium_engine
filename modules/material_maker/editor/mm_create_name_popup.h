#ifndef MM_CREATE_NAME_POPUP_H
#define MM_CREATE_NAME_POPUP_H

#include "core/math/vector2.h"
#include "core/string/node_path.h"
#include "core/object/reference.h"
#include "core/object/undo_redo.h"
#include "core/string/ustring.h"
#include "core/containers/vector.h"

#include "scene/gui/dialogs.h"

class Tree;

class MMCreateNamePopup : public ConfirmationDialog {
	GDCLASS(MMCreateNamePopup, ConfirmationDialog);

public:
	enum OKPressedType {
		OK_PRESSED_TYPE_CLASS = 0,
		OK_PRESSED_TYPE_FILE,
	};

	MMCreateNamePopup();
	~MMCreateNamePopup();

protected:
	void _notification(int p_what);

	void about_to_show();
	void _on_OK_pressed();

	static void _bind_methods();

	Tree *_tree;
	bool _initialized;
};

VARIANT_ENUM_CAST(MMCreateNamePopup::OKPressedType);

#endif
