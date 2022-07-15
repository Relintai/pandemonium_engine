
#include "text_editor_vanilla_editor.h"

#include "core/color.h"
#include "core/object.h"
#include "text_editor_settings.h"

#include "scene/gui/box_container.h"
#include "scene/gui/button.h"
#include "scene/gui/check_box.h"
#include "scene/gui/file_dialog.h"
#include "scene/gui/label.h"
#include "scene/gui/line_edit.h"
#include "scene/gui/text_edit.h"
#include "scene/gui/texture_rect.h"

#include "scene/resources/dynamic_font.h"

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
	if (fileextension == "bbs") {
		text_editor->add_color_region("[b]", "[/b]", Color::color8(153, 153, 255, 255), false);
		text_editor->add_color_region("[i]", "[/i]", Color::color8(153, 255, 153, 255), false);
		text_editor->add_color_region("[s]", "[/s]", Color::color8(255, 153, 153, 255), false);
		text_editor->add_color_region("[u]", "[/u]", Color::color8(255, 255, 102, 255), false);
		text_editor->add_color_region("[url", "[/url]", Color::color8(153, 204, 255, 255), false);
		text_editor->add_color_region("[code]", "[/code]", Color::color8(192, 192, 192, 255), false);
		text_editor->add_color_region("[img]", "[/img]", Color::color8(255, 204, 153, 255), false);
		text_editor->add_color_region("[center]", "[/center]", Color::color8(175, 238, 238, 255), false);
		text_editor->add_color_region("[right]", "[/right]", Color::color8(135, 206, 235, 255), false);
	} else if (fileextension == "html") {
		text_editor->add_color_region("<b>", "</b>", Color::color8(153, 153, 255, 255), false);
		text_editor->add_color_region("<i>", "</i>", Color::color8(153, 255, 153, 255), false);
		text_editor->add_color_region("<del>", "</del>", Color::color8(255, 153, 153, 255), false);
		text_editor->add_color_region("<ins>", "</ins>", Color::color8(255, 255, 102, 255), false);
		text_editor->add_color_region("<a", "</a>", Color::color8(153, 204, 255, 255), false);
		text_editor->add_color_region("<img", "/>", Color::color8(255, 204, 153, 255), true);
		text_editor->add_color_region("<pre>", "</pre>", Color::color8(192, 192, 192, 255), false);
		text_editor->add_color_region("<center>", "</center>", Color::color8(175, 238, 238, 255), false);
		text_editor->add_color_region("<right>", "</right>", Color::color8(135, 206, 235, 255), false);
	} else if (fileextension == "md") {
		text_editor->add_color_region("***", "***", Color::color8(126, 186, 181, 255), false);
		text_editor->add_color_region("**", "**", Color::color8(153, 153, 255, 255), false);
		text_editor->add_color_region("*", "*", Color::color8(153, 255, 153, 255), false);
		text_editor->add_color_region("+ ", "", Color::color8(255, 178, 102, 255), false);
		text_editor->add_color_region("- ", "", Color::color8(255, 178, 102, 255), false);
		text_editor->add_color_region("~~", "~~", Color::color8(255, 153, 153, 255), false);
		text_editor->add_color_region("__", "__", Color::color8(255, 255, 102, 255), false);
		text_editor->add_color_region("[", ")", Color::color8(153, 204, 255, 255), false);
		text_editor->add_color_region("`", "`", Color::color8(192, 192, 192, 255), false);
		text_editor->add_color_region("\"*.", "\"", Color::color8(255, 255, 255, 255), true);
		text_editor->add_color_region("# ", "", Color::color8(105, 105, 105, 255), true);
		text_editor->add_color_region("## ", "", Color::color8(128, 128, 128, 255), true);
		text_editor->add_color_region("### ", "", Color::color8(169, 169, 169, 255), true);
		text_editor->add_color_region("#### ", "", Color::color8(192, 192, 192, 255), true);
		text_editor->add_color_region("##### ", "", Color::color8(211, 211, 211, 255), true);
		text_editor->add_color_region("###### ", "", Color::color8(255, 255, 255, 255), true);
		text_editor->add_color_region("> ", "", Color::color8(172, 138, 79, 255), true);
	} else if (fileextension == "cfg") {
		text_editor->add_color_region("[", "]", Color::color8(153, 204, 255, 255), false);
		text_editor->add_color_region("\"", "\"", Color::color8(255, 255, 102, 255), false);
		text_editor->add_color_region(";", "", Color::color8(128, 128, 128, 255), true);
	} else if (fileextension == "ini") {
		text_editor->add_color_region("[", "]", Color::color8(153, 204, 255, 255), false);
		text_editor->add_color_region("\"", "\"", Color::color8(255, 255, 102, 255), false);
		text_editor->add_color_region(";", "", Color::color8(128, 128, 128, 255), true);
	}
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

void TextEditorVanillaEditor::_on_LineEdit_text_changed(const String &new_text) {
	int linecount = text_editor->get_line_count();

	if (new_text != "") {
		bool found;
		//bool find = false;

		for (int line = 0; line < linecount; ++line) { //line in range(linecount)
			for (int column = 0; column < text_editor->get_line(line).length(); ++column) { //column in range(text_editor.get_line(line).length())
				int fl;
				int fc;
				found = text_editor->search(new_text, search_flag, line, column, fl, fc);

				if (found) {
					if (fc == line) {
						//						if not find:;
						text_editor->select(line, fl, fc, fl + new_text.length());
					}
					//							find = true;
				} else {
					text_editor->select(0, 0, 0, 0);
				}
			}
		}
	} else {
		text_editor->select(0, 0, 0, 0);
	}
}

void TextEditorVanillaEditor::_on_matchcase_toggled(const bool button_pressed) {
	if (button_pressed) {
		search_flag = 1;
	} else {
		if (search_box_whole_words_cb->is_pressed()) {
			search_flag = 2;
		} else {
			search_flag = 0;
		}
	}

	_on_LineEdit_text_changed(search_box_line_edit->get_text());
}

void TextEditorVanillaEditor::_on_wholewords_toggled(const bool button_pressed) {
	if (button_pressed) {
		search_flag = 2;
	} else {
		if (search_box_match_case_cb->is_pressed()) {
			search_flag = 1;
		} else {
			search_flag = 0;
		}
	}

	_on_LineEdit_text_changed(search_box_line_edit->get_text());
}

void TextEditorVanillaEditor::_on_close_pressed() {
	search_box->hide();
}

void TextEditorVanillaEditor::open_search_box() {
	if (search_box->is_visible()) {
		search_box->hide();
	} else {
		search_box->show();
		//	search_box->get_node(NodePath("LineEdit"))->grab_focus();
	}
}

void TextEditorVanillaEditor::_on_Button_pressed() {
	//int linecount = text_editor->get_line_count() - 1;
	String old_text = replace_box_replace_le->get_text();
	String new_text = replace_box_with->get_text();
	String text = text_editor->get_text();
	text_editor->set_text(text.replace(old_text, new_text));
}

void TextEditorVanillaEditor::open_replace_box() {
	if (replace_box->is_visible()) {
		replace_box->hide();
	} else {
		replace_box->show();
		//	replace_box->get_node("replace")->grab_focus();
	}
}

void TextEditorVanillaEditor::_on_close2_pressed() {
	replace_box->hide();
}

void TextEditorVanillaEditor::_on_LineEdit_focus_entered() {
	_on_LineEdit_text_changed(search_box_line_edit->get_text());
}

TextEditorVanillaEditor::TextEditorVanillaEditor() {
	search_flag = 0;

	set_v_size_flags(SIZE_EXPAND_FILL);
	set_anchors_and_margins_preset(PRESET_WIDE);

	text_editor = memnew(TextEdit);
	add_child(text_editor);
	text_editor->set_highlight_current_line(true);
	text_editor->set_syntax_coloring(true);
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

	search_box = memnew(HBoxContainer);
	add_child(search_box);
	search_box->hide();

	Label *selabel = memnew(Label);
	search_box->add_child(selabel);
	selabel->set_text("Search:");

	search_box_line_edit = memnew(LineEdit);
	search_box->add_child(search_box_line_edit);
	search_box_line_edit->set_h_size_flags(SIZE_EXPAND_FILL);
	search_box_line_edit->connect("text_changed", this, "_on_LineEdit_text_changed");
	search_box_line_edit->connect("focus_entered", this, "_on_LineEdit_focus_entered");

	search_box_match_case_cb = memnew(CheckBox);
	search_box->add_child(search_box_match_case_cb);
	search_box_match_case_cb->set_text("Match Case");
	search_box_match_case_cb->connect("toggled", this, "_on_matchcase_toggled");

	search_box_whole_words_cb = memnew(CheckBox);
	search_box->add_child(search_box_whole_words_cb);
	search_box_whole_words_cb->set_text("Whole Words");
	search_box_whole_words_cb->connect("toggled", this, "_on_wholewords_toggled");

	search_box_close_button = memnew(Button);
	search_box->add_child(search_box_close_button);
	search_box_close_button->set_text("x");
	search_box_close_button->set_flat(true);
	search_box_whole_words_cb->connect("pressed", this, "_on_close_pressed");

	replace_box = memnew(HBoxContainer);
	add_child(replace_box);
	replace_box->hide();

	Label *rblabel = memnew(Label);
	replace_box->add_child(rblabel);
	rblabel->set_text("Replace:");

	replace_box_replace_le = memnew(LineEdit);
	replace_box->add_child(replace_box_replace_le);
	replace_box_replace_le->set_h_size_flags(SIZE_EXPAND_FILL);

	Label *rb2label = memnew(Label);
	replace_box->add_child(rb2label);
	rb2label->set_text("With:");

	replace_box_with = memnew(LineEdit);
	replace_box->add_child(replace_box_with);
	replace_box_with->set_h_size_flags(SIZE_EXPAND_FILL);

	replace_box_button = memnew(Button);
	replace_box->add_child(replace_box_button);
	replace_box_button->set_text("Replace");
	replace_box_button->connect("pressed", this, "_on_Button_pressed");

	replace_box_close = memnew(Button);
	replace_box->add_child(replace_box_close);
	replace_box_close->set_text("x");
	replace_box_close->set_flat(true);
	replace_box_button->connect("pressed", this, "_on_close2_pressed");

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
	ClassDB::bind_method(D_METHOD("_on_LineEdit_text_changed", "new_text"), &TextEditorVanillaEditor::_on_LineEdit_text_changed);
	ClassDB::bind_method(D_METHOD("_on_matchcase_toggled", "button_pressed"), &TextEditorVanillaEditor::_on_matchcase_toggled);
	ClassDB::bind_method(D_METHOD("_on_wholewords_toggled", "button_pressed"), &TextEditorVanillaEditor::_on_wholewords_toggled);
	ClassDB::bind_method(D_METHOD("_on_close_pressed"), &TextEditorVanillaEditor::_on_close_pressed);
	//ClassDB::bind_method(D_METHOD("open_search_box"), &TextEditorVanillaEditor::open_search_box);
	ClassDB::bind_method(D_METHOD("_on_Button_pressed"), &TextEditorVanillaEditor::_on_Button_pressed);
	//ClassDB::bind_method(D_METHOD("open_replace_box"), &TextEditorVanillaEditor::open_replace_box);
	ClassDB::bind_method(D_METHOD("_on_close2_pressed"), &TextEditorVanillaEditor::_on_close2_pressed);
	ClassDB::bind_method(D_METHOD("_on_LineEdit_focus_entered"), &TextEditorVanillaEditor::_on_LineEdit_focus_entered);
}
