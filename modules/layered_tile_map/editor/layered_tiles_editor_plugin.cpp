/*************************************************************************/
/*  layered_tiles_editor_plugin.cpp                                      */
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

#include "layered_tiles_editor_plugin.h"

#include "layered_tile_set_editor.h"

#include "core/os/mutex.h"

#include "../layered_tile_map.h"
#include "../layered_tile_map_layer.h"
#include "../layered_tile_set.h"
#include "editor/editor_interface.h"
#include "editor/editor_node.h"
#include "editor/editor_settings.h"
#include "editor/editor_string_names.h"
#include "editor/gui/editor_bottom_panel.h"
#include "editor/plugins/canvas_item_editor_plugin.h"
#include "editor/themes/editor_scale.h"
#include "scene/gui/box_container.h"
#include "scene/gui/button.h"
#include "scene/gui/separator.h"
#include "scene/main/control.h"
#include "scene/resources/image_texture.h"

LayeredTilesEditorUtils *LayeredTilesEditorUtils::singleton = nullptr;
LayeredTileMapEditorPlugin *tile_map_plugin_singleton = nullptr;
LayeredTileSetEditorPlugin *tile_set_plugin_singleton = nullptr;

void LayeredTilesEditorUtils::_preview_frame_started() {
	RS::get_singleton()->request_frame_drawn_callback(callable_mp(const_cast<LayeredTilesEditorUtils *>(this), &LayeredTilesEditorUtils::_pattern_preview_done));
}

void LayeredTilesEditorUtils::_pattern_preview_done() {
	pattern_preview_done.post();
}

void LayeredTilesEditorUtils::_thread_func(void *ud) {
	LayeredTilesEditorUtils *te = static_cast<LayeredTilesEditorUtils *>(ud);
	set_current_thread_safe_for_nodes(true);
	te->_thread();
}

void LayeredTilesEditorUtils::_thread() {
	pattern_thread_exited.clear();
	while (!pattern_thread_exit.is_set()) {
		pattern_preview_sem.wait();

		pattern_preview_mutex.lock();
		if (pattern_preview_queue.size() == 0) {
			pattern_preview_mutex.unlock();
		} else {
			QueueItem item = pattern_preview_queue.front()->get();
			pattern_preview_queue.pop_front();
			pattern_preview_mutex.unlock();

			int thumbnail_size = EDITOR_GET("filesystem/file_dialog/thumbnail_size");
			thumbnail_size *= EDSCALE;
			Vector2 thumbnail_size2 = Vector2(thumbnail_size, thumbnail_size);

			if (item.pattern.is_valid() && !item.pattern->is_empty()) {
				// Generate the pattern preview
				SubViewport *viewport = memnew(SubViewport);
				viewport->set_size(thumbnail_size2);
				viewport->set_disable_input(true);
				viewport->set_transparent_background(true);
				viewport->set_update_mode(SubViewport::UPDATE_ONCE);

				LayeredTileMap *tile_map = memnew(LayeredTileMap);
				tile_map->set_tileset(item.tile_set);
				tile_map->set_pattern(0, Vector2(), item.pattern);
				viewport->add_child(tile_map);

				PoolVector2iArray used_cells = tile_map->get_used_cells(0);

				Rect2 encompassing_rect;
				encompassing_rect.set_position(tile_map->map_to_local(used_cells[0]));
				for (int i = 0; i < used_cells.size(); i++) {
					Vector2i cell = used_cells[i];
					Vector2 world_pos = tile_map->map_to_local(cell);
					encompassing_rect.expand_to(world_pos);

					// Texture.
					Ref<LayeredTileSetAtlasSource> atlas_source = item.tile_set->get_source(tile_map->get_cell_source_id(0, cell));
					if (atlas_source.is_valid()) {
						Vector2i coords = tile_map->get_cell_atlas_coords(0, cell);
						int alternative = tile_map->get_cell_alternative_tile(0, cell);

						if (atlas_source->has_tile(coords) && atlas_source->has_alternative_tile(coords, alternative)) {
							Vector2 center = world_pos - atlas_source->get_tile_data(coords, alternative)->get_texture_origin();
							encompassing_rect.expand_to(center - atlas_source->get_tile_texture_region(coords).size / 2);
							encompassing_rect.expand_to(center + atlas_source->get_tile_texture_region(coords).size / 2);
						}
					}
				}

				Vector2 scale = thumbnail_size2 / MAX(encompassing_rect.size.x, encompassing_rect.size.y);
				tile_map->set_scale(scale);
				tile_map->set_position(-(scale * encompassing_rect.get_center()) + thumbnail_size2 / 2);

				// Add the viewport at the last moment to avoid rendering too early.
				callable_mp((Node *)EditorNode::get_singleton(), &Node::add_child).call_deferred(viewport, false, Node::INTERNAL_MODE_DISABLED);

				RS::get_singleton()->connect(SNAME("frame_pre_draw"), callable_mp(const_cast<LayeredTilesEditorUtils *>(this), &LayeredTilesEditorUtils::_preview_frame_started), Object::CONNECT_ONE_SHOT);

				pattern_preview_done.wait();

				Ref<Image> image = viewport->get_texture()->get_image();

				// Find the index for the given pattern. TODO: optimize.
				item.callback.call(item.pattern, ImageTexture::create_from_image(image));

				viewport->queue_free();
			}
		}
	}
	pattern_thread_exited.set();
}

void LayeredTilesEditorUtils::queue_pattern_preview(Ref<LayeredTileSet> p_tile_set, Ref<LayeredTileMapPattern> p_pattern, Callable p_callback) {
	ERR_FAIL_COND(!p_tile_set.is_valid());
	ERR_FAIL_COND(!p_pattern.is_valid());
	{
		MutexLock lock(pattern_preview_mutex);
		pattern_preview_queue.push_back({ p_tile_set, p_pattern, p_callback });
	}
	pattern_preview_sem.post();
}

void LayeredTilesEditorUtils::set_sources_lists_current(int p_current) {
	atlas_sources_lists_current = p_current;
}

void LayeredTilesEditorUtils::synchronize_sources_list(Object *p_current_list, Object *p_current_sort_button) {
	ItemList *item_list = Object::cast_to<ItemList>(p_current_list);
	MenuButton *sorting_button = Object::cast_to<MenuButton>(p_current_sort_button);
	ERR_FAIL_NULL(item_list);
	ERR_FAIL_NULL(sorting_button);

	if (sorting_button->is_visible_in_tree()) {
		for (int i = 0; i != SOURCE_SORT_MAX; i++) {
			sorting_button->get_popup()->set_item_checked(i, (i == (int)source_sort));
		}
	}

	if (item_list->is_visible_in_tree()) {
		// Make sure the selection is not overwritten after sorting.
		int atlas_sources_lists_current_mem = atlas_sources_lists_current;
		item_list->emit_signal(SNAME("sort_request"));
		atlas_sources_lists_current = atlas_sources_lists_current_mem;

		if (atlas_sources_lists_current < 0 || atlas_sources_lists_current >= item_list->get_item_count()) {
			item_list->deselect_all();
		} else {
			item_list->set_current(atlas_sources_lists_current);
			item_list->ensure_current_is_visible();
			item_list->emit_signal(SNAME("item_selected"), atlas_sources_lists_current);
		}
	}
}

void LayeredTilesEditorUtils::set_atlas_view_transform(float p_zoom, Vector2 p_scroll) {
	atlas_view_zoom = p_zoom;
	atlas_view_scroll = p_scroll;
}

void LayeredTilesEditorUtils::synchronize_atlas_view(Object *p_current) {
	LayeredTileAtlasView *tile_atlas_view = Object::cast_to<LayeredTileAtlasView>(p_current);
	ERR_FAIL_NULL(tile_atlas_view);

	if (tile_atlas_view->is_visible_in_tree()) {
		tile_atlas_view->set_transform(atlas_view_zoom, atlas_view_scroll);
	}
}

void LayeredTilesEditorUtils::set_sorting_option(int p_option) {
	source_sort = p_option;
}

List<int> LayeredTilesEditorUtils::get_sorted_sources(const Ref<LayeredTileSet> p_tile_set) const {
	SourceNameComparator::tile_set = p_tile_set;
	List<int> source_ids;

	for (int i = 0; i < p_tile_set->get_source_count(); i++) {
		source_ids.push_back(p_tile_set->get_source_id(i));
	}

	switch (source_sort) {
		case SOURCE_SORT_ID_REVERSE:
			// Already sorted.
			source_ids.reverse();
			break;
		case SOURCE_SORT_NAME:
			source_ids.sort_custom<SourceNameComparator>();
			break;
		case SOURCE_SORT_NAME_REVERSE:
			source_ids.sort_custom<SourceNameComparator>();
			source_ids.reverse();
			break;
		default: // SOURCE_SORT_ID
			break;
	}

	SourceNameComparator::tile_set.unref();
	return source_ids;
}

Ref<LayeredTileSet> LayeredTilesEditorUtils::SourceNameComparator::tile_set;

bool LayeredTilesEditorUtils::SourceNameComparator::operator()(const int &p_a, const int &p_b) const {
	String name_a;
	String name_b;

	{
		LayeredTileSetSource *source = *tile_set->get_source(p_a);

		if (!source->get_name().is_empty()) {
			name_a = source->get_name();
		}

		LayeredTileSetAtlasSource *atlas_source = Object::cast_to<LayeredTileSetAtlasSource>(source);
		if (atlas_source) {
			Ref<Texture> texture = atlas_source->get_texture();
			if (name_a.is_empty() && texture.is_valid()) {
				name_a = texture->get_path().get_file();
			}
		}

		if (name_a.is_empty()) {
			name_a = itos(p_a);
		}
	}

	{
		LayeredTileSetSource *source = *tile_set->get_source(p_b);

		if (!source->get_name().is_empty()) {
			name_b = source->get_name();
		}

		LayeredTileSetAtlasSource *atlas_source = Object::cast_to<LayeredTileSetAtlasSource>(source);
		if (atlas_source) {
			Ref<Texture> texture = atlas_source->get_texture();
			if (name_b.is_empty() && texture.is_valid()) {
				name_b = texture->get_path().get_file();
			}
		}

		if (name_b.is_empty()) {
			name_b = itos(p_b);
		}
	}

	return NaturalNoCaseComparator()(name_a, name_b);
}

void LayeredTilesEditorUtils::display_tile_set_editor_panel() {
	tile_map_plugin_singleton->hide_editor();
	tile_set_plugin_singleton->make_visible(true);
}

void LayeredTilesEditorUtils::draw_selection_rect(CanvasItem *p_ci, const Rect2 &p_rect, const Color &p_color) {
	Ref<Texture> selection_texture = EditorNode::get_singleton()->get_editor_theme()->get_icon(SNAME("TileSelection"), EditorStringName(EditorIcons));

	real_t scale = p_ci->get_global_transform().get_scale().x * 0.5;
	p_ci->draw_set_transform(p_rect.position, 0, Vector2(1, 1) / scale);
	RS::get_singleton()->canvas_item_add_nine_patch(
			p_ci->get_canvas_item(), Rect2(Vector2(), p_rect.size * scale), Rect2(), selection_texture->get_rid(),
			Vector2(2, 2), Vector2(2, 2), RS::NINE_PATCH_STRETCH, RS::NINE_PATCH_STRETCH, false, p_color);
	p_ci->draw_set_transform_matrix(Transform2D());
}

LayeredTilesEditorUtils::LayeredTilesEditorUtils() {
	singleton = this;
	// Pattern preview generation thread.
	pattern_preview_thread.start(_thread_func, this);
}

LayeredTilesEditorUtils::~LayeredTilesEditorUtils() {
	if (pattern_preview_thread.is_started()) {
		pattern_thread_exit.set();
		pattern_preview_sem.post();
		while (!pattern_thread_exited.is_set()) {
			OS::get_singleton()->delay_usec(10000);
			RenderingServer::get_singleton()->sync(); //sync pending stuff, as thread may be blocked on visual server
		}
		pattern_preview_thread.wait_to_finish();
	}
	singleton = nullptr;
}

void LayeredTileMapEditorPlugin::_tile_map_layer_changed() {
	if (tile_map_changed_needs_update) {
		return;
	}
	tile_map_changed_needs_update = true;
	callable_mp(this, &LayeredTileMapEditorPlugin::_update_tile_map).call_deferred();
}

void LayeredTileMapEditorPlugin::_tile_map_layer_removed() {
	// Workaround for LayeredTileMap, making sure the editor stays open when you delete the currently edited layer.
	LayeredTileMap *tile_map = Object::cast_to<LayeredTileMap>(ObjectDB::get_instance(tile_map_group_id));
	if (tile_map) {
		edit(tile_map);
	}
}

void LayeredTileMapEditorPlugin::_update_tile_map() {
	LayeredTileMapLayer *edited_layer = Object::cast_to<LayeredTileMapLayer>(ObjectDB::get_instance(tile_map_layer_id));
	if (edited_layer) {
		Ref<LayeredTileSet> tile_set = edited_layer->get_effective_tile_set();
		if (tile_set.is_valid() && tile_set_id != tile_set->get_instance_id()) {
			tile_set_plugin_singleton->edit(tile_set.ptr());
			tile_set_plugin_singleton->make_visible(true);
			tile_set_id = tile_set->get_instance_id();
		} else if (tile_set.is_null()) {
			tile_set_plugin_singleton->edit(nullptr);
			tile_set_plugin_singleton->make_visible(false);
			tile_set_id = ObjectID();
		}
	}
	tile_map_changed_needs_update = false;
}

void LayeredTileMapEditorPlugin::_select_layer(const StringName &p_name) {
	LayeredTileMapLayer *edited_layer = Object::cast_to<LayeredTileMapLayer>(ObjectDB::get_instance(tile_map_layer_id));
	ERR_FAIL_NULL(edited_layer);

	Node *parent = edited_layer->get_parent();
	ERR_FAIL_NULL(parent);

	LayeredTileMapLayer *new_layer = Object::cast_to<LayeredTileMapLayer>(parent->get_node_or_null(String(p_name)));
	edit(new_layer);
}

void LayeredTileMapEditorPlugin::_edit_tile_map_layer(LayeredTileMapLayer *p_tile_map_layer) {
	ERR_FAIL_NULL(p_tile_map_layer);

	editor->edit(p_tile_map_layer);

	// Update the selected layers in the LayeredTileMapLayerGroup parent node.
	LayeredTileMapLayerGroup *tile_map_layer_group = Object::cast_to<LayeredTileMapLayerGroup>(p_tile_map_layer->get_parent());
	if (tile_map_layer_group) {
		Vector<StringName> selected;
		selected.push_back(p_tile_map_layer->get_name());
		tile_map_layer_group->set_selected_layers(selected);
	}

	// Update the object IDs.
	tile_map_layer_id = p_tile_map_layer->get_instance_id();
	p_tile_map_layer->connect("changed", callable_mp(this, &LayeredTileMapEditorPlugin::_tile_map_layer_changed));
	p_tile_map_layer->connect("tree_exited", callable_mp(this, &LayeredTileMapEditorPlugin::_tile_map_layer_removed));
	if (tile_map_layer_group) {
		tile_map_group_id = tile_map_layer_group->get_instance_id();
		tile_map_layer_group->connect("child_entered_tree", callable_mp(editor, &LayeredTileMapLayerEditor::update_layers_selector).unbind(1));
		tile_map_layer_group->connect("child_exiting_tree", callable_mp(editor, &LayeredTileMapLayerEditor::update_layers_selector).unbind(1));
		tile_map_layer_group->connect("child_order_changed", callable_mp(editor, &LayeredTileMapLayerEditor::update_layers_selector));
	}

	// Update the edited tileset.
	Ref<LayeredTileSet> tile_set = p_tile_map_layer->get_effective_tile_set();
	if (tile_set.is_valid()) {
		tile_set_plugin_singleton->edit(tile_set.ptr());
		tile_set_plugin_singleton->make_visible(true);
		tile_set_id = tile_set->get_instance_id();
	} else {
		tile_set_plugin_singleton->edit(nullptr);
		tile_set_plugin_singleton->make_visible(false);
	}
}

void LayeredTileMapEditorPlugin::_edit_tile_map_layer_group(LayeredTileMapLayerGroup *p_tile_map_layer_group) {
	ERR_FAIL_NULL(p_tile_map_layer_group);

	Vector<StringName> selected_layers = p_tile_map_layer_group->get_selected_layers();

	LayeredTileMapLayer *selected_layer = nullptr;
	if (selected_layers.size() > 0) {
		// Edit the selected layer.
		selected_layer = Object::cast_to<LayeredTileMapLayer>(p_tile_map_layer_group->get_node_or_null(String(selected_layers[0])));
	}
	if (!selected_layer) {
		// Edit the first layer found.
		for (int i = 0; i < p_tile_map_layer_group->get_child_count(); i++) {
			selected_layer = Object::cast_to<LayeredTileMapLayer>(p_tile_map_layer_group->get_child(i));
			if (selected_layer) {
				break;
			}
		}
	}

	if (selected_layer) {
		_edit_tile_map_layer(selected_layer);
	} else {
		editor->edit(nullptr);
	}
}

void LayeredTileMapEditorPlugin::_notification(int p_notification) {
	if (p_notification == NOTIFICATION_EXIT_TREE) {
		get_tree()->queue_delete(LayeredTilesEditorUtils::get_singleton());
	}
}

void LayeredTileMapEditorPlugin::edit(Object *p_object) {
	LayeredTileMapLayer *edited_layer = Object::cast_to<LayeredTileMapLayer>(ObjectDB::get_instance(tile_map_layer_id));
	if (edited_layer) {
		edited_layer->disconnect("changed", callable_mp(this, &LayeredTileMapEditorPlugin::_tile_map_layer_changed));
		edited_layer->disconnect("tree_exited", callable_mp(this, &LayeredTileMapEditorPlugin::_tile_map_layer_removed));
	}

	LayeredTileMapLayerGroup *tile_map_group = Object::cast_to<LayeredTileMapLayerGroup>(ObjectDB::get_instance(tile_map_group_id));
	if (tile_map_group) {
		tile_map_group->disconnect("child_entered_tree", callable_mp(editor, &LayeredTileMapLayerEditor::update_layers_selector).unbind(1));
		tile_map_group->disconnect("child_exiting_tree", callable_mp(editor, &LayeredTileMapLayerEditor::update_layers_selector).unbind(1));
		tile_map_group->disconnect("child_order_changed", callable_mp(editor, &LayeredTileMapLayerEditor::update_layers_selector));
	}

	tile_map_group_id = ObjectID();
	tile_map_layer_id = ObjectID();
	tile_set_id = ObjectID();

	LayeredTileMapLayerGroup *tile_map_layer_group = Object::cast_to<LayeredTileMap>(p_object);
	LayeredTileMapLayer *tile_map_layer = Object::cast_to<LayeredTileMapLayer>(p_object);
	if (tile_map_layer_group) {
		_edit_tile_map_layer_group(tile_map_layer_group);
	} else if (tile_map_layer) {
		_edit_tile_map_layer(tile_map_layer);
	} else {
		// Deselect the layer in the group.
		if (edited_layer) {
			tile_map_layer_group = Object::cast_to<LayeredTileMapLayerGroup>(edited_layer->get_parent());
			if (tile_map_layer_group) {
				tile_map_layer_group->set_selected_layers(Vector<StringName>());
			}
		}
	}
}

bool LayeredTileMapEditorPlugin::handles(Object *p_object) const {
	return Object::cast_to<LayeredTileMapLayer>(p_object) != nullptr || Object::cast_to<LayeredTileMapLayerGroup>(p_object) != nullptr;
}

void LayeredTileMapEditorPlugin::make_visible(bool p_visible) {
	if (p_visible) {
		button->show();
		EditorNode::get_bottom_panel()->make_item_visible(editor);
	} else {
		button->hide();
		if (editor->is_visible_in_tree()) {
			EditorNode::get_bottom_panel()->hide_bottom_panel();
		}
	}
}

bool LayeredTileMapEditorPlugin::forward_canvas_gui_input(const Ref<InputEvent> &p_event) {
	return editor->forward_canvas_gui_input(p_event);
}

void LayeredTileMapEditorPlugin::forward_canvas_draw_over_viewport(Control *p_overlay) {
	editor->forward_canvas_draw_over_viewport(p_overlay);
}

void LayeredTileMapEditorPlugin::hide_editor() {
	if (editor->is_visible_in_tree()) {
		EditorNode::get_bottom_panel()->hide_bottom_panel();
	}
}

bool LayeredTileMapEditorPlugin::is_editor_visible() const {
	return editor->is_visible_in_tree();
}

LayeredTileMapEditorPlugin::LayeredTileMapEditorPlugin() {
	if (!LayeredTilesEditorUtils::get_singleton()) {
		memnew(LayeredTilesEditorUtils);
	}
	tile_map_plugin_singleton = this;

	editor = memnew(LayeredTileMapLayerEditor);
	editor->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	editor->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	editor->set_custom_minimum_size(Size2(0, 200) * EDSCALE);
	editor->connect("change_selected_layer_request", callable_mp(this, &LayeredTileMapEditorPlugin::_select_layer));
	editor->hide();

	button = EditorNode::get_bottom_panel()->add_item(TTR("LayeredTileMap"), editor);
	button->hide();
}

LayeredTileMapEditorPlugin::~LayeredTileMapEditorPlugin() {
	tile_map_plugin_singleton = nullptr;
}

void LayeredTileSetEditorPlugin::edit(Object *p_object) {
	editor->edit(Ref<LayeredTileSet>(p_object));
	if (p_object) {
		edited_tileset = p_object->get_instance_id();
	} else {
		edited_tileset = ObjectID();
	}
}

bool LayeredTileSetEditorPlugin::handles(Object *p_object) const {
	return Object::cast_to<LayeredTileSet>(p_object) != nullptr;
}

void LayeredTileSetEditorPlugin::make_visible(bool p_visible) {
	if (p_visible) {
		button->show();
		if (!tile_map_plugin_singleton->is_editor_visible()) {
			EditorNode::get_bottom_panel()->make_item_visible(editor);
		}
	} else {
		button->hide();
		if (editor->is_visible_in_tree()) {
			EditorNode::get_bottom_panel()->hide_bottom_panel();
		}
	}
}

ObjectID LayeredTileSetEditorPlugin::get_edited_tileset() const {
	return edited_tileset;
}

LayeredTileSetEditorPlugin::LayeredTileSetEditorPlugin() {
	if (!LayeredTilesEditorUtils::get_singleton()) {
		memnew(LayeredTilesEditorUtils);
	}
	tile_set_plugin_singleton = this;

	editor = memnew(LayeredTileSetEditor);
	editor->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	editor->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	editor->set_custom_minimum_size(Size2(0, 200) * EDSCALE);
	editor->hide();

	button = EditorNode::get_bottom_panel()->add_item(TTR("LayeredTileSet"), editor);
	button->hide();
}

LayeredTileSetEditorPlugin::~LayeredTileSetEditorPlugin() {
	tile_set_plugin_singleton = nullptr;
}
