#ifndef CONTROL_H
#define CONTROL_H

/*************************************************************************/
/*  control.h                                                            */
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

#include "core/containers/rid.h"
#include "core/math/transform_2d.h"
#include "scene/main/canvas_item.h"
#include "scene/resources/theme.h"

class Viewport;
class Label;
class Panel;
class Texture;
class Shader;
class StyleBox;
class Font;
class Node;
class Timer;

class Control : public CanvasItem {
	GDCLASS(Control, CanvasItem);
	OBJ_CATEGORY("GUI Nodes");

public:
	enum Anchor {

		ANCHOR_BEGIN = 0,
		ANCHOR_END = 1
	};

	enum GrowDirection {
		GROW_DIRECTION_BEGIN,
		GROW_DIRECTION_END,
		GROW_DIRECTION_BOTH
	};

	enum FocusMode {
		FOCUS_NONE,
		FOCUS_CLICK,
		FOCUS_ALL
	};

	enum SizeFlags {

		SIZE_FILL = 1,
		SIZE_EXPAND = 2,
		SIZE_EXPAND_FILL = SIZE_EXPAND | SIZE_FILL,
		SIZE_SHRINK_CENTER = 4, //ignored by expand or fill
		SIZE_SHRINK_END = 8, //ignored by expand or fill

	};

	enum MouseFilter {
		MOUSE_FILTER_STOP,
		MOUSE_FILTER_PASS,
		MOUSE_FILTER_IGNORE
	};

	enum CursorShape {
		CURSOR_ARROW,
		CURSOR_IBEAM,
		CURSOR_POINTING_HAND,
		CURSOR_CROSS,
		CURSOR_WAIT,
		CURSOR_BUSY,
		CURSOR_DRAG,
		CURSOR_CAN_DROP,
		CURSOR_FORBIDDEN,
		CURSOR_VSIZE,
		CURSOR_HSIZE,
		CURSOR_BDIAGSIZE,
		CURSOR_FDIAGSIZE,
		CURSOR_MOVE,
		CURSOR_VSPLIT,
		CURSOR_HSPLIT,
		CURSOR_HELP,
		CURSOR_MAX
	};

	enum LayoutPreset {
		PRESET_TOP_LEFT,
		PRESET_TOP_RIGHT,
		PRESET_BOTTOM_LEFT,
		PRESET_BOTTOM_RIGHT,
		PRESET_CENTER_LEFT,
		PRESET_CENTER_TOP,
		PRESET_CENTER_RIGHT,
		PRESET_CENTER_BOTTOM,
		PRESET_CENTER,
		PRESET_LEFT_WIDE,
		PRESET_TOP_WIDE,
		PRESET_RIGHT_WIDE,
		PRESET_BOTTOM_WIDE,
		PRESET_VCENTER_WIDE,
		PRESET_HCENTER_WIDE,
		PRESET_WIDE
	};

	enum LayoutPresetMode {
		PRESET_MODE_MINSIZE,
		PRESET_MODE_KEEP_WIDTH,
		PRESET_MODE_KEEP_HEIGHT,
		PRESET_MODE_KEEP_SIZE
	};

private:
	struct CComparator {
		bool operator()(const Control *p_a, const Control *p_b) const {
			if (p_a->get_canvas_layer() == p_b->get_canvas_layer()) {
				return p_b->is_greater_than(p_a);
			}

			return p_a->get_canvas_layer() < p_b->get_canvas_layer();
		}
	};

	struct Data {
		Point2 pos_cache;
		Size2 size_cache;
		Size2 minimum_size_cache;
		bool minimum_size_valid;

		Size2 last_minimum_size;
		bool updating_last_minimum_size;

		float margin[4];
		float anchor[4];
		FocusMode focus_mode;
		GrowDirection h_grow;
		GrowDirection v_grow;

		float rotation;
		Vector2 scale;
		Vector2 pivot_offset;

		bool pending_resize;

		int h_size_flags;
		int v_size_flags;
		float expand;
		Point2 custom_minimum_size;

		bool pass_on_modal_close_click;

		MouseFilter mouse_filter;

		bool clip_contents;

		bool block_minimum_size_adjust;
		bool disable_visibility_clip;

		Control *parent;
		ObjectID drag_owner;
		bool modal_exclusive;
		uint64_t modal_frame; //frame used to put something as modal
		Ref<Theme> theme;
		Control *theme_owner;
		StringName theme_type_variation;
		String tooltip;
		CursorShape default_cursor;

		List<Control *>::Element *MI; //modal item
		List<Control *>::Element *SI;
		List<Control *>::Element *RI;

		CanvasItem *parent_canvas_item;

		ObjectID modal_prev_focus_owner;

		NodePath focus_neighbour[4];
		NodePath focus_next;
		NodePath focus_prev;

		ObjectID shortcut_context;
		NodePath shortcut_context_path;
		mutable ObjectID shortcut_context_path_cache;

		HashMap<StringName, Ref<Texture>> icon_override;
		HashMap<StringName, Ref<Shader>> shader_override;
		HashMap<StringName, Ref<StyleBox>> style_override;
		HashMap<StringName, Ref<Font>> font_override;
		HashMap<StringName, Color> color_override;
		HashMap<StringName, int> constant_override;

		mutable HashMap<StringName, HashMap<StringName, Ref<Texture>>> theme_icon_cache;
		mutable HashMap<StringName, HashMap<StringName, Ref<StyleBox>>> theme_style_cache;
		mutable HashMap<StringName, HashMap<StringName, Ref<Font>>> theme_font_cache;
		mutable HashMap<StringName, HashMap<StringName, Color>> theme_color_cache;
		mutable HashMap<StringName, HashMap<StringName, int>> theme_constant_cache;

	} data;

	void _window_find_focus_neighbour(const Vector2 &p_dir, Node *p_at, const Point2 *p_points, float p_min, float &r_closest_dist, Control **r_closest);
	Control *_get_focus_neighbour(Margin p_margin, int p_count = 0);

	void _set_anchor(Margin p_margin, float p_anchor);
	void _set_position(const Point2 &p_point);
	void _set_global_position(const Point2 &p_point);
	void _set_size(const Size2 &p_size);

	void _propagate_theme_changed(CanvasItem *p_at, Control *p_owner, bool p_assign = true);
	void _theme_changed();
	void _invalidate_theme_cache();

	void _change_notify_margins();
	void _update_minimum_size();

	void _resize(const Size2 &p_size);

	void _compute_margins(Rect2 p_rect, const float p_anchors[4], float (&r_margins)[4]);
	void _compute_anchors(Rect2 p_rect, const float p_margins[4], float (&r_anchors)[4]);

	void _size_changed();
	String _get_tooltip() const;

	void _override_changed();

	void _update_canvas_item_transform();

	Transform2D _get_internal_transform() const;

	friend class Viewport;
	void _modal_stack_remove();
	void _modal_set_prev_focus_owner(ObjectID p_prev);

	void _update_minimum_size_cache();

	template <class T>
	static T get_theme_item_in_types(Control *p_theme_owner, Theme::DataType p_data_type, const StringName &p_name, List<StringName> p_theme_types);
	static bool has_theme_item_in_types(Control *p_theme_owner, Theme::DataType p_data_type, const StringName &p_name, List<StringName> p_theme_types);
	_FORCE_INLINE_ void _get_theme_type_dependencies(const StringName &p_theme_type, List<StringName> *p_list) const;

protected:
	virtual void add_child_notify(Node *p_child);
	virtual void remove_child_notify(Node *p_child);

	//virtual void _window_gui_input(InputEvent p_event);
	void set_modal_exclusive(bool p_exclusive);

	bool _set(const StringName &p_name, const Variant &p_value);
	bool _get(const StringName &p_name, Variant &r_ret) const;
	void _get_property_list(List<PropertyInfo> *p_list) const;

	void _notification(int p_notification);
	static void _bind_methods();
	virtual void _validate_property(PropertyInfo &property) const;

	//bind helpers

public:
	enum {

		/*		NOTIFICATION_DRAW=30,
		NOTIFICATION_VISIBILITY_CHANGED=38*/
		NOTIFICATION_RESIZED = 40,
		NOTIFICATION_MOUSE_ENTER = 41,
		NOTIFICATION_MOUSE_EXIT = 42,
		NOTIFICATION_FOCUS_ENTER = 43,
		NOTIFICATION_FOCUS_EXIT = 44,
		NOTIFICATION_THEME_CHANGED = 45,
		NOTIFICATION_MODAL_CLOSE = 46,
		NOTIFICATION_SCROLL_BEGIN = 47,
		NOTIFICATION_SCROLL_END = 48,

	};

	/* EDITOR */
#ifdef TOOLS_ENABLED
	virtual Dictionary _edit_get_state() const;
	virtual void _edit_set_state(const Dictionary &p_state);

	virtual void _edit_set_position(const Point2 &p_position);
	virtual Point2 _edit_get_position() const;

	virtual void _edit_set_scale(const Size2 &p_scale);
	virtual Size2 _edit_get_scale() const;

	virtual void _edit_set_rect(const Rect2 &p_edit_rect);
	virtual Rect2 _edit_get_rect() const;
	virtual bool _edit_use_rect() const;

	virtual void _edit_set_rotation(float p_rotation);
	virtual float _edit_get_rotation() const;
	virtual bool _edit_use_rotation() const;

	virtual void _edit_set_pivot(const Point2 &p_pivot);
	virtual Point2 _edit_get_pivot() const;
	virtual bool _edit_use_pivot() const;

	virtual Size2 _edit_get_minimum_size() const;
#endif

	void accept_event();

	virtual Size2 get_minimum_size() const;
	virtual Size2 get_combined_minimum_size() const;
	virtual bool has_point(const Point2 &p_point) const;
	virtual void set_drag_forwarding(Control *p_target);
	virtual Variant get_drag_data(const Point2 &p_point);
	virtual bool can_drop_data(const Point2 &p_point, const Variant &p_data) const;
	virtual void drop_data(const Point2 &p_point, const Variant &p_data);
	void set_drag_preview(Control *p_control);
	void force_drag(const Variant &p_data, Control *p_control);
	bool is_drag_successful() const;

	void set_custom_minimum_size(const Size2 &p_custom);
	Size2 get_custom_minimum_size() const;

	bool is_window_modal_on_top() const;
	uint64_t get_modal_frame() const; //frame in which this was made modal

	Control *get_parent_control() const;

	/* POSITIONING */

	void set_anchors_preset(LayoutPreset p_preset, bool p_keep_margins = true);
	void set_margins_preset(LayoutPreset p_preset, LayoutPresetMode p_resize_mode = PRESET_MODE_MINSIZE, int p_margin = 0);
	void set_anchors_and_margins_preset(LayoutPreset p_preset, LayoutPresetMode p_resize_mode = PRESET_MODE_MINSIZE, int p_margin = 0);

	void set_anchor(Margin p_margin, float p_anchor, bool p_keep_margin = true, bool p_push_opposite_anchor = true);
	float get_anchor(Margin p_margin) const;

	void set_margin(Margin p_margin, float p_value);
	float get_margin(Margin p_margin) const;

	void set_anchor_and_margin(Margin p_margin, float p_anchor, float p_pos, bool p_push_opposite_anchor = true);

	void set_begin(const Point2 &p_point); // helper
	void set_end(const Point2 &p_point); // helper

	Point2 get_begin() const;
	Point2 get_end() const;

	void set_position(const Point2 &p_point, bool p_keep_margins = false);
	void set_global_position(const Point2 &p_point, bool p_keep_margins = false);
	Point2 get_position() const;
	Point2 get_global_position() const;

	void set_size(const Size2 &p_size, bool p_keep_margins = false);
	Size2 get_size() const;

	Rect2 get_rect() const;
	Rect2 get_global_rect() const;
	Rect2 get_window_rect() const; ///< use with care, as it blocks waiting for the visual server
	Rect2 get_anchorable_rect() const;

	void set_rotation(float p_radians);
	void set_rotation_degrees(float p_degrees);
	float get_rotation() const;
	float get_rotation_degrees() const;

	void set_h_grow_direction(GrowDirection p_direction);
	GrowDirection get_h_grow_direction() const;

	void set_v_grow_direction(GrowDirection p_direction);
	GrowDirection get_v_grow_direction() const;

	void set_pivot_offset(const Vector2 &p_pivot);
	Vector2 get_pivot_offset() const;

	void set_scale(const Vector2 &p_scale);
	Vector2 get_scale() const;

	void show_modal(bool p_exclusive = false);

	void set_theme(const Ref<Theme> &p_theme);
	Ref<Theme> get_theme() const;

	void set_theme_type_variation(const StringName &p_theme_type);
	StringName get_theme_type_variation() const;

	void set_h_size_flags(int p_flags);
	int get_h_size_flags() const;

	void set_v_size_flags(int p_flags);
	int get_v_size_flags() const;

	void set_stretch_ratio(float p_ratio);
	float get_stretch_ratio() const;

	void minimum_size_changed();

	/* FOCUS */

	void set_focus_mode(FocusMode p_focus_mode);
	FocusMode get_focus_mode() const;
	bool has_focus() const;
	void grab_focus();
	void release_focus();

	Control *find_next_valid_focus() const;
	Control *find_prev_valid_focus() const;

	void set_focus_neighbour(Margin p_margin, const NodePath &p_neighbour);
	NodePath get_focus_neighbour(Margin p_margin) const;

	void set_focus_next(const NodePath &p_next);
	NodePath get_focus_next() const;
	void set_focus_previous(const NodePath &p_prev);
	NodePath get_focus_previous() const;

	Control *get_focus_owner() const;

	void set_mouse_filter(MouseFilter p_filter);
	MouseFilter get_mouse_filter() const;

	void set_pass_on_modal_close_click(bool p_pass_on);
	bool get_pass_on_modal_close_click() const;

	/* SKINNING */

	void add_theme_icon_override(const StringName &p_name, const Ref<Texture> &p_icon);
	void add_theme_shader_override(const StringName &p_name, const Ref<Shader> &p_shader);
	void add_theme_style_override(const StringName &p_name, const Ref<StyleBox> &p_style);
	void add_theme_font_override(const StringName &p_name, const Ref<Font> &p_font);
	void add_theme_color_override(const StringName &p_name, const Color &p_color);
	void add_theme_constant_override(const StringName &p_name, int p_constant);

	void remove_theme_icon_override(const StringName &p_name);
	void remove_theme_shader_override(const StringName &p_name);
	void remove_theme_stylebox_override(const StringName &p_name);
	void remove_theme_font_override(const StringName &p_name);
	void remove_theme_color_override(const StringName &p_name);
	void remove_theme_constant_override(const StringName &p_name);

	Ref<Texture> get_theme_icon(const StringName &p_name, const StringName &p_theme_type = StringName()) const;
	Ref<Shader> get_theme_shader(const StringName &p_name, const StringName &p_theme_type = StringName()) const;
	Ref<StyleBox> get_theme_stylebox(const StringName &p_name, const StringName &p_theme_type = StringName()) const;
	Ref<Font> get_theme_font(const StringName &p_name, const StringName &p_theme_type = StringName()) const;
	Color get_theme_color(const StringName &p_name, const StringName &p_theme_type = StringName()) const;
	int get_theme_constant(const StringName &p_name, const StringName &p_theme_type = StringName()) const;

	bool has_theme_icon_override(const StringName &p_name) const;
	bool has_theme_shader_override(const StringName &p_name) const;
	bool has_theme_stylebox_override(const StringName &p_name) const;
	bool has_theme_font_override(const StringName &p_name) const;
	bool has_theme_color_override(const StringName &p_name) const;
	bool has_theme_constant_override(const StringName &p_name) const;

	bool has_theme_icon(const StringName &p_name, const StringName &p_theme_type = StringName()) const;
	bool has_theme_shader(const StringName &p_name, const StringName &p_theme_type = StringName()) const;
	bool has_theme_stylebox(const StringName &p_name, const StringName &p_theme_type = StringName()) const;
	bool has_theme_font(const StringName &p_name, const StringName &p_theme_type = StringName()) const;
	bool has_theme_color(const StringName &p_name, const StringName &p_theme_type = StringName()) const;
	bool has_theme_constant(const StringName &p_name, const StringName &p_theme_type = StringName()) const;

	Ref<Font> get_theme_default_font() const;

	/* TOOLTIP */

	void set_tooltip(const String &p_tooltip);
	virtual String get_tooltip(const Point2 &p_pos) const;
	virtual Control *make_custom_tooltip(const String &p_text) const;

	/* CURSOR */

	void set_default_cursor_shape(CursorShape p_shape);
	CursorShape get_default_cursor_shape() const;
	virtual CursorShape get_cursor_shape(const Point2 &p_pos = Point2i()) const;

	virtual Transform2D get_transform() const;

	bool is_toplevel_control() const;

	Size2 get_parent_area_size() const;
	Rect2 get_parent_anchorable_rect() const;

	void grab_click_focus();

	void warp_mouse(const Point2 &p_to_pos);

	void set_shortcut_context(const Node *p_node);
	Node *get_shortcut_context() const;

	void set_shortcut_context_path(const NodePath &p_node_path);
	NodePath get_shortcut_context_path() const;

	bool is_focus_owner_in_shortcut_context() const;

	virtual bool is_text_field() const;

	Control *get_root_parent_control() const;

	void set_clip_contents(bool p_clip);
	bool is_clipping_contents();

	void set_block_minimum_size_adjust(bool p_block);
	bool is_minimum_size_adjust_blocked() const;

	void set_disable_visibility_clip(bool p_ignore);
	bool is_visibility_clip_disabled() const;

	virtual void get_argument_options(const StringName &p_function, int p_idx, List<String> *r_options, const String &quote_style) const;
	virtual String get_configuration_warning() const;

	Control();
	~Control();
};

VARIANT_ENUM_CAST(Control::FocusMode);
VARIANT_ENUM_CAST(Control::SizeFlags);
VARIANT_ENUM_CAST(Control::CursorShape);
VARIANT_ENUM_CAST(Control::LayoutPreset);
VARIANT_ENUM_CAST(Control::LayoutPresetMode);
VARIANT_ENUM_CAST(Control::MouseFilter);
VARIANT_ENUM_CAST(Control::GrowDirection);
VARIANT_ENUM_CAST(Control::Anchor);

#endif
