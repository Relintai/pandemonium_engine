#ifndef TEXT_EDITOR_PREVIEW_H
#define TEXT_EDITOR_PREVIEW_H

#include "core/ustring.h"

#include "scene/gui/dialogs.h"

class RichTextLabel;
class GridContainer;

class TextEditorPreview : public WindowDialog {
	GDCLASS(TextEditorPreview, WindowDialog);

public:
	void print_preview(const String &content);
	void print_bb(const String &content);
	void print_markdown(const String &content);
	void print_html(const String &content);
	void print_csv(const Array &rows);
	void _on_TextEditorPreview_popup_hide();

	TextEditorPreview();
	~TextEditorPreview();

protected:
	void _notification(int p_what);

	static void _bind_methods();

	RichTextLabel *text_preview;
	GridContainer *table_preview;
};

#endif
