#ifndef LAYERED_TILE_SET_SCENES_COLLECTION_SOURCE_EDITOR_H
#define LAYERED_TILE_SET_SCENES_COLLECTION_SOURCE_EDITOR_H

/*************************************************************************/
/*  layered_tile_set_scenes_collection_source_editor.h                   */
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

#include "../layered_tile_set.h"
#include "editor/editor_inspector.h"
#include "scene/gui/box_container.h"

class Button;
class ItemList;
class Label;
class EditorFileDialog;

class LayeredTileSetScenesCollectionSourceEditor : public HBoxContainer {
	GDCLASS(LayeredTileSetScenesCollectionSourceEditor, HBoxContainer);

private:
	// -- Proxy object for an atlas source, needed by the inspector --
	class LayeredTileSetScenesCollectionProxyObject : public Object {
		GDCLASS(LayeredTileSetScenesCollectionProxyObject, Object);

	private:
		Ref<LayeredTileSet> tile_set;
		LayeredTileSetScenesCollectionSource *tile_set_scenes_collection_source = nullptr;
		int source_id = -1;

	protected:
		void _changed_callback(Object *p_changed, const char *p_prop);

		bool _set(const StringName &p_name, const Variant &p_value);
		bool _get(const StringName &p_name, Variant &r_ret) const;
		void _get_property_list(List<PropertyInfo> *p_list) const;
		static void _bind_methods();

	public:
		void set_id(int p_id);
		int get_id();

		void edit(Ref<LayeredTileSet> p_tile_set, LayeredTileSetScenesCollectionSource *p_tile_set_scenes_collection_source, int p_source_id);
	};

	// -- Proxy object for a tile, needed by the inspector --
	class LayeredSceneTileProxyObject : public Object {
		GDCLASS(LayeredSceneTileProxyObject, Object);

	private:
		LayeredTileSetScenesCollectionSourceEditor *tile_set_scenes_collection_source_editor = nullptr;

		LayeredTileSetScenesCollectionSource *tile_set_scenes_collection_source = nullptr;
		int source_id;
		int scene_id;

	protected:
		bool _set(const StringName &p_name, const Variant &p_value);
		bool _get(const StringName &p_name, Variant &r_ret) const;
		void _get_property_list(List<PropertyInfo> *p_list) const;

		static void _bind_methods();

	public:
		// Update the proxyed object.
		void edit(LayeredTileSetScenesCollectionSource *p_tile_set_atlas_source, int p_scene_id);

		LayeredSceneTileProxyObject(LayeredTileSetScenesCollectionSourceEditor *p_tiles_set_scenes_collection_source_editor) {
			tile_set_scenes_collection_source_editor = p_tiles_set_scenes_collection_source_editor;
		}
	};

private:
	Ref<LayeredTileSet> tile_set;
	LayeredTileSetScenesCollectionSource *tile_set_scenes_collection_source = nullptr;
	int tile_set_source_id = -1;

	bool tile_set_scenes_collection_source_changed_needs_update = false;

	// Source inspector.
	LayeredTileSetScenesCollectionProxyObject *scenes_collection_source_proxy_object = nullptr;
	Label *scenes_collection_source_inspector_label = nullptr;
	EditorInspector *scenes_collection_source_inspector = nullptr;

	// Tile inspector.
	LayeredSceneTileProxyObject *tile_proxy_object = nullptr;
	Label *tile_inspector_label = nullptr;
	EditorInspector *tile_inspector = nullptr;

	ItemList *scene_tiles_list = nullptr;
	Button *scene_tile_add_button = nullptr;
	Button *scene_tile_delete_button = nullptr;
	EditorFileDialog *scene_select_dialog = nullptr;

	void _tile_set_scenes_collection_source_changed();
	void _scenes_collection_source_proxy_object_changed(const String &p_what);
	void _scene_thumbnail_done(const String &p_path, const Ref<Texture> &p_preview, const Ref<Texture> &p_small_preview, const Variant &p_ud);
	void _scenes_list_item_activated(int p_index);

	void _source_add_pressed();
	void _scene_file_selected(const String &p_path);
	void _source_delete_pressed();

	// Update methods.
	void _update_source_inspector();
	void _update_tile_inspector();
	void _update_scenes_list();
	void _update_action_buttons();

	void _drop_data_fw(const Point2 &p_point, const Variant &p_data, Control *p_from);
	bool _can_drop_data_fw(const Point2 &p_point, const Variant &p_data, Control *p_from) const;
	
	void _tile_proxy_object_changed(const String &p_what);
	void _scene_tiles_list_item_selected(int p_index);

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	void edit(Ref<LayeredTileSet> p_tile_set, LayeredTileSetScenesCollectionSource *p_tile_set_scenes_collection_source, int p_source_id);
	LayeredTileSetScenesCollectionSourceEditor();
	~LayeredTileSetScenesCollectionSourceEditor();
};

#endif // TILE_SET_SCENES_COLLECTION_SOURCE_EDITOR_H
