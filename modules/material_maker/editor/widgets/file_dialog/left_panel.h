#ifndef LEFT_PANEL_H
#define LEFT_PANEL_H

#include "core/variant/array.h"
#include "core/os/input_event.h"
#include "core/string/ustring.h"

#include "scene/gui/box_container.h"

class ItemList;

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

	void _on_FavList_item_activated(const int index);
	void _on_RecentList_item_activated(const int index);
	void _on_FavList_gui_input(const Ref<InputEvent> &event);
	void _on_RecentList_gui_input(const Ref<InputEvent> &event);

	LeftPanel();
	~LeftPanel();

protected:
	static void _bind_methods();

	Array recents;
	Array favorites;

	ItemList *_recent_list;
	ItemList *_fav_list;
};

#endif
