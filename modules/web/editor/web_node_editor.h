#ifndef WEB_NODE_EDITOR_H
#define WEB_NODE_EDITOR_H

/*
Copyright (c) 2022 Péter Magyar

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "core/object/reference.h"

#include "scene/gui/box_container.h"

class WebNode;
class WebNodeEditorWebServer;
class RichTextLabel;
class TextEdit;

class WebNodeEditor : public VBoxContainer {
	GDCLASS(WebNodeEditor, VBoxContainer);

public:
	void edit(WebNode *web_node);
	void refresh();
	void clear();

	WebNodeEditor();
	~WebNodeEditor();

protected:
	void _notification(int p_what);
	static void _bind_methods();

	bool _prettify_html;

	WebNode *_edited_node;
	WebNodeEditorWebServer *_web_server;
	HBoxContainer *_toolbar;

	RichTextLabel *_result_info_label;
	TextEdit *_results_label;
};

#endif
