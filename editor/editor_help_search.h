#ifndef EDITOR_HELP_SEARCH_H
#define EDITOR_HELP_SEARCH_H

/*************************************************************************/
/*  editor_help_search.h                                                 */
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

#include "core/object/reference.h"
#include "scene/gui/dialogs.h"

#include "core/containers/ordered_hash_map.h"
#include "core/containers/rb_map.h"
#include "core/containers/vector.h"
#include "core/math/color.h"
#include "core/object/object.h"
#include "core/string/ustring.h"

#include "editor/doc/doc_data.h"
#include "scene/resources/texture.h"

class Control;
class InputEvent;
class LineEdit;
class OptionButton;
class ToolButton;
class Tree;
class TreeItem;

class EditorHelpSearch : public ConfirmationDialog {
	GDCLASS(EditorHelpSearch, ConfirmationDialog);

	enum SearchFlags {
		SEARCH_CLASSES = 1 << 0,
		SEARCH_METHODS = 1 << 1,
		SEARCH_SIGNALS = 1 << 2,
		SEARCH_CONSTANTS = 1 << 3,
		SEARCH_PROPERTIES = 1 << 4,
		SEARCH_THEME_ITEMS = 1 << 5,
		SEARCH_ALL = SEARCH_CLASSES | SEARCH_METHODS | SEARCH_SIGNALS | SEARCH_CONSTANTS | SEARCH_PROPERTIES | SEARCH_THEME_ITEMS,
		SEARCH_CASE_SENSITIVE = 1 << 29,
		SEARCH_SHOW_HIERARCHY = 1 << 30
	};

	LineEdit *search_box;
	ToolButton *case_sensitive_button;
	ToolButton *hierarchy_button;
	OptionButton *filter_combo;
	Tree *results_tree;
	bool old_search;
	String old_term;

	class Runner;

	Ref<Runner> search;

	void _update_icons();
	void _update_results();

	void _search_box_gui_input(const Ref<InputEvent> &p_event);
	void _search_box_text_changed(const String &p_text);
	void _filter_combo_item_selected(int p_option);
	void _confirmed();

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	void popup_dialog();
	void popup_dialog(const String &p_term);

	EditorHelpSearch();
};

class EditorHelpSearch::Runner : public Reference {
	enum Phase {
		PHASE_MATCH_CLASSES_INIT,
		PHASE_MATCH_CLASSES,
		PHASE_CLASS_ITEMS_INIT,
		PHASE_CLASS_ITEMS,
		PHASE_MEMBER_ITEMS_INIT,
		PHASE_MEMBER_ITEMS,
		PHASE_SELECT_MATCH,
		PHASE_MAX
	};
	int phase;

	struct ClassMatch {
		DocData::ClassDoc *doc;
		bool name;
		Vector<DocData::MethodDoc *> methods;
		Vector<DocData::MethodDoc *> signals;
		Vector<DocData::ConstantDoc *> constants;
		Vector<DocData::PropertyDoc *> properties;
		Vector<DocData::ThemeItemDoc *> theme_properties;

		bool required() {
			return name || methods.size() || signals.size() || constants.size() || properties.size() || theme_properties.size();
		}
	};

	Control *ui_service;
	Tree *results_tree;
	String term;
	int search_flags;

	Ref<Texture> empty_icon;
	Color disabled_color;

	RBMap<String, DocData::ClassDoc>::Element *iterator_doc;
	RBMap<String, ClassMatch> matches;
	RBMap<String, ClassMatch>::Element *iterator_match;
	TreeItem *root_item;
	RBMap<String, TreeItem *> class_items;
	TreeItem *matched_item;
	float match_highest_score = 0;

	bool _slice();
	bool _phase_match_classes_init();
	bool _phase_match_classes();
	bool _phase_class_items_init();
	bool _phase_class_items();
	bool _phase_member_items_init();
	bool _phase_member_items();
	bool _phase_select_match();

	bool _match_string(const String &p_term, const String &p_string) const;
	void _match_item(TreeItem *p_item, const String &p_text);
	TreeItem *_create_class_hierarchy(const ClassMatch &p_match);
	TreeItem *_create_class_item(TreeItem *p_parent, const DocData::ClassDoc *p_doc, bool p_gray);
	TreeItem *_create_method_item(TreeItem *p_parent, const DocData::ClassDoc *p_class_doc, const DocData::MethodDoc *p_doc);
	TreeItem *_create_signal_item(TreeItem *p_parent, const DocData::ClassDoc *p_class_doc, const DocData::MethodDoc *p_doc);
	TreeItem *_create_constant_item(TreeItem *p_parent, const DocData::ClassDoc *p_class_doc, const DocData::ConstantDoc *p_doc);
	TreeItem *_create_property_item(TreeItem *p_parent, const DocData::ClassDoc *p_class_doc, const DocData::PropertyDoc *p_doc);
	TreeItem *_create_theme_property_item(TreeItem *p_parent, const DocData::ClassDoc *p_class_doc, const DocData::ThemeItemDoc *p_doc);
	TreeItem *_create_member_item(TreeItem *p_parent, const String &p_class_name, const String &p_icon, const String &p_name, const String &p_type, const String &p_metatype, const String &p_tooltip);

public:
	bool work(uint64_t slot = 100000);

	Runner(Control *p_icon_service, Tree *p_results_tree, const String &p_term, int p_search_flags);
};

#endif // EDITOR_HELP_SEARCH_H
