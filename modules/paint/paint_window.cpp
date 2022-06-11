/*
Copyright (c) 2019 Flairieve
Copyright (c) 2020-2022 cobrapitz
Copyright (c) 2022 Péter Magyar

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "paint_window.h"

#include "core/image.h"
#include "core/os/input.h"
#include "scene/resources/texture.h"

#include "actions/brighten_action.h"
#include "actions/brush_action.h"
#include "actions/bucket_action.h"
#include "actions/cut_action.h"
#include "actions/darken_action.h"
#include "actions/line_action.h"
#include "actions/multiline_action.h"
#include "actions/paste_cut_action.h"
#include "actions/pencil_action.h"
#include "actions/rainbow_action.h"
#include "actions/rect_action.h"

#include "paint_canvas.h"
#include "paint_canvas_background.h"
#include "paint_canvas_layer.h"
#include "paint_color_grid.h"
#include "paint_layer_button.h"
#include "paint_navbar.h"
#include "paint_utilities.h"
#include "scene/resources/style_box.h"

#include "scene/gui/button.h"
#include "scene/gui/check_button.h"
#include "scene/gui/color_picker.h"
#include "scene/gui/label.h"
#include "scene/gui/margin_container.h"
#include "scene/gui/panel_container.h"
#include "scene/gui/rich_text_label.h"
#include "scene/gui/scroll_container.h"
#include "scene/gui/slider.h"
#include "scene/gui/texture_button.h"
#include "scene/gui/texture_rect.h"

#include "dialogs/paint_canvas_dialog.h"
#include "dialogs/paint_change_grid_size_dialog.h"
#include "dialogs/paint_load_file_dialog.h"
#include "dialogs/paint_save_file_dialog.h"
#include "paint_settings.h"

#include "icons/paint_icons.h"

static float scale = 1;

template <class T>
static Ref<Texture> make_icon(T p_src) {
	Ref<ImageTexture> texture(memnew(ImageTexture));
	Ref<Image> img = memnew(Image(p_src));
	if (scale > 1) {
		Size2 orig_size = Size2(img->get_width(), img->get_height());

		img->convert(Image::FORMAT_RGBA8);
		img->expand_x2_hq2x();
		if (scale != 2.0) {
			img->resize(orig_size.x * scale, orig_size.y * scale);
		}
	} else if (scale < 1) {
		Size2 orig_size = Size2(img->get_width(), img->get_height());
		img->convert(Image::FORMAT_RGBA8);
		img->resize(orig_size.x * scale, orig_size.y * scale);
	}
	texture->create_from_image(img, ImageTexture::FLAG_FILTER);

	return texture;
}

Color PaintWindow::get_selected_color() {
	return _selected_color;
}

void PaintWindow::set_selected_color(const Color &color) {
	_selected_color = color;
}

void PaintWindow::_input(const Ref<InputEvent> &event) {
	if (!is_visible_in_tree()) {
		return;
	}

	if (is_any_menu_open()) {
		return;
	}

	Ref<InputEventKey> iek = event;

	if (iek.is_valid() && iek->is_pressed() && !iek->is_echo()) {
		_handle_shortcuts(iek->get_scancode());
		return;
	}

	if (is_mouse_in_canvas() && paint_canvas->mouse_on_top) {
		_handle_zoom(event);
	}

	if (paint_canvas->is_active_layer_locked()) {
		return;
	}

	Ref<InputEventMouseButton> iemb = event;

	if (iemb.is_valid()) {
		if (brush_mode == Tools::CUT) {
			if (iemb->get_button_index() == BUTTON_LEFT && !iemb->is_pressed()) {
				commit_action();
			}
		}

		if (paint_canvas->mouse_in_region && paint_canvas->mouse_on_top) {
			switch (brush_mode) {
				case Tools::BUCKET: {
					if (iemb->get_button_index() == BUTTON_LEFT && iemb->is_pressed()) {
						if (!_current_action.is_valid()) {
							_current_action = get_action();
						}

						Array arr;
						arr.append(cell_mouse_position);
						arr.append(last_cell_mouse_position);
						arr.append(_selected_color);

						do_action(arr);
					}
				} break;
				case Tools::COLORPICKER: {
					if (iemb->get_button_index() == BUTTON_LEFT) {
						if (iemb->is_pressed()) {
							Color c = paint_canvas->get_pixel(cell_mouse_position.x, cell_mouse_position.y);

							if (c.a < 0.00001) {
								return;
							}

							_selected_color = c;
							_picked_color = true;

							color_grid->add_color_prefab(_selected_color);
						} else {
							if (_picked_color) {
								set_brush(_previous_tool);
							}
						}
					} else {
						if (iemb->get_button_index() == BUTTON_RIGHT && iemb->is_pressed()) {
							set_brush(_previous_tool);
						}
					}
				} break;
				case Tools::PASTECUT: {
					if (iemb->get_button_index() == BUTTON_RIGHT && iemb->is_pressed()) {
						commit_action();
						set_brush(Tools::PAINT);
					}
				} break;
				default:
					break;
			}
		}
	}
}
void PaintWindow::_process(float delta) {
	if (!is_visible_in_tree()) {
		return;
	}

	if (is_any_menu_open()) {
		return;
	}

	if (is_mouse_in_canvas()) {
		_handle_scroll();
	}

	//Update commonly used variables
	int grid_size = paint_canvas->get_pixel_size();
	mouse_position = get_global_mouse_position(); //paint_canvas.get_local_mouse_position()
	canvas_position = paint_canvas->get_global_position();
	canvas_mouse_position = Vector2(mouse_position.x - canvas_position.x, mouse_position.y - canvas_position.y);

	if (is_mouse_in_canvas() && paint_canvas->mouse_on_top) {
		cell_mouse_position = Vector2(floor(canvas_mouse_position.x / grid_size), floor(canvas_mouse_position.y / grid_size));
		cell_color = paint_canvas->get_pixel(cell_mouse_position.x, cell_mouse_position.y);
	}

	update_text_info();

	//	if not is_mouse_in_canvas():
	//		paint_canvas.tool_layer.clear()
	//		paint_canvas.update()
	//		paint_canvas.tool_layer.update_texture()
	//	else:

	if (is_mouse_in_canvas() && paint_canvas->mouse_on_top) {
		if (!paint_canvas->is_active_layer_locked()) {
			if (is_position_in_canvas(get_global_mouse_position()) || is_position_in_canvas(_last_mouse_pos_canvas_area)) {
				brush_process();
			}
			//else {
			//print(cell_mouse_position, " not in ", paint_canvas_container_node.rect_size);
			//print("not in canvas");
			//}
		}

		_draw_tool_brush();
	}

	//Update last variables with the current variables

	last_mouse_position = mouse_position;
	last_canvas_position = canvas_position;
	last_canvas_mouse_position = canvas_mouse_position;
	last_cell_mouse_position = cell_mouse_position;
	last_cell_color = cell_color;

	_last_mouse_pos_canvas_area = get_global_mouse_position(); //paint_canvas_container_node.get_local_mouse_position()
}

void PaintWindow::_handle_shortcuts(const int scancode) {
	switch (scancode) {
		case K_UNDO:
			undo_action();
			break;
		case K_REDO:
			redo_action();
			break;
		case K_PENCIL:
			set_brush(Tools::PAINT);
			break;
		case K_BRUSH:
			set_brush(Tools::BRUSH);
			break;
		case K_BUCKET:
			set_brush(Tools::BUCKET);
			break;
		case K_RAINBOW:
			set_brush(Tools::RAINBOW);
			break;
		case K_LINE:
			set_brush(Tools::LINE);
			break;
		case K_DARK:
			set_brush(Tools::DARKEN);
			break;
		case K_BRIGHT:
			set_brush(Tools::BRIGHTEN);
			break;
		case K_CUT:
			set_brush(Tools::CUT);
			break;
		case K_PICK:
			set_brush(Tools::COLORPICKER);
			break;
	}
}

void PaintWindow::_draw_tool_brush() {
	paint_canvas->tool_layer->clear();

	switch (brush_mode) {
		case Tools::PASTECUT: {
			for (int idx = 0; idx < _selection_cells.size(); ++idx) {
				Vector2i pixel = _selection_cells[idx];
				//if pixel.x < 0 || pixel.y < 0:
				//	print(pixel);
				Color color = _selection_colors[idx];
				pixel -= _cut_pos + _cut_size / 2;
				pixel += cell_mouse_position;
				paint_canvas->_set_pixel_v(paint_canvas->tool_layer, pixel, color);
			}
		} break;
		case Tools::BRUSH: {
			PoolVector2iArray pixels = BrushPrefabs::get_brush(selected_brush_prefab, brush_size_slider->get_value());

			PoolVector2iArray::Read r = pixels.read();

			for (int i = 0; i < pixels.size(); ++i) {
				Vector2i pixel = r[i];
				paint_canvas->_set_pixel(paint_canvas->tool_layer, cell_mouse_position.x + pixel.x, cell_mouse_position.y + pixel.y, _selected_color);
			}

			r.release();
		} break;
		case Tools::RAINBOW: {
			paint_canvas->_set_pixel(paint_canvas->tool_layer, cell_mouse_position.x, cell_mouse_position.y, Color(0.46875, 0.446777, 0.446777, 0.196078));
		} break;
		case Tools::COLORPICKER: {
			paint_canvas->_set_pixel(paint_canvas->tool_layer, cell_mouse_position.x, cell_mouse_position.y, Color(0.866667, 0.847059, 0.847059, 0.196078));
		} break;
		default: {
			paint_canvas->_set_pixel(paint_canvas->tool_layer, cell_mouse_position.x, cell_mouse_position.y, _selected_color);
		} break;
	}

	paint_canvas->update();
	//TODO add here brush prefab drawing
	paint_canvas->tool_layer->update_texture();
}

void PaintWindow::_handle_scroll() {
	if (Input::get_singleton()->is_mouse_button_pressed(BUTTON_MIDDLE)) {
		if (!_middle_mouse_pressed) {
			_middle_mouse_pressed_start_pos = paint_canvas->get_position();
			_middle_mouse_pressed_pos = get_global_mouse_position();

			_middle_mouse_pressed = true;
		}

		paint_canvas->set_position(_middle_mouse_pressed_start_pos + (get_global_mouse_position() - _middle_mouse_pressed_pos));

	} else if (!_middle_mouse_pressed) {
		_middle_mouse_pressed = false;
	}
}

void PaintWindow::_handle_zoom(const Ref<InputEvent> &event) {
	Ref<InputEventMouseButton> iemb = event;

	if (!iemb.is_valid()) {
		return;
	}

	if (iemb->is_pressed()) {
		if (iemb->get_button_index() == BUTTON_WHEEL_UP) {
			int pixel_size = paint_canvas->get_pixel_size();
			int px = MIN(pixel_size * 2, max_zoom_in);

			if (px == pixel_size) {
				return;
			}

			paint_canvas->set_pixel_size(px);

			paint_canvas->canvas_background->set_pixel_size(8 * pow(0.5, big_grid_pixels) / paint_canvas->get_pixel_size());

			Point2 pos = paint_canvas->get_position();
			Size2 size = paint_canvas->get_size();
			Size2 tsize = get_size();

			pos -= paint_canvas->get_local_mouse_position();
			pos.x = CLAMP(pos.x, -size.x * 0.8, tsize.x);
			pos.y = CLAMP(pos.y, -size.y * 0.8, tsize.y);

			paint_canvas->set_position(pos);

		} else if (iemb->get_button_index() == BUTTON_WHEEL_DOWN) {
			int pixel_size = paint_canvas->get_pixel_size();
			int px = MAX(pixel_size / 2.0, max_zoom_out);

			if (px == pixel_size) {
				return;
			}

			paint_canvas->set_pixel_size(px);

			paint_canvas->canvas_background->set_pixel_size(8 * pow(0.5, big_grid_pixels) / paint_canvas->get_pixel_size());

			Point2 pos = paint_canvas->get_position();
			Size2 size = paint_canvas->get_size();
			Size2 tsize = get_size();

			pos += paint_canvas->get_local_mouse_position() / 2;
			pos.x = CLAMP(pos.x, -size.x * 0.8, tsize.x);
			pos.y = CLAMP(pos.y, -size.y * 0.8, tsize.y);

			paint_canvas->set_position(pos);
		}
	}
}

void PaintWindow::_handle_cut() {
	if (Input::get_singleton()->is_mouse_button_pressed(BUTTON_RIGHT)) {
		paint_canvas->clear_preview_layer();
		set_brush(_previous_tool);
		return;
	}

	if (Input::get_singleton()->is_mouse_button_pressed(BUTTON_LEFT)) {
		PoolVector2iArray pixels = PaintUtilities::get_pixels_in_line(cell_mouse_position, last_cell_mouse_position);

		PoolVector2iArray::Read r = pixels.read();
		for (int i = 0; i < pixels.size(); ++i) {
			Vector2i pixel_pos = r[i];

			for (int idx = 0; idx < _selection_cells.size(); ++idx) {
				Vector2i pixel = _selection_cells[idx];
				Color color = _selection_colors[idx];
				pixel -= _cut_pos + _cut_size / 2;
				pixel += pixel_pos;
				paint_canvas->set_pixel_v(pixel, color);
			}
		}

		r.release();
	} else {
		if (_last_preview_draw_cell_pos == cell_mouse_position) {
			return;
		}

		paint_canvas->clear_preview_layer();

		for (int idx = 0; idx < _selection_cells.size(); ++idx) {
			Vector2i pixel = _selection_cells[idx];
			Color color = _selection_colors[idx];
			pixel -= _cut_pos + _cut_size / 2;
			pixel += cell_mouse_position;
			paint_canvas->set_preview_pixel_v(pixel, color);
		}

		_last_preview_draw_cell_pos = cell_mouse_position;
	}
}
void PaintWindow::brush_process() {
	if (Input::get_singleton()->is_mouse_button_pressed(BUTTON_LEFT)) {
		if (_current_action.is_null()) {
			_current_action = get_action();
		}

		if (brush_mode == Tools::COLORPICKER) {
			_current_action.unref();
		}

		switch (brush_mode) {
			case Tools::PAINT: {
				Array arr;

				arr.append(cell_mouse_position);
				arr.append(last_cell_mouse_position);
				arr.append(_selected_color);

				do_action(arr);
			} break;
			case Tools::BRUSH: {
				Array arr;

				arr.append(cell_mouse_position);
				arr.append(last_cell_mouse_position);
				arr.append(_selected_color);
				arr.append(selected_brush_prefab);
				arr.append(brush_size_slider->get_value());

				do_action(arr);
			} break;
			case Tools::LINE: {
				Array arr;

				arr.append(cell_mouse_position);
				arr.append(last_cell_mouse_position);
				arr.append(_selected_color);

				do_action(arr);
			} break;
			case Tools::RECT: {
				Array arr;

				arr.append(cell_mouse_position);
				arr.append(last_cell_mouse_position);
				arr.append(_selected_color);

				do_action(arr);
			} break;
			case Tools::DARKEN: {
				Array arr;

				arr.append(cell_mouse_position);
				arr.append(last_cell_mouse_position);
				arr.append(_selected_color);

				do_action(arr);
			} break;
			case Tools::BRIGHTEN: {
				Array arr;

				arr.append(cell_mouse_position);
				arr.append(last_cell_mouse_position);
				arr.append(_selected_color);

				do_action(arr);
			} break;
			case Tools::COLORPICKER: {
			} break;
			case Tools::CUT: {
				Array arr;

				arr.append(cell_mouse_position);
				arr.append(last_cell_mouse_position);
				arr.append(_selected_color);

				do_action(arr);
			} break;
			case Tools::PASTECUT: {
				Array arr;

				arr.append(cell_mouse_position);
				arr.append(last_cell_mouse_position);
				arr.append(_selection_cells);
				arr.append(_selection_colors);
				arr.append(_cut_pos);
				arr.append(_cut_size);

				do_action(arr);
			} break;
			case Tools::RAINBOW: {
				Array arr;

				arr.append(cell_mouse_position);
				arr.append(last_cell_mouse_position);

				do_action(arr);
			} break;
			default: {
			} break;
		}

		paint_canvas->update();

	} else if (Input::get_singleton()->is_mouse_button_pressed(BUTTON_RIGHT)) {
		paint_canvas->update();

		if (_current_action.is_null()) {
			_current_action = get_action();
		}

		switch (brush_mode) {
			case Tools::PAINT: {
				Array arr;

				arr.append(cell_mouse_position);
				arr.append(last_cell_mouse_position);
				arr.append(Color(1, 1, 1, 0));

				do_action(arr);
			} break;
			case Tools::BRUSH: {
				Array arr;

				arr.append(cell_mouse_position);
				arr.append(last_cell_mouse_position);
				arr.append(Color(1, 1, 1, 0));
				arr.append(selected_brush_prefab);
				arr.append(brush_size_slider->get_value());

				do_action(arr);
			} break;
			default: {
			} break;
		}
	} else {
		if (_current_action.is_valid() && _current_action->can_commit()) {
			commit_action();
			paint_canvas->update();
		}
	}
}
void PaintWindow::update_text_info() {
	String text;

	text = vformat("Mouse Position %s\tCanvas Mouse Position %s \tCanvas Position %s\t\nCell Position %s \tCell Color %s\t",
			mouse_position, canvas_mouse_position, canvas_position, cell_mouse_position, cell_color);

	text_info->set_text(text);
}

void PaintWindow::_on_Save_pressed() {
	paint_save_file_dialog->popup_centered();
}

void PaintWindow::do_action(const Array &data) {
	if (_current_action.is_null()) {
		//print("clear redo");
		_redo_history.clear();
	}

	_current_action->do_action(paint_canvas, data);
}
void PaintWindow::commit_action() {
	if (!_current_action.is_valid()) {
		return;
	}

	//print("commit action")
	_current_action->commit_action(paint_canvas);
	Ref<PaintAction> action = get_action();
	action->action_data = _current_action->action_data.duplicate(true);
	_actions_history.push_back(action);
	_redo_history.clear();

	switch (brush_mode) {
		case Tools::CUT: {
			Ref<CutAction> ca = _current_action;

			_cut_pos = ca->mouse_start_pos;
			_cut_size = ca->mouse_end_pos - ca->mouse_start_pos;

			PoolVector2iArray dcell = ca->redo_cells;
			PoolColorArray dcol = ca->redo_colors;

			//_selection_cells = dcell.duplicate();
			//_selection_colors = dcol.duplicate();

			_selection_cells = dcell;
			_selection_colors = dcol;

			set_brush(Tools::PASTECUT);
		} break;
		default: {
			_current_action.unref();
		} break;
	}
}
void PaintWindow::redo_action() {
	if (_redo_history.empty()) {
		//print("nothing to redo");
		return;
	}

	Ref<PaintAction> action = _redo_history[0];
	_redo_history.remove(0);

	if (!action.is_valid()) {
		return;
	}

	_actions_history.push_back(action);
	action->redo_action(paint_canvas);
	paint_canvas->update();

	//print("redo action");
}
void PaintWindow::undo_action() {
	if (_actions_history.empty()) {
		return;
	}

	Ref<PaintAction> action = _redo_history[0];
	_redo_history.remove(0);

	if (!action.is_valid()) {
		return;
	}

	_redo_history.push_back(action);
	action->undo_action(paint_canvas);
	update();
	paint_canvas->update();

	//print("undo action")
}

Ref<PaintAction> PaintWindow::get_action() {
	switch (brush_mode) {
		case Tools::PAINT:
			return Ref<PaintAction>(memnew(PencilAction));
		case Tools::BRUSH:
			return Ref<PaintAction>(memnew(BrushAction));
		case Tools::LINE:
			return Ref<PaintAction>(memnew(LineAction));
		case Tools::RAINBOW:
			return Ref<PaintAction>(memnew(RainbowAction));
		case Tools::BUCKET:
			return Ref<PaintAction>(memnew(BucketAction));
		case Tools::RECT:
			return Ref<PaintAction>(memnew(RectAction));
		case Tools::DARKEN:
			return Ref<PaintAction>(memnew(DarkenAction));
		case Tools::BRIGHTEN:
			return Ref<PaintAction>(memnew(BrightenAction));
		case Tools::CUT:
			return Ref<PaintAction>(memnew(CutAction));
		case Tools::PASTECUT:
			return Ref<PaintAction>(memnew(PasteCutAction));
		default:
			//print("no tool!")
			break;
	}

	return Ref<PaintAction>();
}

void PaintWindow::set_brush(const PaintWindow::Tools new_mode) {
	if (brush_mode == new_mode) {
		return;
	}

	_previous_tool = brush_mode;
	brush_mode = new_mode;

	_current_action = get_action();

	switch (_previous_tool) {
		case Tools::CUT:
			paint_canvas->clear_preview_layer();
			break;
		case Tools::PASTECUT:
			_selection_cells.resize(0);
			_selection_colors.resize(0);
			break;
		case Tools::BUCKET:
			_current_action.unref();
			break;
		default:
			break;
	}

	//print("Selected: ", Tools::keys()[brush_mode]);
}

void PaintWindow::change_color(const Color &new_color) {
	if (new_color.a == 0) {
		return;
	}

	_selected_color = new_color;

	color_picker_button->set_pick_color(_selected_color);
}

void PaintWindow::_on_ColorPicker_color_changed(const Color &color) {
	_selected_color = color;
}
void PaintWindow::_on_PaintTool_pressed() {
	set_brush(Tools::PAINT);
}
void PaintWindow::_on_BucketTool_pressed() {
	set_brush(Tools::BUCKET);
}
void PaintWindow::_on_RainbowTool_pressed() {
	set_brush(Tools::RAINBOW);
}
void PaintWindow::_on_BrushTool_pressed() {
	set_brush(Tools::BRUSH);
}
void PaintWindow::_on_LineTool_pressed() {
	set_brush(Tools::LINE);
}
void PaintWindow::_on_RectTool_pressed() {
	set_brush(Tools::RECT);
}
void PaintWindow::_on_DarkenTool_pressed() {
	set_brush(Tools::DARKEN);
}
void PaintWindow::_on_BrightenTool_pressed() {
	set_brush(Tools::BRIGHTEN);
}
void PaintWindow::_on_ColorPickerTool_pressed() {
	set_brush(Tools::COLORPICKER);
}
void PaintWindow::_on_CutTool_pressed() {
	set_brush(Tools::CUT);
}
void PaintWindow::_on_Editor_visibility_changed() {
	set_process(is_visible_in_tree());
}

void PaintWindow::_on_ChangeGridSizeDialog_confirmed() {
	paint_canvas->set_grid_size(paint_change_grid_size_dialog->get_grid_value());
	paint_canvas->set_big_grid_size(paint_change_grid_size_dialog->get_big_grid_value());
}

void PaintWindow::_on_ChangeCanvasSizeDialog_confirmed() {
	paint_canvas->resize(paint_canvas_dialog->get_size_x(), paint_canvas_dialog->get_size_y());
}

void PaintWindow::highlight_layer(const String &layer_name) {
	for (int i = 0; i < layers_box_container->get_child_count(); ++i) {
		PaintLayerButton *button = Object::cast_to<PaintLayerButton>(layers_box_container->get_child(i));

		if (!button) {
			continue;
		}

		String name = button->get_name();

		if (paint_canvas->find_layer_by_name(name)->locked) {
			Ref<StyleBox> sb = button->get("custom_styles/panel");

			if (sb.is_valid()) {
				sb->set("bg_color", locked_layer_highlight);
			}
		} else if (name == layer_name) {
			Ref<StyleBox> sb = button->get("custom_styles/panel");

			if (sb.is_valid()) {
				sb->set("bg_color", current_layer_highlight);
			}
		} else {
			Ref<StyleBox> sb = button->get("custom_styles/panel");

			if (sb.is_valid()) {
				sb->set("bg_color", other_layer_highlight);
			}
		}
	}
}

void PaintWindow::toggle_layer_visibility(Node *button, const String &layer_name) {
	//print("toggling: ", layer_name)
	paint_canvas->toggle_layer_visibility(layer_name);
}
void PaintWindow::select_layer(const String &layer_name) {
	//print("select layer: ", layer_name)
	paint_canvas->select_layer(layer_name);
	highlight_layer(layer_name);
}
void PaintWindow::lock_layer(Node *button, const String &layer_name) {
	paint_canvas->toggle_lock_layer(layer_name);
	highlight_layer(paint_canvas->get_active_layer()->name);
}

Ref<PaintCanvasLayer> PaintWindow::add_new_layer() {
	PaintLayerButton *new_layer_button = memnew(PaintLayerButton);

	_total_added_layers += 1;
	String name = "Layer" + String::num(_total_added_layers);
	new_layer_button->set_name(name);
	new_layer_button->layer_button->set_text(name);

	if (layers_box_container->get_child_count() > 0) {
		layers_box_container->add_child_below_node(layers_box_container->get_child(layers_box_container->get_child_count() - 1), new_layer_button, true);
	} else {
		layers_box_container->add_child(new_layer_button, true);
	}

	_layer_button_ref[new_layer_button->get_name()] = new_layer_button;

	Vector<Variant> blb;
	blb.push_back(new_layer_button->get_name());
	new_layer_button->layer_button->connect("pressed", this, "select_layer", blb);

	Vector<Variant> bvis;
	bvis.push_back(new_layer_button->visible_button);
	bvis.push_back(new_layer_button->get_name());
	new_layer_button->visible_button->connect("pressed", this, "toggle_layer_visibility", bvis);

	Vector<Variant> bud;
	bud.push_back(new_layer_button);
	new_layer_button->up_button->connect("pressed", this, "move_down", bud);
	new_layer_button->down_button->connect("pressed", this, "move_up", bud);

	Vector<Variant> blo;
	blo.push_back(new_layer_button);
	blo.push_back(new_layer_button->get_name());
	new_layer_button->lock_button->connect("pressed", this, "lock_layer", blo);

	Ref<PaintCanvasLayer> layer = paint_canvas->add_new_layer(new_layer_button->get_name());

	highlight_layer(paint_canvas->get_active_layer()->name);

	//print("added layer: ", layer.name)

	return layer;
}
void PaintWindow::remove_active_layer() {
	if (layers_box_container->get_child_count() <= 1) {
		return;
	}

	String layer_name = paint_canvas->active_layer->name;
	paint_canvas->remove_layer(layer_name);
	layers_box_container->remove_child(_layer_button_ref[layer_name]);
	_layer_button_ref[layer_name]->queue_delete();
	_layer_button_ref.erase(layer_name);

	highlight_layer(paint_canvas->get_active_layer()->name);
}
void PaintWindow::duplicate_active_layer() {
	PaintLayerButton *new_layer_button = memnew(PaintLayerButton);
	//new_layer_button.set("custom_styles/panel", layers_box_container.get_child(0).get("custom_styles/panel").duplicate());
	layers_box_container->add_child_below_node(layers_box_container->get_child(layers_box_container->get_child_count() - 1), new_layer_button, true);

	_total_added_layers += 1; // for keeping track...
	new_layer_button->layer_button->set_text("Layer" + String::num(_total_added_layers));

	Ref<PaintCanvasLayer> new_layer = paint_canvas->duplicate_layer(paint_canvas->active_layer->name, new_layer_button->get_name());
	new_layer->update_texture();
	_layer_button_ref[new_layer->name] = new_layer_button;

	Vector<Variant> blb;
	blb.push_back(new_layer_button->get_name());
	new_layer_button->layer_button->connect("pressed", this, "select_layer", blb);

	Vector<Variant> bvis;
	bvis.push_back(new_layer_button->visible_button);
	bvis.push_back(new_layer_button->get_name());
	new_layer_button->visible_button->connect("pressed", this, "toggle_layer_visibility", bvis);

	Vector<Variant> bud;
	bud.push_back(new_layer_button);
	new_layer_button->up_button->connect("pressed", this, "move_down", bud);
	new_layer_button->down_button->connect("pressed", this, "move_up", bud);

	Vector<Variant> blo;
	blo.push_back(new_layer_button);
	blo.push_back(new_layer_button->get_name());
	new_layer_button->lock_button->connect("pressed", this, "lock_layer", blo);

	// update highlight
	highlight_layer(paint_canvas->get_active_layer()->name);
	//print("added layer: ", new_layer.name, " (total:", layers_box_container.get_child_count(), ")")
}

void PaintWindow::move_up(Node *layer_btn) {
	int new_idx = MIN(layer_btn->get_index() + 1, layers_box_container->get_child_count());
	//print("move_up: ", layer_btn.name, " from ", layer_btn.get_index(), " to ", new_idx)
	layers_box_container->move_child(layer_btn, new_idx);
	paint_canvas->move_layer_back(layer_btn->get_name());
}
void PaintWindow::move_down(Node *layer_btn) {
	int new_idx = MAX(layer_btn->get_index() - 1, 0);
	//print("move_down: ", layer_btn.name, " from ", layer_btn.get_index(), " to ", new_idx)
	layers_box_container->move_child(layer_btn, new_idx);
	paint_canvas->move_layer_forward(layer_btn->get_name());
}

void PaintWindow::_connect_layer_buttons() {
	for (int i = 0; i < layers_box_container->get_child_count(); ++i) {
		PaintLayerButton *layer_btn = Object::cast_to<PaintLayerButton>(layers_box_container->get_child(i));

		if (!layer_btn) {
			continue;
		}

		if (layer_btn->layer_button->is_connected("pressed", this, "select_layer")) {
			continue;
		}

		Vector<Variant> blb;
		blb.push_back(layer_btn->get_name());
		layer_btn->layer_button->connect("pressed", this, "select_layer", blb);

		Vector<Variant> bvis;
		bvis.push_back(layer_btn->visible_button);
		bvis.push_back(layer_btn->get_name());
		layer_btn->visible_button->connect("pressed", this, "toggle_layer_visibility", bvis);

		Vector<Variant> bud;
		bud.push_back(layer_btn);
		layer_btn->up_button->connect("pressed", this, "move_down", bud);
		layer_btn->down_button->connect("pressed", this, "move_up", bud);

		Vector<Variant> blo;
		blo.push_back(layer_btn);
		blo.push_back(layer_btn->get_name());
		layer_btn->lock_button->connect("pressed", this, "lock_layer", blo);
	}
}

void PaintWindow::_on_Button_pressed() {
	add_new_layer();
}
void PaintWindow::_on_PaintCanvasContainer_mouse_entered() {
	if (mouse_on_top) {
		return;
	}

	mouse_on_top = true;
	paint_canvas->tool_layer->clear();
	paint_canvas->update();
	paint_canvas->tool_layer->update_texture();
}
void PaintWindow::_on_PaintCanvasContainer_mouse_exited() {
	if (!mouse_on_top) {
		return;
	}

	mouse_on_top = false;
	paint_canvas->tool_layer->clear();
	paint_canvas->update();
	paint_canvas->tool_layer->update_texture();
}
void PaintWindow::_on_ColorPicker_popup_closed() {
	color_grid->add_color_prefab(color_picker_button->get_pick_color());
}

bool PaintWindow::is_position_in_canvas(const Vector2 &pos) {
	Rect2 r = Rect2(paint_canvas_container->get_global_position(), paint_canvas_container->get_global_position() + paint_canvas_container->get_size());

	if (r.has_point(pos)) {
		return true;
	}

	return false;
}

bool PaintWindow::is_mouse_in_canvas() {
	if (is_position_in_canvas(get_global_mouse_position())) {
		return true; //mouse_on_top # check if mouse is inside canvas
	} else {
		return false;
	}
}

bool PaintWindow::is_any_menu_open() {
	return paint_canvas_dialog->is_visible() || paint_change_grid_size_dialog->is_visible() ||
			paint_load_file_dialog->is_visible() || paint_save_file_dialog->is_visible() ||
			paint_settings_dialog->is_visible() || navbar->is_any_menu_open();
}

void PaintWindow::_on_LockAlpha_pressed() {
	//bool checked = lock_alpha_button->is_pressed();
	paint_canvas->active_layer->toggle_alpha_locked();

	/*
		for i in range(find_node("Layer").get_popup().get_item_count()):
			if find_node("Layer").get_popup().get_item_text(i) == "Toggle Alpha Locked":
				find_node("Layer").get_popup().set_item_checked(i, not find_node("Layer").get_popup().is_item_checked(i));
	*/
}
void PaintWindow::_on_BrushRect_pressed() {
	if (brush_mode != Tools::BRUSH) {
		set_brush(Tools::BRUSH);
	}

	selected_brush_prefab = BrushPrefabs::RECT;
}
void PaintWindow::_on_BrushCircle_pressed() {
	if (brush_mode != Tools::BRUSH) {
		set_brush(Tools::BRUSH);
	}

	selected_brush_prefab = BrushPrefabs::CIRCLE;
}
void PaintWindow::_on_BrushVLine_pressed() {
	if (brush_mode != Tools::BRUSH) {
		set_brush(Tools::BRUSH);
	}

	selected_brush_prefab = BrushPrefabs::V_LINE;
}
void PaintWindow::_on_BrushHLine_pressed() {
	if (brush_mode != Tools::BRUSH) {
		set_brush(Tools::BRUSH);
	}

	selected_brush_prefab = BrushPrefabs::H_LINE;
}
void PaintWindow::_on_BrushSize_value_changed(float value) {
	brush_size_label->set_text(rtos(value));
}
void PaintWindow::_on_XSymmetry_pressed() {
	paint_canvas->symmetry_x = !paint_canvas->symmetry_x;
}
void PaintWindow::_on_YSymmetry_pressed() {
	paint_canvas->symmetry_y = !paint_canvas->symmetry_y;
}

void PaintWindow::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
			set_process_input(is_visible_in_tree());
		} break;
		case NOTIFICATION_VISIBILITY_CHANGED: {
			if (is_visible_in_tree()) {
				set_process_input(true);
			} else {
				set_process_input(false);
			}
		} break;
		case NOTIFICATION_POSTINITIALIZE: {
			connect("visibility_changed", this, "_on_Editor_visibility_changed");
			paint_canvas->canvas_background->set_pixel_size(8 * pow(0.5, big_grid_pixels) / paint_canvas->get_pixel_size());
		} break;
		case NOTIFICATION_PROCESS: {
			_process(get_process_delta_time());
		} break;
		default: {
		} break;
	}
}

PaintWindow::PaintWindow() {
	_selected_color = Color(1, 1, 1, 1);

	allow_drawing = true;

	mouse_in_region = false;
	mouse_on_top = false;

	_picked_color = false;

	_total_added_layers = 0;

	_middle_mouse_pressed = false;
	_previous_tool = PaintWindow::COLORPICKER;
	brush_mode = PaintWindow::COLORPICKER;

	selected_brush_prefab = BrushPrefabs::CIRCLE;

	current_layer_highlight = Color(0.354706, 0.497302, 0.769531);
	other_layer_highlight = Color(0.180392, 0.176471, 0.176471);
	locked_layer_highlight = Color(0.098039, 0.094118, 0.094118);

	big_grid_pixels = 4;

	set_clip_contents(true);
	set_h_size_flags(SIZE_EXPAND_FILL);
	set_v_size_flags(SIZE_EXPAND_FILL);

	//Main content panel
	PanelContainer *main_content_panel = memnew(PanelContainer);
	add_child(main_content_panel);
	main_content_panel->set_anchors_and_margins_preset(Control::PRESET_WIDE);

	VBoxContainer *main_content_container = memnew(VBoxContainer);
	main_content_panel->add_child(main_content_container);

	//Main Content Top (Navbar)
	navbar = memnew(PaintNavbar);
	navbar->paint_window = this;
	navbar->set_h_size_flags(SIZE_EXPAND_FILL);
	main_content_container->add_child(navbar);

	//Main Content Mid (App)
	HBoxContainer *app_mid_container = memnew(HBoxContainer);
	app_mid_container->set_h_size_flags(SIZE_EXPAND_FILL);
	app_mid_container->set_v_size_flags(SIZE_EXPAND_FILL);
	main_content_container->add_child(app_mid_container);

	//Main Content Mid (App) -- Left Panel
	PanelContainer *left_content_panel = memnew(PanelContainer);
	left_content_panel->set_h_size_flags(SIZE_EXPAND_FILL);
	left_content_panel->set_v_size_flags(SIZE_EXPAND_FILL);
	app_mid_container->add_child(left_content_panel);

	Ref<StyleBoxFlat> style_box;
	style_box.instance();
	style_box->set("bg_color", Color(0.2, 0.2, 0.2));
	left_content_panel->set("custom_styles/panel", style_box);

	//Main Content Mid (App) -- Left Panel -- Margin container
	MarginContainer *left_main_margin_container = memnew(MarginContainer);
	left_content_panel->add_child(left_main_margin_container);

	//Main Content Mid (App) -- Left Panel -- Margin container -- VBoxContainer
	VBoxContainer *left_main_vbox_container = memnew(VBoxContainer);
	left_main_vbox_container->set("custom_constants/separation", 12);
	left_main_margin_container->add_child(left_main_vbox_container);

	//Main Content Mid (App) -- Left Panel -- Margin container -- VBoxContainer -- Colors
	ScrollContainer *left_color_scroll_container = memnew(ScrollContainer);
	left_color_scroll_container->set_custom_minimum_size(Size2(0, 145));
	left_main_vbox_container->add_child(left_color_scroll_container);

	color_grid = memnew(PaintColorGrid);
	color_grid->set_columns(4);
	color_grid->set_custom_minimum_size(Size2(0, 145));
	color_grid->set_h_size_flags(SIZE_EXPAND_FILL);
	color_grid->set_v_size_flags(SIZE_EXPAND_FILL);
	color_grid->connect("color_change_request", this, "change_color");
	left_color_scroll_container->add_child(color_grid);

	//Main Content Mid (App) -- Left Panel -- Margin container -- VBoxContainer -- Lock Alpha Button
	lock_alpha_button = memnew(CheckButton);
	lock_alpha_button->set_text("Lock Alpha");
	lock_alpha_button->set_text_align(Button::ALIGN_RIGHT);
	lock_alpha_button->connect("pressed", this, "_on_LockAlpha_pressed");
	left_main_vbox_container->add_child(lock_alpha_button);

	//Main Content Mid (App) -- Left Panel -- Margin container -- VBoxContainer -- Brush Selection
	GridContainer *brush_container = memnew(GridContainer);
	brush_container->set_columns(4);
	left_main_vbox_container->add_child(brush_container);

	TextureButton *brush_rect_button = memnew(TextureButton);
	brush_rect_button->set_normal_texture(make_icon(brush_rect_png));
	brush_rect_button->set_hover_texture(make_icon(brush_rect_hovered_png));
	brush_rect_button->set_custom_minimum_size(Size2(25, 25));
	brush_rect_button->connect("pressed", this, "_on_BrushRect_pressed");
	brush_container->add_child(brush_rect_button);

	TextureButton *brush_circle_button = memnew(TextureButton);
	brush_circle_button->set_normal_texture(make_icon(brush_circle_png));
	brush_circle_button->set_hover_texture(make_icon(brush_circle_hovered_png));
	brush_circle_button->set_custom_minimum_size(Size2(25, 25));
	brush_circle_button->connect("pressed", this, "_on_BrushCircle_pressed");
	brush_container->add_child(brush_circle_button);

	TextureButton *brush_v_line_button = memnew(TextureButton);
	brush_v_line_button->set_normal_texture(make_icon(brush_v_line_png));
	brush_v_line_button->set_hover_texture(make_icon(brush_v_line_hovered_png));
	brush_v_line_button->set_custom_minimum_size(Size2(25, 25));
	brush_v_line_button->connect("pressed", this, "_on_BrushVLine_pressed");
	brush_container->add_child(brush_v_line_button);

	TextureButton *brush_h_line_button = memnew(TextureButton);
	brush_h_line_button->set_normal_texture(make_icon(brush_h_line_png));
	brush_h_line_button->set_hover_texture(make_icon(brush_h_line_hovered_png));
	brush_h_line_button->set_custom_minimum_size(Size2(25, 25));
	brush_h_line_button->connect("pressed", this, "_on_BrushHLine_pressed");
	brush_container->add_child(brush_h_line_button);

	//Main Content Mid (App) -- Left Panel -- Margin container -- VBoxContainer -- Brush Size
	Label *brush_size_name_label = memnew(Label);
	brush_size_name_label->set_text("Brush Size");
	left_main_vbox_container->add_child(brush_size_name_label);

	HBoxContainer *brush_size_container = memnew(HBoxContainer);
	left_main_vbox_container->add_child(brush_size_container);

	brush_size_label = memnew(Label);
	brush_size_label->set_text("1");
	brush_size_container->add_child(brush_size_label);

	brush_size_slider = memnew(HSlider);
	brush_size_slider->set_min(1);
	brush_size_slider->set_value(1);
	brush_size_slider->set_h_size_flags(SIZE_EXPAND_FILL);
	brush_size_slider->set_v_size_flags(SIZE_EXPAND);
	brush_size_container->add_child(brush_size_slider);

	brush_size_label->set_text(String::num(static_cast<int>(brush_size_slider->get_value())));
	brush_size_slider->connect("value_changed", this, "_on_BrushSize_value_changed");

	//Main Content Mid (App) -- Left Panel -- Margin container -- VBoxContainer -- Symmetries
	x_symmetry_button = memnew(CheckButton);
	x_symmetry_button->set_text("X Symmetry");
	x_symmetry_button->connect("pressed", this, "_on_XSymmetry_pressed");
	left_main_vbox_container->add_child(x_symmetry_button);

	y_symmetry_button = memnew(CheckButton);
	y_symmetry_button->set_text("Y Symmetry");
	y_symmetry_button->connect("pressed", this, "_on_YSymmetry_pressed");
	left_main_vbox_container->add_child(y_symmetry_button);

	//Main Content Mid (App) -- Mid (PaintCanvas)
	paint_canvas_container = memnew(Control);
	paint_canvas_container->set_clip_contents(true);
	paint_canvas_container->set_focus_mode(Control::FOCUS_CLICK);
	paint_canvas_container->set_mouse_filter(Control::MOUSE_FILTER_PASS);
	paint_canvas_container->set_h_size_flags(SIZE_EXPAND_FILL);
	paint_canvas_container->set_v_size_flags(SIZE_EXPAND_FILL);
	paint_canvas_container->set_stretch_ratio(6);
	paint_canvas_container->connect("mouse_entered", this, "_on_PaintCanvasContainer_mouse_entered");
	paint_canvas_container->connect("mouse_exited", this, "_on_PaintCanvasContainer_mouse_exited");
	app_mid_container->add_child(paint_canvas_container);

	paint_canvas = memnew(PaintCanvas);
	paint_canvas->set_pixel_size(4);
	paint_canvas->set_canvas_width(64);
	paint_canvas->set_canvas_height(64);
	paint_canvas->set_grid_size(1);
	paint_canvas->set_big_grid_size(8);
	paint_canvas->set_mouse_filter(Control::MOUSE_FILTER_PASS);
	paint_canvas->set_h_size_flags(SIZE_EXPAND_FILL);
	paint_canvas->set_v_size_flags(SIZE_EXPAND_FILL);
	paint_canvas->set_size(Size2(256, 256));
	paint_canvas->set_anchors_and_margins_preset(Control::PRESET_CENTER);
	paint_canvas_container->add_child(paint_canvas);
	navbar->canvas = paint_canvas;

	//Main Content Mid (App) -- Right Panel
	PanelContainer *mid_right_panel_container = memnew(PanelContainer);
	mid_right_panel_container->set_h_size_flags(SIZE_EXPAND_FILL);
	mid_right_panel_container->set_v_size_flags(SIZE_EXPAND_FILL);
	app_mid_container->add_child(mid_right_panel_container);

	style_box.instance();
	style_box->set("bg_color", Color(0.2, 0.2, 0.2));
	mid_right_panel_container->set("custom_styles/panel", style_box);

	ScrollContainer *mid_right_main_scroll_container = memnew(ScrollContainer);
	mid_right_main_scroll_container->set_h_size_flags(SIZE_EXPAND_FILL);
	mid_right_main_scroll_container->set_v_size_flags(SIZE_EXPAND_FILL);
	mid_right_main_scroll_container->set_h_scroll(false);
	mid_right_panel_container->add_child(mid_right_main_scroll_container);

	VBoxContainer *main_tool_contianer = memnew(VBoxContainer);
	main_tool_contianer->set_h_size_flags(SIZE_EXPAND_FILL);
	main_tool_contianer->set_v_size_flags(SIZE_EXPAND_FILL);
	mid_right_main_scroll_container->add_child(main_tool_contianer);

	//Main Content Mid (App) -- Right Panel -- Tools
	VBoxContainer *tools_contianer = memnew(VBoxContainer);
	tools_contianer->set_h_size_flags(SIZE_EXPAND_FILL);
	main_tool_contianer->add_child(tools_contianer);

	paint_tool_button = memnew(Button);
	paint_tool_button->set_text("Pencil (Q)");
	paint_tool_button->set_h_size_flags(SIZE_EXPAND_FILL);
	paint_tool_button->set_v_size_flags(SIZE_EXPAND_FILL);
	paint_tool_button->connect("pressed", this, "_on_PaintTool_pressed");
	tools_contianer->add_child(paint_tool_button);

	brush_tool_button = memnew(Button);
	brush_tool_button->set_text("Brush");
	brush_tool_button->set_h_size_flags(SIZE_EXPAND_FILL);
	brush_tool_button->set_v_size_flags(SIZE_EXPAND_FILL);
	brush_tool_button->connect("pressed", this, "_on_BrushTool_pressed");
	tools_contianer->add_child(brush_tool_button);
	brush_tool_button->hide();

	multi_tool_button = memnew(Button);
	multi_tool_button->set_text("Polygon");
	multi_tool_button->set_h_size_flags(SIZE_EXPAND_FILL);
	multi_tool_button->set_v_size_flags(SIZE_EXPAND_FILL);
	//multi_tool_button->connect("pressed", this, "_on_PaintTool_pressed");
	tools_contianer->add_child(multi_tool_button);
	multi_tool_button->hide();

	bucket_tool_button = memnew(Button);
	bucket_tool_button->set_text("Bucket Fill (F)");
	bucket_tool_button->set_h_size_flags(SIZE_EXPAND_FILL);
	bucket_tool_button->set_v_size_flags(SIZE_EXPAND_FILL);
	bucket_tool_button->connect("pressed", this, "_on_BucketTool_pressed");
	tools_contianer->add_child(bucket_tool_button);

	rainbow_tool_button = memnew(Button);
	rainbow_tool_button->set_text("Rainbow (R)");
	rainbow_tool_button->set_h_size_flags(SIZE_EXPAND_FILL);
	rainbow_tool_button->set_v_size_flags(SIZE_EXPAND_FILL);
	rainbow_tool_button->connect("pressed", this, "_on_RainbowTool_pressed");
	tools_contianer->add_child(rainbow_tool_button);

	line_tool_button = memnew(Button);
	line_tool_button->set_text("Line (L)");
	line_tool_button->set_h_size_flags(SIZE_EXPAND_FILL);
	line_tool_button->set_v_size_flags(SIZE_EXPAND_FILL);
	line_tool_button->connect("pressed", this, "_on_LineTool_pressed");
	tools_contianer->add_child(line_tool_button);

	rect_tool_button = memnew(Button);
	rect_tool_button->set_text("Rectangle");
	rect_tool_button->set_h_size_flags(SIZE_EXPAND_FILL);
	rect_tool_button->set_v_size_flags(SIZE_EXPAND_FILL);
	rect_tool_button->connect("pressed", this, "_on_RectTool_pressed");
	tools_contianer->add_child(rect_tool_button);

	darken_tool_button = memnew(Button);
	darken_tool_button->set_text("Darken (D)");
	darken_tool_button->set_h_size_flags(SIZE_EXPAND_FILL);
	darken_tool_button->set_v_size_flags(SIZE_EXPAND_FILL);
	darken_tool_button->connect("pressed", this, "_on_DarkenTool_pressed");
	tools_contianer->add_child(darken_tool_button);

	brighten_tool_button = memnew(Button);
	brighten_tool_button->set_text("Brighten (B)");
	brighten_tool_button->set_h_size_flags(SIZE_EXPAND_FILL);
	brighten_tool_button->set_v_size_flags(SIZE_EXPAND_FILL);
	brighten_tool_button->connect("pressed", this, "_on_BrightenTool_pressed");
	tools_contianer->add_child(brighten_tool_button);

	color_picker_tool_button = memnew(Button);
	color_picker_tool_button->set_text("Color Picker (P)");
	color_picker_tool_button->set_h_size_flags(SIZE_EXPAND_FILL);
	color_picker_tool_button->set_v_size_flags(SIZE_EXPAND_FILL);
	color_picker_tool_button->connect("pressed", this, "_on_ColorPickerTool_pressed");
	tools_contianer->add_child(color_picker_tool_button);

	cut_tool_button = memnew(Button);
	cut_tool_button->set_text("Cut Section (C)");
	cut_tool_button->set_h_size_flags(SIZE_EXPAND_FILL);
	cut_tool_button->set_v_size_flags(SIZE_EXPAND_FILL);
	cut_tool_button->connect("pressed", this, "_on_CutTool_pressed");
	tools_contianer->add_child(cut_tool_button);

	color_picker_button = memnew(ColorPickerButton);
	color_picker_button->set_text("Cut Section (C)");
	color_picker_button->set_h_size_flags(SIZE_EXPAND_FILL);
	color_picker_button->set_v_size_flags(SIZE_EXPAND_FILL);
	color_picker_button->connect("color_changed", this, "_on_ColorPicker_color_changed");
	color_picker_button->connect("popup_closed", this, "_on_ColorPicker_popup_closed");
	tools_contianer->add_child(color_picker_button);

	//Main Content Mid (App) -- Right Panel -- Layers
	ScrollContainer *layers_scroll_contianer = memnew(ScrollContainer);
	layers_scroll_contianer->set_h_size_flags(SIZE_EXPAND_FILL);
	layers_scroll_contianer->set_v_size_flags(SIZE_EXPAND_FILL);
	layers_scroll_contianer->set_h_scroll(false);
	main_tool_contianer->add_child(layers_scroll_contianer);

	VBoxContainer *main_layers_box_container = memnew(VBoxContainer);
	main_layers_box_container->set_h_size_flags(SIZE_EXPAND_FILL);
	main_layers_box_container->set_v_size_flags(SIZE_EXPAND_FILL);
	layers_scroll_contianer->add_child(main_layers_box_container);

	layers_box_container = memnew(VBoxContainer);
	layers_box_container->set_h_size_flags(SIZE_EXPAND_FILL);
	layers_box_container->set_v_size_flags(SIZE_EXPAND_FILL);
	main_layers_box_container->add_child(layers_box_container);

	add_layer_button = memnew(Button);
	add_layer_button->set_text("+");
	add_layer_button->set_custom_minimum_size(Size2(0, 25));
	add_layer_button->set_h_size_flags(SIZE_EXPAND_FILL);
	add_layer_button->connect("pressed", this, "_on_Button_pressed");
	main_layers_box_container->add_child(add_layer_button);

	//Main Content Bottom (Text display)
	PanelContainer *bottom_content_panel = memnew(PanelContainer);
	bottom_content_panel->set_h_size_flags(SIZE_EXPAND_FILL);
	bottom_content_panel->set_custom_minimum_size(Size2(0, 50));
	main_content_container->add_child(bottom_content_panel);

	text_info = memnew(RichTextLabel);
	text_info->set_scroll_active(false);
	bottom_content_panel->add_child(text_info);

	//PaintCanvasDialog
	paint_canvas_dialog = memnew(PaintCanvasDialog);
	paint_canvas_dialog->set_size_x(64);
	paint_canvas_dialog->set_size_y(64);
	paint_canvas_dialog->connect("confirmed", this, "_on_ChangeCanvasSizeDialog_confirmed");
	add_child(paint_canvas_dialog);

	//PaintChangeGridSizeDialog
	paint_change_grid_size_dialog = memnew(PaintChangeGridSizeDialog);
	paint_change_grid_size_dialog->set_grid_value(1);
	paint_change_grid_size_dialog->set_big_grid_value(8);
	paint_change_grid_size_dialog->connect("confirmed", this, "_on_ChangeGridSizeDialog_confirmed");
	add_child(paint_change_grid_size_dialog);

	//PaintLoadFileDialog
	paint_load_file_dialog = memnew(PaintLoadFileDialog);
	paint_load_file_dialog->canvas = paint_canvas;
	paint_load_file_dialog->window = this;
	paint_load_file_dialog->set_size(Size2(600, 400));
	add_child(paint_load_file_dialog);

	//PaintSaveFileDialog
	paint_save_file_dialog = memnew(PaintSaveFileDialog);
	paint_save_file_dialog->canvas = paint_canvas;
	paint_load_file_dialog->set_size(Size2(600, 400));
	add_child(paint_save_file_dialog);

	//PaintSettings
	paint_settings_dialog = memnew(PaintSettings);
	paint_settings_dialog->canvas_outline = paint_canvas->canvas_outline;
	add_child(paint_settings_dialog);

	set_brush(Tools::PAINT);

	_on_ChangeCanvasSizeDialog_confirmed();
	_on_ChangeGridSizeDialog_confirmed();

	//find_node("CanvasBackground").material.set_shader_param("pixel_size", 8 * pow(0.5, big_grid_pixels)/paint_canvas.pixel_size)

	add_new_layer();
	paint_canvas->update();
}

PaintWindow::~PaintWindow() {
}

void PaintWindow::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_input", "event"), &PaintWindow::_input);

	ClassDB::bind_method(D_METHOD("change_color", "color"), &PaintWindow::change_color);

	ClassDB::bind_method(D_METHOD("_on_Save_pressed"), &PaintWindow::_on_Save_pressed);

	ClassDB::bind_method(D_METHOD("_on_ColorPicker_color_changed", "color"), &PaintWindow::_on_ColorPicker_color_changed);

	ClassDB::bind_method(D_METHOD("_on_PaintTool_pressed"), &PaintWindow::_on_PaintTool_pressed);
	ClassDB::bind_method(D_METHOD("_on_BucketTool_pressed"), &PaintWindow::_on_BucketTool_pressed);
	ClassDB::bind_method(D_METHOD("_on_RainbowTool_pressed"), &PaintWindow::_on_RainbowTool_pressed);
	ClassDB::bind_method(D_METHOD("_on_BrushTool_pressed"), &PaintWindow::_on_BrushTool_pressed);
	ClassDB::bind_method(D_METHOD("_on_LineTool_pressed"), &PaintWindow::_on_LineTool_pressed);
	ClassDB::bind_method(D_METHOD("_on_RectTool_pressed"), &PaintWindow::_on_RectTool_pressed);
	ClassDB::bind_method(D_METHOD("_on_DarkenTool_pressed"), &PaintWindow::_on_DarkenTool_pressed);
	ClassDB::bind_method(D_METHOD("_on_BrightenTool_pressed"), &PaintWindow::_on_BrightenTool_pressed);
	ClassDB::bind_method(D_METHOD("_on_ColorPickerTool_pressed"), &PaintWindow::_on_ColorPickerTool_pressed);
	ClassDB::bind_method(D_METHOD("_on_CutTool_pressed"), &PaintWindow::_on_CutTool_pressed);
	ClassDB::bind_method(D_METHOD("_on_Editor_visibility_changed"), &PaintWindow::_on_Editor_visibility_changed);
	ClassDB::bind_method(D_METHOD("_on_ChangeGridSizeDialog_confirmed"), &PaintWindow::_on_ChangeGridSizeDialog_confirmed);
	ClassDB::bind_method(D_METHOD("_on_ChangeCanvasSizeDialog_confirmed"), &PaintWindow::_on_ChangeCanvasSizeDialog_confirmed);

	ClassDB::bind_method(D_METHOD("_on_Button_pressed"), &PaintWindow::_on_Button_pressed);
	ClassDB::bind_method(D_METHOD("_on_PaintCanvasContainer_mouse_entered"), &PaintWindow::_on_PaintCanvasContainer_mouse_entered);
	ClassDB::bind_method(D_METHOD("_on_PaintCanvasContainer_mouse_exited"), &PaintWindow::_on_PaintCanvasContainer_mouse_exited);
	ClassDB::bind_method(D_METHOD("_on_ColorPicker_popup_closed"), &PaintWindow::_on_ColorPicker_popup_closed);

	ClassDB::bind_method(D_METHOD("_on_LockAlpha_pressed"), &PaintWindow::_on_LockAlpha_pressed);
	ClassDB::bind_method(D_METHOD("_on_BrushRect_pressed"), &PaintWindow::_on_BrushRect_pressed);
	ClassDB::bind_method(D_METHOD("_on_BrushCircle_pressed"), &PaintWindow::_on_BrushCircle_pressed);
	ClassDB::bind_method(D_METHOD("_on_BrushVLine_pressed"), &PaintWindow::_on_BrushVLine_pressed);
	ClassDB::bind_method(D_METHOD("_on_BrushHLine_pressed"), &PaintWindow::_on_BrushHLine_pressed);
	ClassDB::bind_method(D_METHOD("_on_BrushSize_value_changed", "value"), &PaintWindow::_on_BrushSize_value_changed);
	ClassDB::bind_method(D_METHOD("_on_XSymmetry_pressed"), &PaintWindow::_on_XSymmetry_pressed);
	ClassDB::bind_method(D_METHOD("_on_YSymmetry_pressed"), &PaintWindow::_on_YSymmetry_pressed);

	ClassDB::bind_method(D_METHOD("lock_layer"), &PaintWindow::lock_layer);
	ClassDB::bind_method(D_METHOD("toggle_layer_visibility"), &PaintWindow::toggle_layer_visibility);
	ClassDB::bind_method(D_METHOD("select_layer", "name"), &PaintWindow::select_layer);
}
