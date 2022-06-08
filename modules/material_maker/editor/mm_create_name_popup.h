#ifndef MM_CREATE_NAME_POPUP_H
#define MM_CREATE_NAME_POPUP_H

#include "core/math/vector2.h"
#include "core/node_path.h"
#include "core/reference.h"
#include "core/undo_redo.h"
#include "core/ustring.h"
#include "core/vector.h"

#include "scene/gui/dialogs.h"

class MMCreateNamePopup : public ConfirmationDialog {
	GDCLASS(MMCreateNamePopup, ConfirmationDialog);

public:
	NodePath get_line_edit_path();
	void set_line_edit_path(const NodePath &val);

	NodePath get_tree_path();
	void set_tree_path(const NodePath &val);

	PoolStringArray get_type_folders();
	void set_type_folders(const PoolStringArray &val);

	String get_resource_type();
	void set_resource_type(const String &val);

	LineEdit *get_line_edit();
	void set_line_edit(const LineEdit *val);

	Tree *get_tree();
	void set_tree(Tree *val);

	void _ready();
	void set_resource_type(const String &resource_type);
	void about_to_show();
	void evaluate_folder(const String &folder, TreeItem *root);
	void _on_OK_pressed();

	MMCreateNamePopup();
	~MMCreateNamePopup();

protected:
	static void _bind_methods();

	//export(NodePath)
	NodePath line_edit_path;
	//export(NodePath)
	NodePath tree_path;
	//export(PoolStringArray)
	PoolStringArray type_folders = ;
	String _resource_type = "MMNode";
	LineEdit *_line_edit;
	Tree *_tree;
};

#endif
