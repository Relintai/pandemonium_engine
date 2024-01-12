/*************************************************************************/
/*  text_editor_vanilla_editor.cpp                                       */
/*************************************************************************/
/*                         This file is part of:                         */
/*                          PANDEMONIUM ENGINE                           */
/*             https://github.com/Relintai/pandemonium_engine            */
/*************************************************************************/
/* Copyright (c) 2022-present Péter Magyar.                              */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "text_editor_vanilla_editor.h"

#include "core/math/color.h"
#include "core/object/object.h"
#include "text_editor_settings.h"

#include "scene/gui/box_container.h"
#include "scene/gui/button.h"
#include "scene/gui/check_box.h"
#include "scene/gui/file_dialog.h"
#include "scene/gui/label.h"
#include "scene/gui/line_edit.h"
#include "scene/gui/resources/syntax_highlighter.h"
#include "scene/gui/text_edit.h"
#include "scene/gui/texture_rect.h"

#include "editor_code_editor/editor_code_text_editor.h"
#include "editor_code_editor/editor_find_replace_bar.h"

#include "scene/resources/font/dynamic_font.h"

String TextEditorVanillaEditor::get_current_path() {
	return current_path;
}

void TextEditorVanillaEditor::set_current_path(const String &val) {
	current_path = val;
}

String TextEditorVanillaEditor::get_current_filename() {
	return current_filename;
}

void TextEditorVanillaEditor::set_current_filename(const String &val) {
	current_filename = val;
}

int TextEditorVanillaEditor::get_search_flag() const {
	return search_flag;
}

void TextEditorVanillaEditor::set_search_flag(const int val) {
	search_flag = val;
}

void TextEditorVanillaEditor::set_font(const String &font_path) {
	Ref<DynamicFont> dynamic_font;
	Ref<DynamicFontData> dynamic_font_data;

	dynamic_font.instance();
	dynamic_font_data.instance();

	dynamic_font_data->set_font_path(font_path);
	dynamic_font->set_font_data(dynamic_font_data);
	text_editor->set("custom_fonts/font", dynamic_font);
}

void TextEditorVanillaEditor::load_default_font() {
	String default_font = last_opened_files->get_editor_font();

	if (default_font != "") {
		set_font(default_font);
	}
}

void TextEditorVanillaEditor::set_wrap_enabled(const bool enabled) {
	text_editor->set_wrap_enabled(enabled);
	text_editor->update();
}

void TextEditorVanillaEditor::draw_minimap(const bool value) {
	text_editor->set_draw_minimap(value);
	text_editor->update();
}

void TextEditorVanillaEditor::color_region(const String &fileextension) {
	/*
	if (fileextension == "bbs") {
		Ref<CodeHighlighter> highlighter;
		highlighter.instance();

		highlighter->add_color_region("[b]", "[/b]", Color::color8(153, 153, 255, 255), false);
		highlighter->add_color_region("[i]", "[/i]", Color::color8(153, 255, 153, 255), false);
		highlighter->add_color_region("[s]", "[/s]", Color::color8(255, 153, 153, 255), false);
		highlighter->add_color_region("[u]", "[/u]", Color::color8(255, 255, 102, 255), false);
		highlighter->add_color_region("[url", "[/url]", Color::color8(153, 204, 255, 255), false);
		highlighter->add_color_region("[code]", "[/code]", Color::color8(192, 192, 192, 255), false);
		highlighter->add_color_region("[img]", "[/img]", Color::color8(255, 204, 153, 255), false);
		highlighter->add_color_region("[center]", "[/center]", Color::color8(175, 238, 238, 255), false);
		highlighter->add_color_region("[right]", "[/right]", Color::color8(135, 206, 235, 255), false);

		text_editor->set_syntax_highlighter(highlighter);
	} else if (fileextension == "html") {
		Ref<CodeHighlighter> highlighter;
		highlighter.instance();

		highlighter->add_color_region("<b>", "</b>", Color::color8(153, 153, 255, 255), false);
		highlighter->add_color_region("<i>", "</i>", Color::color8(153, 255, 153, 255), false);
		highlighter->add_color_region("<del>", "</del>", Color::color8(255, 153, 153, 255), false);
		highlighter->add_color_region("<ins>", "</ins>", Color::color8(255, 255, 102, 255), false);
		highlighter->add_color_region("<a", "</a>", Color::color8(153, 204, 255, 255), false);
		highlighter->add_color_region("<img", "/>", Color::color8(255, 204, 153, 255), true);
		highlighter->add_color_region("<pre>", "</pre>", Color::color8(192, 192, 192, 255), false);
		highlighter->add_color_region("<center>", "</center>", Color::color8(175, 238, 238, 255), false);
		highlighter->add_color_region("<right>", "</right>", Color::color8(135, 206, 235, 255), false);

		text_editor->set_syntax_highlighter(highlighter);
	} else if (fileextension == "md") {
		Ref<CodeHighlighter> highlighter;
		highlighter.instance();

		highlighter->add_color_region("***", "***", Color::color8(126, 186, 181, 255), false);
		highlighter->add_color_region("**", "**", Color::color8(153, 153, 255, 255), false);
		highlighter->add_color_region("*", "*", Color::color8(153, 255, 153, 255), false);
		highlighter->add_color_region("+ ", "", Color::color8(255, 178, 102, 255), false);
		highlighter->add_color_region("- ", "", Color::color8(255, 178, 102, 255), false);
		highlighter->add_color_region("~~", "~~", Color::color8(255, 153, 153, 255), false);
		highlighter->add_color_region("__", "__", Color::color8(255, 255, 102, 255), false);
		highlighter->add_color_region("[", ")", Color::color8(153, 204, 255, 255), false);
		highlighter->add_color_region("`", "`", Color::color8(192, 192, 192, 255), false);
		highlighter->add_color_region("\"*.", "\"", Color::color8(255, 255, 255, 255), true);
		highlighter->add_color_region("# ", "", Color::color8(105, 105, 105, 255), true);
		highlighter->add_color_region("## ", "", Color::color8(128, 128, 128, 255), true);
		highlighter->add_color_region("### ", "", Color::color8(169, 169, 169, 255), true);
		highlighter->add_color_region("#### ", "", Color::color8(192, 192, 192, 255), true);
		highlighter->add_color_region("##### ", "", Color::color8(211, 211, 211, 255), true);
		highlighter->add_color_region("###### ", "", Color::color8(255, 255, 255, 255), true);
		highlighter->add_color_region("> ", "", Color::color8(172, 138, 79, 255), true);

		text_editor->set_syntax_highlighter(highlighter);
	} else if (fileextension == "cfg") {
		Ref<CodeHighlighter> highlighter;
		highlighter.instance();

		highlighter->add_color_region("[", "]", Color::color8(153, 204, 255, 255), false);
		highlighter->add_color_region("\"", "\"", Color::color8(255, 255, 102, 255), false);
		highlighter->add_color_region(";", "", Color::color8(128, 128, 128, 255), true);

		text_editor->set_syntax_highlighter(highlighter);
	} else if (fileextension == "ini") {
		Ref<CodeHighlighter> highlighter;
		highlighter.instance();

		highlighter->add_color_region("[", "]", Color::color8(153, 204, 255, 255), false);
		highlighter->add_color_region("\"", "\"", Color::color8(255, 255, 102, 255), false);
		highlighter->add_color_region(";", "", Color::color8(128, 128, 128, 255), true);

		text_editor->set_syntax_highlighter(highlighter);
	}
	*/
}

void TextEditorVanillaEditor::clean_editor() {
	text_editor->set_text("");
	//file_info_last_modified_icon.texture = IconLoader.load_icon_from_name("save");
	file_info_last_modified->set_text("");
	file_list->invalidate();
	current_filename = "";
	current_path = "";
}

void TextEditorVanillaEditor::new_file_open(const String &file_content, const OS::DateTime &last_modified, const String &current_file_path) {
	current_path = current_file_path;
	current_filename = current_file_path.get_file();
	color_region(current_filename.get_extension());
	text_editor->set_text(file_content);
	update_lastmodified(last_modified, "save");
	file_list->invalidate();
	count_characters();
}

void TextEditorVanillaEditor::update_lastmodified(const OS::DateTime &last_modified, const String &icon) {
	file_info_last_modified->set_text(itos(last_modified.time.hour) + ":" + itos(last_modified.time.min) + "  " + itos(last_modified.date.day) + "/" + itos(last_modified.date.month) + "/" + itos(last_modified.date.year));
}

//file_info_last_modified_icon.texture = IconLoader.load_icon_from_name(icon);

void TextEditorVanillaEditor::new_file_create(const String &file_name) {
	text_editor->set_text("");
	file_list->invalidate();
}

void TextEditorVanillaEditor::_on_Readonly_toggled(const bool button_pressed) {
	if (button_pressed) {
		file_info_read_only->set_text("Read Only");
		text_editor->set_readonly((true));
	} else {
		file_info_read_only->set_text("Can Edit");
		text_editor->set_readonly((false));
	}
}

void TextEditorVanillaEditor::_on_text_editor_text_changed() {
	//file_info_last_modified_icon.texture = IconLoader.load_icon_from_name("saveas");
	count_characters();
	emit_signal("text_changed");
}

void TextEditorVanillaEditor::count_characters() {
	int counted = 0;

	for (int line = 0; line < text_editor->get_line_count(); ++line) { //line in range(text_editor.get_line_count())
		counted += text_editor->get_line(line).length();
	}

	file_info_c_counter->set_text(itos(counted));
}

void TextEditorVanillaEditor::open_search_box() {
	_find_replace_bar->popup_search();
}
void TextEditorVanillaEditor::open_replace_box() {
	_find_replace_bar->popup_replace();
}

TextEditorVanillaEditor::TextEditorVanillaEditor() {
	search_flag = 0;

	set_v_size_flags(SIZE_EXPAND_FILL);
	set_anchors_and_margins_preset(PRESET_WIDE);

	text_editor = memnew(TextEdit);
	add_child(text_editor);
	text_editor->set_highlight_current_line(true);
	text_editor->set_show_line_numbers(true);
	text_editor->set_breakpoint_gutter_enabled(true);
	text_editor->set_highlight_all_occurrences(true);
	text_editor->set_override_selected_font_color(true);
	text_editor->set_smooth_scroll_enabled(true);
	text_editor->set_hiding_enabled(true);
	//todo look this up from the editor settings;
	//text_editor->set_caret_blink(true);
	//text_editor->set_caret_blink_speed(1);
	text_editor->set_right_click_moves_caret(false);
	text_editor->set_draw_minimap(true);
	text_editor->set_v_size_flags(SIZE_EXPAND_FILL);
	text_editor->set("custom_colors/member_variable_color", Color(0.737255, 0.882353, 1));
	text_editor->set("custom_colors/code_folding_color", Color(1, 1, 1, 0.701961));
	text_editor->set("custom_colors/function_color", Color(0.341176, 0.701961, 1));
	text_editor->set("custom_colors/safe_line_number_color", Color(0.8, 0.968627, 0.827451, 0.74902));
	text_editor->set("custom_colors/symbol_color", Color(0.670588, 0.788235, 1));
	text_editor->set("custom_colors/caret_background_color", Color(0, 0, 0));
	text_editor->set("custom_colors/selection_color", Color(0.411765, 0.611765, 0.909804, 0.34902));
	text_editor->set("custom_colors/caret_color", Color(1, 1, 1));
	text_editor->set("custom_colors/breakpoint_color", Color(1, 0.470588, 0.419608));
	text_editor->set("custom_colors/font_color_selected", Color(0, 0, 0));
	text_editor->set("custom_colors/font_color", Color(1, 1, 1));
	text_editor->set("custom_colors/completion_font_color", Color(1, 1, 1, 0.392157));
	text_editor->set("custom_colors/completion_scroll_color", Color(1, 1, 1, 0.070588));
	text_editor->set("custom_colors/background_color", Color(0.121569, 0.145098, 0.192157));
	text_editor->set("custom_colors/number_color", Color(0.631373, 1, 0.882353));
	text_editor->set("custom_colors/completion_background_color", Color(0.196078, 0.231373, 0.309804));
	text_editor->set("custom_colors/brace_mismatch_color", Color(1, 0.470588, 0.419608));
	text_editor->set("custom_colors/current_line_color", Color(1, 1, 1, 0.070588));
	text_editor->set("custom_colors/completion_selected_color", Color(1, 1, 1, 0.070588));
	text_editor->set("custom_colors/mark_color", Color(1, 0.470588, 0.419608, 0.301961));
	text_editor->set("custom_colors/word_highlighted_color", Color(1, 1, 1, 0.392157));
	text_editor->set("custom_colors/completion_existing_color", Color(1, 1, 1, 0.392157));
	text_editor->set("custom_constants/completion_lines", 20);
	text_editor->set("custom_constants/completion_max_width", 20);
	text_editor->set("custom_constants/completion_scroll_width", 20);

	_find_replace_bar = memnew(EditorFindReplaceBar);
	_find_replace_bar->set_text_edit(text_editor);
	add_child(_find_replace_bar);
	_find_replace_bar->hide();

	HBoxContainer *file_info = memnew(HBoxContainer);
	add_child(file_info);

	file_info_last_modified_icon = memnew(TextureRect);
	file_info->add_child(file_info_last_modified_icon);
	file_info_last_modified_icon->set_stretch_mode(TextureRect::STRETCH_KEEP_CENTERED);

	Label *filabel = memnew(Label);
	file_info->add_child(filabel);
	filabel->set_text("Last modified time:");

	file_info_last_modified = memnew(Label);
	file_info->add_child(file_info_last_modified);

	Label *fi2label = memnew(Label);
	file_info->add_child(fi2label);
	fi2label->set_text("Characters counter:");
	fi2label->set_align(Label::ALIGN_RIGHT);
	fi2label->set_h_size_flags(SIZE_EXPAND | SIZE_FILL | SIZE_SHRINK_CENTER | SIZE_SHRINK_END);

	file_info_c_counter = memnew(Label);
	file_info->add_child(file_info_c_counter);
	file_info_c_counter->set_h_size_flags(SIZE_EXPAND);

	file_info_read_only = memnew(CheckBox);
	file_info->add_child(file_info_read_only);
	file_info_read_only->set_text("Can Edit");
	file_info_read_only->set_flat(true);
	file_info_read_only->set_h_size_flags(SIZE_EXPAND | SIZE_SHRINK_END);
}

TextEditorVanillaEditor::~TextEditorVanillaEditor() {
}

void TextEditorVanillaEditor::_notification(int p_what) {
	if (p_what == NOTIFICATION_POST_ENTER_TREE) {
		text_editor->connect("text_changed", this, "_on_text_editor_text_changed");
		//FileList = get_parent().get_parent().get_parent().get_parent().get_node("FileList");
		file_info_read_only->connect("toggled", this, "_on_Readonly_toggled");
		//file_info_read_only.set("custom_icons/checked",IconLoader.load_icon_from_name("read"));
		//file_info_read_only.set("custom_icons/unchecked",IconLoader.load_icon_from_name("edit"));
		add_to_group("vanilla_editor");
		load_default_font();
	}
}

void TextEditorVanillaEditor::_bind_methods() {
	ADD_SIGNAL(MethodInfo("text_changed"));

	//	ClassDB::bind_method(D_METHOD("get_current_path"), &TextEditorVanillaEditor::get_current_path);
	//	ClassDB::bind_method(D_METHOD("set_current_path", "value"), &TextEditorVanillaEditor::set_current_path);
	//ADD_PROPERTY(PropertyInfo(Variant::STRING, "current_path"), "set_current_path", "get_current_path");

	//ClassDB::bind_method(D_METHOD("get_current_filename"), &TextEditorVanillaEditor::get_current_filename);
	//ClassDB::bind_method(D_METHOD("set_current_filename", "value"), &TextEditorVanillaEditor::set_current_filename);
	//ADD_PROPERTY(PropertyInfo(Variant::STRING, "current_filename"), "set_current_filename", "get_current_filename");

	//ClassDB::bind_method(D_METHOD("get_search_flag"), &TextEditorVanillaEditor::get_search_flag);
	//ClassDB::bind_method(D_METHOD("set_search_flag", "value"), &TextEditorVanillaEditor::set_search_flag);
	//ADD_PROPERTY(PropertyInfo(Variant::INT, "search_flag"), "set_search_flag", "get_search_flag");

	//	ClassDB::bind_method(D_METHOD("set_font", "font_path"), &TextEditorVanillaEditor::set_font);
	//	ClassDB::bind_method(D_METHOD("load_default_font"), &TextEditorVanillaEditor::load_default_font);
	//ClassDB::bind_method(D_METHOD("set_wrap_enabled", "enabled"), &TextEditorVanillaEditor::set_wrap_enabled);
	//ClassDB::bind_method(D_METHOD("draw_minimap", "value"), &TextEditorVanillaEditor::draw_minimap);
	//ClassDB::bind_method(D_METHOD("color_region", "filextension"), &TextEditorVanillaEditor::color_region);
	//ClassDB::bind_method(D_METHOD("clean_editor"), &TextEditorVanillaEditor::clean_editor);
	//ClassDB::bind_method(D_METHOD("new_file_open", "file_content", "last_modified", "current_file_path"), &TextEditorVanillaEditor::new_file_open);
	//ClassDB::bind_method(D_METHOD("update_lastmodified", "last_modified", "icon"), &TextEditorVanillaEditor::update_lastmodified);
	//ClassDB::bind_method(D_METHOD("new_file_create", "file_name"), &TextEditorVanillaEditor::new_file_create);
	ClassDB::bind_method(D_METHOD("_on_Readonly_toggled", "button_pressed"), &TextEditorVanillaEditor::_on_Readonly_toggled);
	ClassDB::bind_method(D_METHOD("_on_text_editor_text_changed"), &TextEditorVanillaEditor::_on_text_editor_text_changed);
	//ClassDB::bind_method(D_METHOD("count_characters"), &TextEditorVanillaEditor::count_characters);
}
