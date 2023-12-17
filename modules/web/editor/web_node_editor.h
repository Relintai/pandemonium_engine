#ifndef WEB_NODE_EDITOR_H
#define WEB_NODE_EDITOR_H



#include "core/object/reference.h"

#include "scene/gui/box_container.h"

class WebNode;
class WebNodeEditorWebServer;
class RichTextLabel;
class TextEdit;
class MarginContainer;
class Button;
class ButtonGroup;

class WebNodeEditor : public VBoxContainer {
	GDCLASS(WebNodeEditor, VBoxContainer);

public:
	void add_control_to_tool_bar(Control *control);
	void remove_control_from_tool_bar(Control *control);

	void add_main_screen_tab(Control *control);
	void remove_main_screen_tab(Control *control);
	void switch_to_main_screen_tab(Control *control);

	Ref<ButtonGroup> get_main_button_group();

	void edit(WebNode *web_node);
	void refresh_html_preview();
	void clear();

	static WebNodeEditor *get_singleton() { return _singleton; }

	WebNodeEditor();
	~WebNodeEditor();

protected:
	void _on_html_previewer_tool_button_toggled(bool on);
	void _on_html_previewer_visibility_changed();

	void _notification(int p_what);

	static void _bind_methods();
	bool _prettify_html;

	WebNode *_edited_node;
	WebNodeEditorWebServer *_web_server;
	HBoxContainer *_toolbar;
	MarginContainer *_main_container;

	Ref<ButtonGroup> _main_button_group;
	Button *_html_previewer_tool_button;
	VBoxContainer *_html_previewer;
	RichTextLabel *_result_info_label;
	TextEdit *_results_label;

private:
	static WebNodeEditor *_singleton;
};

#endif
