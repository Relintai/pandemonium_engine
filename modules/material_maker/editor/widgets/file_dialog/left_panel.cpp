
#include "left_panel.h"

#include "core/os/keyboard.h"
#include "scene/gui/item_list.h"
#include "scene/gui/label.h"

Array LeftPanel::get_recents() {
	return recents;
}

void LeftPanel::set_recents(const Array &val) {
	recents = val;
}

Array LeftPanel::get_favorites() {
	return favorites;
}

void LeftPanel::set_favorites(const Array &val) {
	favorites = val;
}

void LeftPanel::add_recent(const String &file_path) {
	if (recents.find(file_path) != -1) {
		recents.erase(file_path);
	}

	recents.push_front(file_path);
	update_lists();
}

void LeftPanel::add_favorite(const String &file_path) {
	if (favorites.find(file_path) == -1) {
		favorites.push_back(file_path);
		update_lists();
	}
}

String LeftPanel::my_basename(const String &s) {
	int slash_pos = s.find_last("/");

	if (slash_pos == -1 || slash_pos + 1 == s.length()) {
		return s;
	}

	return s.right(slash_pos + 1);
}

void LeftPanel::update_lists() {
	_fav_list->clear();

	for (int i = 0; i < favorites.size(); ++i) {
		String d = favorites[i];
		_fav_list->add_item(my_basename(d));
		_fav_list->set_item_tooltip(_fav_list->get_item_count() - 1, d);
	}

	_recent_list->clear();

	for (int i = 0; i < recents.size(); ++i) {
		String d = recents[i];
		_recent_list->add_item(my_basename(d));
		_recent_list->set_item_tooltip(_recent_list->get_item_count() - 1, d);
	}
}

void LeftPanel::_on_FavList_item_activated(const int index) {
	emit_signal("open_directory", _fav_list->get_item_tooltip(index));
}

void LeftPanel::_on_RecentList_item_activated(const int index) {
	emit_signal("open_directory", _recent_list->get_item_tooltip(index));
}

void LeftPanel::_on_FavList_gui_input(const Ref<InputEvent> &event) {
	Ref<InputEventKey> iek = event;

	if (iek.is_valid() && iek->is_pressed() && iek->get_scancode() == KEY_DELETE) {
		if (!_fav_list->get_selected_items().empty()) {
			favorites.remove(_fav_list->get_selected_items()[0]);
			update_lists();
		}
	}
}

void LeftPanel::_on_RecentList_gui_input(const Ref<InputEvent> &event) {
	Ref<InputEventKey> iek = event;

	if (iek.is_valid() && iek->is_pressed() && iek->get_scancode() == KEY_DELETE) {
		if (!_recent_list->get_selected_items().empty()) {
			recents.remove(_recent_list->get_selected_items()[0]);
			update_lists();
		}
	}
}

LeftPanel::LeftPanel() {
	set_v_size_flags(SIZE_EXPAND_FILL);

	Label *recentlabel = memnew(Label);
	recentlabel->set_text("Recent");
	add_child(recentlabel);

	_recent_list = memnew(ItemList);
	_recent_list->set_custom_minimum_size(Vector2(100, 0));
	_recent_list->set_v_size_flags(SIZE_EXPAND_FILL);
	add_child(_recent_list);

	Label *favlabel = memnew(Label);
	favlabel->set_text("Favorite");
	add_child(favlabel);

	_fav_list = memnew(ItemList);
	_fav_list->set_custom_minimum_size(Vector2(100, 0));
	_fav_list->set_v_size_flags(SIZE_EXPAND_FILL);
	add_child(_fav_list);
}

LeftPanel::~LeftPanel() {
}

void LeftPanel::_bind_methods() {
	ADD_SIGNAL(MethodInfo("open_directory", PropertyInfo(Variant::STRING, "dirpath")));

	ClassDB::bind_method(D_METHOD("get_recents"), &LeftPanel::get_recents);
	ClassDB::bind_method(D_METHOD("set_recents", "value"), &LeftPanel::set_recents);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "recents"), "set_recents", "get_recents");

	ClassDB::bind_method(D_METHOD("get_favorites"), &LeftPanel::get_favorites);
	ClassDB::bind_method(D_METHOD("set_favorites", "value"), &LeftPanel::set_favorites);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "favorites"), "set_favorites", "get_favorites");

	ClassDB::bind_method(D_METHOD("add_recent", "file_path"), &LeftPanel::add_recent);
	ClassDB::bind_method(D_METHOD("add_favorite", "file_path"), &LeftPanel::add_favorite);
	ClassDB::bind_method(D_METHOD("my_basename", "s"), &LeftPanel::my_basename);
	ClassDB::bind_method(D_METHOD("update_lists"), &LeftPanel::update_lists);
	ClassDB::bind_method(D_METHOD("_on_FavList_item_activated", "index"), &LeftPanel::_on_FavList_item_activated);
	ClassDB::bind_method(D_METHOD("_on_RecentList_item_activated", "index"), &LeftPanel::_on_RecentList_item_activated);
	ClassDB::bind_method(D_METHOD("_on_FavList_gui_input", "event"), &LeftPanel::_on_FavList_gui_input);
	ClassDB::bind_method(D_METHOD("_on_RecentList_gui_input", "event"), &LeftPanel::_on_RecentList_gui_input);
}
