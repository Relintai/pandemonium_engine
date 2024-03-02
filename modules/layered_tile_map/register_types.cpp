/*************************************************************************/
/*  register_types.cpp                                                   */
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

#include "register_types.h"

#include "geometry_parser/layered_tilemap_navigation_geometry_parser_2d.h"
#include "servers/navigation/navigation_mesh_generator.h"

#include "layered_tile_map_layer_group.h"
#include "layered_tile_map_layer.h"
#include "layered_tile_map.h"
#include "layered_tile_set.h"

#ifdef TOOLS_ENABLED
#include "editor/layered_tiles_editor_plugin.h"
#endif

void register_layered_tile_map_types(ModuleRegistrationLevel p_level) {
	if (p_level == MODULE_REGISTRATION_LEVEL_SCENE) {
		ClassDB::register_class<LayeredTileData>();
		ClassDB::register_class<LayeredTileMap>();
		ClassDB::register_class<LayeredTileMapLayerGroup>();
		ClassDB::register_class<LayeredTileMapPattern>();
		ClassDB::register_class<LayeredTileSet>();
		ClassDB::register_class<LayeredTileSetAtlasSource>();
		ClassDB::register_class<LayeredTileSetScenesCollectionSource>();
		ClassDB::register_class<LayeredTileSetSource>();

		NavigationMeshGenerator::get_singleton()->register_geometry_parser_2d(memnew(LayeredTileMap2DNavigationGeometryParser2D));
	}

#ifdef TOOLS_ENABLED
	if (p_level == MODULE_REGISTRATION_LEVEL_EDITOR) {
		EditorPlugins::add_by_type<LayeredTileMapEditorPlugin>();
		EditorPlugins::add_by_type<LayeredTileSetEditorPlugin>();
	}
#endif
}

void unregister_layered_tile_map_types(ModuleRegistrationLevel p_level) {
}
