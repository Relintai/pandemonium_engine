
#include "left_panel.h"


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



 //tool;
  Array recents = [];
  Array favorites = [];
 signal open_directory(dirpath);
 //func _ready():;
 //	if get_node("/root/MainWindow") != null:;
 //		var config_cache = get_node("/root/MainWindow").config_cache;
 //		if config_cache.has_section_key("file_dialog", "recents"):;
 //			var parse_result = JSON.parse(config_cache.get_value("file_dialog", "recents"));
 //			if parse_result != null:;
 //				recents = parse_result.result;
 //		if config_cache.has_section_key("file_dialog", "favorites"):;
 //			var parse_result = JSON.parse(config_cache.get_value("file_dialog", "favorites"));
 //			if parse_result != null:;
 //				favorites = parse_result.result;
 //		update_lists();
 //func _exit_tree():;
 //	if get_node("/root/MainWindow") != null:;
 //		var config_cache = get_node("/root/MainWindow").config_cache;
 //		config_cache.set_value("file_dialog", "recents", JSON.print(recents));
 //		config_cache.set_value("file_dialog", "favorites", JSON.print(favorites));

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

  if (slash_pos == -1 || slash_pos+1 == s.length()) {
   return s;
}

  return s.right(slash_pos+1);
}


 void LeftPanel::update_lists() {
  $FavList.clear();

  for (d in favorites) {
   $FavList.add_item(my_basename(d));
   $FavList.set_item_tooltip($FavList.get_item_count()-1, d);
}

  $RecentList.clear();

  for (d in recents) {
   $RecentList.add_item(my_basename(d));
   $RecentList.set_item_tooltip($RecentList.get_item_count()-1, d);
}

}


 void LeftPanel::_on_FavList_item_activated(const Variant &index) {
  emit_signal("open_directory", $FavList.get_item_tooltip(index));
}


 void LeftPanel::_on_RecentList_item_activated(const Variant &index) {
  emit_signal("open_directory", $RecentList.get_item_tooltip(index));
}


 void LeftPanel::_on_FavList_gui_input(const Variant &event) {

  if (event is InputEventKey && event.pressed && event.scancode == KEY_DELETE) {

   if (! $FavList.get_selected_items().empty()) {
    favorites.remove($FavList.get_selected_items()[0]);
    update_lists();
}

}

}


 void LeftPanel::_on_RecentList_gui_input(const Variant &event) {

  if (event is InputEventKey && event.pressed && event.scancode == KEY_DELETE) {

   if (! $RecentList.get_selected_items().empty()) {
    recents.remove($RecentList.get_selected_items()[0]);
    update_lists();
}

}

}

}

 LeftPanel::LeftPanel() {
  recents = [];
  favorites = [];
 }

 LeftPanel::~LeftPanel() {
 }


 static void LeftPanel::_bind_methods() {
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



