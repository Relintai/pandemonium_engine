
#include "text_editor_preview.h"

#include "core/variant.h"
#include "scene/gui/box_container.h"
#include "scene/gui/grid_container.h"
#include "scene/gui/rich_text_label.h"

#include "modules/regex/regex.h"

void TextEditorPreview::print_preview(const String &content) {
	text_preview->append_bbcode(content);
	text_preview->show();
}

void TextEditorPreview::print_bb(const String &content) {
	text_preview->append_bbcode(content);
	text_preview->show();
}

void TextEditorPreview::print_markdown(const String &p_content) {
	String content = p_content;

	Array result;

	PoolStringArray bolded;
	PoolStringArray italics;
	PoolStringArray striked;
	PoolStringArray coded;
	PoolStringArray linknames;
	PoolStringArray images;
	PoolStringArray links;
	PoolStringArray lists;
	PoolStringArray underlined;

	Ref<RegEx> regex;
	regex.instance();

	regex->compile("\\*\\*(?<boldtext>.*)\\*\\*");
	result = regex->search_all(content);

	for (int i = 0; i < result.size(); ++i) { //i in range(result.size())
		Ref<RegExMatch> res = result[i];
		bolded.append(res->get_string("boldtext"));
	}

	regex->compile("\\_\\_(?<underlinetext>.*)\\_\\_");
	result = regex->search_all(content);

	for (int i = 0; i < result.size(); ++i) { //i in range(result.size())
		Ref<RegExMatch> res = result[i];
		underlined.append(res->get_string("underlinetext"));
	}

	regex->compile("\\*(?<italictext>.*)\\*");
	result = regex->search_all(content);

	for (int i = 0; i < result.size(); ++i) { //i in range(result.size())
		Ref<RegExMatch> res = result[i];
		italics.append(res->get_string("italictext"));
	}

	regex->compile("~~(?<strikedtext>.*)~~");
	result = regex->search_all(content);

	for (int i = 0; i < result.size(); ++i) { //i in range(result.size())
		Ref<RegExMatch> res = result[i];
		striked.append(res->get_string("strikedtext"));
	}

	regex->compile("`(?<coded>.*)`");
	result = regex->search_all(content);

	for (int i = 0; i < result.size(); ++i) { //i in range(result.size())
		Ref<RegExMatch> res = result[i];
		coded.append(res->get_string("coded"));
	}

	regex->compile("[+-*](?<element>\\s.*)");
	result = regex->search_all(content);

	for (int i = 0; i < result.size(); ++i) { //i in range(result.size())
		Ref<RegExMatch> res = result[i];
		lists.append(res->get_string("element"));
	}

	regex->compile("(?<img>!\\[.*?\\))");
	result = regex->search_all(content);

	for (int i = 0; i < result.size(); ++i) { //i in range(result.size())
		Ref<RegExMatch> res = result[i];
		images.append(res->get_string("img"));
	}

	regex->compile("\\[(?<linkname>.*?)\\]|\\((?<link>[h\\.]\\S*?)\\)");
	result = regex->search_all(content);

	for (int i = 0; i < result.size(); ++i) { //i in range(result.size())
		Ref<RegExMatch> res = result[i];

		if (res->get_string("link") != "") {
			links.append(res->get_string("link"));
		}

		if (res->get_string("linkname") != "") {
			linknames.append(res->get_string("linkname"));
		}
	}

	for (int i = 0; i < bolded.size(); ++i) { //i in range(bolded.size())
		String bold = bolded[i];
		content = content.replace("**" + bold + "**", "[b]" + bold + "[/b]");
	}

	for (int i = 0; i < italics.size(); ++i) { //i in range(italics.size())
		String italic = italics[i];
		content = content.replace("*" + italic + "*", "[i]" + italic + "[/i]");
	}

	for (int i = 0; i < striked.size(); ++i) { //i in range(striked.size())
		String strik = striked[i];
		content = content.replace("~~" + strik + "~~", "[s]" + strik + "[/s]");
	}

	for (int i = 0; i < underlined.size(); ++i) { //i in range(underlined.size())
		String underline = underlined[i];
		content = content.replace("__" + underline + "__", "[u]" + underline + "[/u]");
	}

	for (int i = 0; i < coded.size(); ++i) { //i in range(coded.size())
		String code = coded[i];
		content = content.replace("`" + code + "`", "[code]" + code + "[/code]");
	}

	for (int i = 0; i < images.size(); ++i) { //i in range(images.size())
		String image = images[i];
		Vector<String> substr = image.split("(");
		String imglink = substr[1].rstrip(")");
		content = content.replace(image, "[img]" + imglink + "[/img]");
	}

	for (int i = 0; i < links.size(); ++i) {
		content = content.replace("[" + linknames[i] + "](" + links[i] + ")", "[url=" + links[i] + "]" + linknames[i] + "[/url]");
	}

	for (int i = 0; i < lists.size(); ++i) { //i in range(lists.size())
		String element = lists[i];

		if (content.find("- " + element)) {
			content = content.replace("-" + element, "[indent]-" + element + "[/indent]");
		}

		if (content.find("+ " + element)) {
			content = content.replace("+" + element, "[indent]-" + element + "[/indent]");
		}

		if (content.find("* " + element)) {
			content = content.replace("+" + element, "[indent]-" + element + "[/indent]");
		}
	}

	text_preview->append_bbcode(content);
	text_preview->show();
}

void TextEditorPreview::print_html(const String &p_content) {
	String content = p_content;

	content = content.replace("<i>", "[i]");
	content = content.replace("</i>", "[/i]");
	content = content.replace("<b>", "[b]");
	content = content.replace("</b>", "[/b]");
	content = content.replace("<u>", "[u]");
	content = content.replace("</u>", "[/u]");
	content = content.replace("<ins>", "[u]");
	content = content.replace("</ins>", "[/u]");
	content = content.replace("<del>", "[s]");
	content = content.replace("</del>", "[/s]");
	content = content.replace("<a href=\"", "[url=");
	content = content.replace("\">", "]");
	content = content.replace("</a>", "[/url]");
	content = content.replace("<img src=\"", "[img]");
	content = content.replace("\" />", "[/img]");
	content = content.replace("\"/>", "[/img]");
	content = content.replace("<pre>", "[code]");
	content = content.replace("</pre>", "[/code]");
	content = content.replace("<center>", "[center]");
	content = content.replace("</center>", "[/center]");
	content = content.replace("<right>", "[right]");
	content = content.replace("</right>", "[/right]");

	text_preview->append_bbcode(content);
	text_preview->show();
}

void TextEditorPreview::print_csv(const Array &rows) {
	//TODO
	/*
	  table_preview->set_columns(rows[0].size());

	  for (item in rows) {
		  for (string in item) {
			  Label *label = memnew(Label);
			  label->set_text(str(string));
			  label->set_h_size_flags(SIZE_EXPAND);
			  label->set_align(1);
			  label->set_valign(1);
			  table_preview->add_child(label);
		  }
	  }

	  table_preview->show();
		*/
}

void TextEditorPreview::_on_TextEditorPreview_popup_hide() {
	queue_delete();
}

TextEditorPreview::TextEditorPreview() {
	set_title("File preview");
	set_resizable(true);
	set_anchors_and_margins_preset(PRESET_WIDE);
	set_margin(MARGIN_LEFT, 81);
	set_margin(MARGIN_TOP, 47);
	set_margin(MARGIN_RIGHT, -80);
	set_margin(MARGIN_BOTTOM, -48);

	VBoxContainer *vbc = memnew(VBoxContainer);
	vbc->set_anchors_and_margins_preset(PRESET_WIDE);
	add_child(vbc);

	text_preview = memnew(RichTextLabel);
	vbc->add_child(text_preview);
	text_preview->set_scroll_follow(true);
	text_preview->set_use_bbcode(true);
	text_preview->set_v_size_flags(SIZE_EXPAND_FILL);
	text_preview->hide();

	table_preview = memnew(GridContainer);
	vbc->add_child(table_preview);
	table_preview->set_columns(3);
	table_preview->set_h_size_flags(SIZE_EXPAND_FILL);
	table_preview->set_v_size_flags(SIZE_EXPAND_FILL);
	table_preview->hide();
}

TextEditorPreview::~TextEditorPreview() {
}

void TextEditorPreview::_notification(int p_what) {
	if (p_what == NOTIFICATION_POSTINITIALIZE) {
		connect("popup_hide", this, "_on_TextEditorPreview_popup_hide");
	}
}

void TextEditorPreview::_bind_methods() {
	ADD_SIGNAL(MethodInfo("image_downloaded"));
	ADD_SIGNAL(MethodInfo("image_loaded"));

	//ClassDB::bind_method(D_METHOD("print_preview", "content"), &TextEditorPreview::print_preview);
	//ClassDB::bind_method(D_METHOD("print_bb", "content"), &TextEditorPreview::print_bb);
	//	ClassDB::bind_method(D_METHOD("print_markdown", "content"), &TextEditorPreview::print_markdown);
	//ClassDB::bind_method(D_METHOD("print_html", "content"), &TextEditorPreview::print_html);
	//ClassDB::bind_method(D_METHOD("print_csv", "rows"), &TextEditorPreview::print_csv);
	ClassDB::bind_method(D_METHOD("_on_TextEditorPreview_popup_hide"), &TextEditorPreview::_on_TextEditorPreview_popup_hide);
}
