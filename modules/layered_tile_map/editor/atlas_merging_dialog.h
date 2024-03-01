#ifndef ATLAS_MERGING_DIALOG_H
#define ATLAS_MERGING_DIALOG_H

/*************************************************************************/
/*  atlas_merging_dialog.h                                               */
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
#include "editor/editor_properties.h"
#include "scene/gui/dialogs.h"
#include "scene/gui/item_list.h"
#include "scene/gui/texture_rect.h"

class EditorFileDialog;
class EditorPropertyVector2i;

class AtlasMergingDialog : public ConfirmationDialog {
	GDCLASS(AtlasMergingDialog, ConfirmationDialog);

private:
	int commited_actions_count = 0;
	bool delete_original_atlases = true;
	Ref<LayeredTileSetAtlasSource> merged;
	LocalVector<HashMap<Vector2i, Vector2i>> merged_mapping;
	Ref<LayeredTileSet> tile_set;

	// Settings.
	int next_line_after_column = 30;

	// GUI.
	ItemList *atlas_merging_atlases_list = nullptr;
	EditorPropertyVector2i *texture_region_size_editor_property = nullptr;
	EditorPropertyInteger *columns_editor_property = nullptr;
	TextureRect *preview = nullptr;
	Label *select_2_atlases_label = nullptr;
	EditorFileDialog *editor_file_dialog = nullptr;
	Button *merge_button = nullptr;

	void _property_changed(const StringName &p_property, const Variant &p_value, const String &p_field, bool p_changing);

	void _generate_merged(const Vector<Ref<LayeredTileSetAtlasSource>> &p_atlas_sources, int p_max_columns);
	void _update_texture();
	void _merge_confirmed(const String &p_path);

protected:
	virtual void ok_pressed() override;
	virtual void cancel_pressed() override;
	virtual void custom_action(const String &) override;

	bool _set(const StringName &p_name, const Variant &p_value);
	bool _get(const StringName &p_name, Variant &r_ret) const;

	void _notification(int p_what);

public:
	void update_tile_set(Ref<LayeredTileSet> p_tile_set);

	AtlasMergingDialog();
};

#endif // ATLAS_MERGING_DIALOG_H
