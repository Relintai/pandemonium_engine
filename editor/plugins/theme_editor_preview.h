#ifndef THEME_EDITOR_PREVIEW_H
#define THEME_EDITOR_PREVIEW_H

/*************************************************************************/
/*  theme_editor_preview.h                                               */
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

#include "scene/gui/box_container.h"

#include "core/math/color.h"
#include "core/object/object.h"
#include "core/object/reference.h"
#include "core/string/ustring.h"

#include "scene/resources/font/font.h"
#include "scene/resources/packed_scene.h"
#include "scene/resources/style_box.h"
#include "scene/resources/theme.h"

class Button;
class ColorRect;
class Control;
class InputEvent;
class MarginContainer;
class ScrollContainer;
class Theme;
struct Vector2;

class ThemeEditorPreview : public VBoxContainer {
	GDCLASS(ThemeEditorPreview, VBoxContainer);

	ScrollContainer *preview_container;
	ColorRect *preview_bg;
	MarginContainer *preview_overlay;
	Control *picker_overlay;
	Control *hovered_control = nullptr;

	struct ThemeCache {
		Ref<StyleBox> preview_picker_overlay;
		Color preview_picker_overlay_color;
		Ref<StyleBox> preview_picker_label;
		Ref<Font> preview_picker_font;
	} theme_cache;

	double time_left = 0;

	void _propagate_redraw(Control *p_at);
	void _refresh_interval();
	void _preview_visibility_changed();

	void _picker_button_cbk();
	Control *_find_hovered_control(Control *p_parent, Vector2 p_mouse_position);

	void _draw_picker_overlay();
	void _gui_input_picker_overlay(const Ref<InputEvent> &p_event);
	void _reset_picker_overlay();

protected:
	HBoxContainer *preview_toolbar;
	MarginContainer *preview_content;
	Button *picker_button;

	void add_preview_overlay(Control *p_overlay);

	void _notification(int p_what);
	static void _bind_methods();

public:
	void set_preview_theme(const Ref<Theme> &p_theme);

	ThemeEditorPreview();
};

class DefaultThemeEditorPreview : public ThemeEditorPreview {
	GDCLASS(DefaultThemeEditorPreview, ThemeEditorPreview);

public:
	DefaultThemeEditorPreview();
};

class SceneThemeEditorPreview : public ThemeEditorPreview {
	GDCLASS(SceneThemeEditorPreview, ThemeEditorPreview);

	Ref<PackedScene> loaded_scene;

	Button *reload_scene_button;

	void _reload_scene();

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	bool set_preview_scene(const String &p_path);
	String get_preview_scene_path() const;

	SceneThemeEditorPreview();
};

#endif // THEME_EDITOR_PREVIEW_H
