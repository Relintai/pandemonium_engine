#ifndef LEFT_PANEL_H
#define LEFT_PANEL_H


class LeftPanel : public VBoxContainer {
 GDCLASS(LeftPanel, VBoxContainer);

 public:

 Array get_recents();
 void set_recents(const Array &val);

 Array get_favorites();
 void set_favorites(const Array &val);

 void add_recent(const String &file_path);
 void add_favorite(const String &file_path);
 String my_basename(const String &s);
 void update_lists();
 void _on_FavList_item_activated(const Variant &index);
 void _on_RecentList_item_activated(const Variant &index);
 void _on_FavList_gui_input(const Variant &event);
 void _on_RecentList_gui_input(const Variant &event);

 LeftPanel();
 ~LeftPanel();

 protected:
 static void _bind_methods();

 //tool
 Array recents = [];
 Array favorites = [];
 signal open_directory(dirpath);
 //func _ready():
 //	if get_node("/root/MainWindow") != null:
 //		var config_cache = get_node("/root/MainWindow").config_cache
 //		if config_cache.has_section_key("file_dialog", "recents"):
 //			var parse_result = JSON.parse(config_cache.get_value("file_dialog", "recents"))
 //			if parse_result != null:
 //				recents = parse_result.result
 //		if config_cache.has_section_key("file_dialog", "favorites"):
 //			var parse_result = JSON.parse(config_cache.get_value("file_dialog", "favorites"))
 //			if parse_result != null:
 //				favorites = parse_result.result
 //		update_lists()
 //func _exit_tree():
 //	if get_node("/root/MainWindow") != null:
 //		var config_cache = get_node("/root/MainWindow").config_cache
 //		config_cache.set_value("file_dialog", "recents", JSON.print(recents))
 //		config_cache.set_value("file_dialog", "favorites", JSON.print(favorites))
};


#endif
