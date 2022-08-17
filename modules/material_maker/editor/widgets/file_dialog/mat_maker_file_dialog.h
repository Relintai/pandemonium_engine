#ifndef MAT_MAKER_FILE_DIALOG_H
#define MAT_MAKER_FILE_DIALOG_H

#include "core/ustring.h"
#include "core/variant/variant.h"

#include "scene/gui/file_dialog.h"

class LeftPanel;
class OptionButton;

class MatMakerFileDialog : public FileDialog {
	GDCLASS(MatMakerFileDialog, FileDialog);

public:
	String get_full_current_dir();

	void _on_FileDialog_file_selected(const String &path);
	void _on_FileDialog_files_selected(const PoolStringArray &paths);
	void _on_FileDialog_dir_selected(const String &dir);
	void _on_FileDialog_popup_hide();

	void select_files();
	void add_favorite();

	MatMakerFileDialog();
	~MatMakerFileDialog();

protected:
	void _notification(int p_what);

	static void _bind_methods();

	LeftPanel *_left_panel;
	OptionButton *_volume_option;
};

#endif
