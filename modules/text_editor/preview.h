#ifndef PREVIEW_H
#define PREVIEW_H

#include "core/ustring.h"

#include "scene/gui/dialogs.h"

class RichTextLabel;
class GridContainer;

class Preview : public WindowDialog {
	GDCLASS(Preview, WindowDialog);

public:
	void print_preview(const String &content);
	void print_bb(const String &content);
	void print_markdown(const String &content);
	void print_html(const String &content);
	void print_csv(const Array &rows);
	void _on_Preview_popup_hide();

	Preview();
	~Preview();

protected:
	static void _bind_methods();

	RichTextLabel *text_preview;
	GridContainer *table_preview;
};

#endif
