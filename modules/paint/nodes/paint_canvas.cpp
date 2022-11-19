#include "paint_canvas.h"

#include "../actions/brighten_action.h"
#include "../actions/brush_action.h"
#include "../actions/bucket_action.h"
#include "../actions/cut_action.h"
#include "../actions/darken_action.h"
#include "../actions/line_action.h"
#include "../actions/multiline_action.h"
#include "../actions/paint_action.h"
#include "../actions/paste_cut_action.h"
#include "../actions/pencil_action.h"
#include "../actions/rainbow_action.h"
#include "../actions/rect_action.h"
#include "../bush_prefabs.h"
#include "../paint_utilities.h"
#include "core/io/image.h"
#include "core/os/keyboard.h"
#include "paint_project.h"
#include "scene/resources/texture.h"

#include "modules/modules_enabled.gen.h"

#ifdef MODULE_LZ4_ENABLED
#include "modules/lz4/lz4_compressor.h"
#endif

bool PaintCanvas::get_symmetry_x() const {
	return _symmetry_x;
}
void PaintCanvas::set_symmetry_x(const bool val) {
	_symmetry_x = val;
}

bool PaintCanvas::get_symmetry_y() const {
	return _symmetry_y;
}
void PaintCanvas::set_symmetry_y(const bool val) {
	_symmetry_y = val;
}

bool PaintCanvas::get_alpha_locked() const {
	return _alpha_locked;
}
void PaintCanvas::set_alpha_locked(const bool val) {
	_alpha_locked = val;
}

int PaintCanvas::get_brush_prefab() const {
	return _brush_prefab;
}
void PaintCanvas::set_brush_prefab(const int val) {
	if (_brush_prefab == val) {
		return;
	}

	_brush_prefab = val;

	emit_signal("brush_prefab_changed");
}

int PaintCanvas::get_brush_size() const {
	return _brush_size;
}
void PaintCanvas::set_brush_size(const int val) {
	if (_brush_size == val) {
		return;
	}

	_brush_size = val;

	emit_signal("brush_size_changed");
}

int PaintCanvas::get_current_tool() const {
	return _current_tool;
}
void PaintCanvas::set_current_tool(const int val) {
	if (_current_tool == val) {
		return;
	}

	_previous_tool = _current_tool;
	_current_tool = val;

	emit_signal("current_tool_changed");
}

int PaintCanvas::get_previous_tool() const {
	return _previous_tool;
}

bool PaintCanvas::validate_pixel_v(const Vector2i &pos) const {
	if (pos.x < 0 || pos.y < 0 || pos.x >= _image->get_width() || pos.y >= _image->get_height()) {
		return false;
	}

	return true;
}

bool PaintCanvas::is_inside_canvas(const int x, const int y) {
	if (x < 0 || y < 0) {
		return false;
	}
	if (x >= get_size().x || y >= get_size().y) {
		return false;
	}

	return true;
}

void PaintCanvas::set_pixel_arr(const PoolVector2iArray &pixels, const Color &color) {
	PoolVector2iArray::Read r = pixels.read();

	for (int i = 0; i < pixels.size(); ++i) {
		const Vector2i &pixel = r[i];

		set_pixel(pixel.x, pixel.y, color);
	}
}
void PaintCanvas::set_pixel_v(const Vector2i &pos, const Color &color) {
	set_pixel(pos.x, pos.y, color);
}
void PaintCanvas::set_pixel(const int x, const int y, const Color &color) {
	if (x < 0 || y < 0 || x >= _image->get_width() || y >= _image->get_height()) {
		return;
	}

	_image->lock();
	_image->set_pixel(x, y, color);
	_image->unlock();
}

Color PaintCanvas::get_pixel_v(const Vector2i &pos) {
	return get_pixel(pos.x, pos.y);
}
Color PaintCanvas::get_pixel(const int x, const int y) {
	if (x < 0 || y < 0 || x >= _image->get_width() || y >= _image->get_height()) {
		return Color();
	}

	_image->lock();
	Color pixel = _image->get_pixel(x, y);
	_image->unlock();

	return pixel;
}
void PaintCanvas::set_preview_pixel_v(const Vector2i &pos, const Color &color) {
	set_preview_pixel(pos.x, pos.y, color);
}

void PaintCanvas::set_preview_pixel(const int x, const int y, const Color &color) {
	if (x < 0 || y < 0 || x >= _preview_image->get_width() || y >= _preview_image->get_height()) {
		return;
	}

	_preview_image->lock();
	_preview_image->set_pixel(x, y, color);
	_preview_image->unlock();
}
Color PaintCanvas::get_preview_pixel_v(const Vector2i &pos) {
	return get_preview_pixel(pos.x, pos.y);
}

Color PaintCanvas::get_preview_pixel(const int x, const int y) {
	if (x < 0 || y < 0 || x >= _preview_image->get_width() || y >= _preview_image->get_height()) {
		return Color();
	}

	_preview_image->lock();
	Color pixel = _preview_image->get_pixel(x, y);
	_preview_image->unlock();

	return pixel;
}

void PaintCanvas::clear() {
	_image->fill(Color(1.00, 1.00, 1.00, 0.00));

	_image_texture->create_from_image(_image, 0);
}

void PaintCanvas::clear_preview() {
	_preview_image->fill(Color(1.00, 1.00, 1.00, 0.00));
	_preview_image_texture->create_from_image(_preview_image, 0);
}

void PaintCanvas::update_textures() {
	if (!_image->empty()) {
		_image_texture->create_from_image(_image, 0);
	}

	if (!_preview_image->empty()) {
		_preview_image_texture->create_from_image(_preview_image, 0);
	}

	update();
}

void PaintCanvas::resize_crop(int width, int height) {
	Vector2i size = get_size();

	if (size.x == width && size.y == height) {
		return;
	}

	if (width <= 0 || height <= 0) {
		_image->clear();
		_preview_image->clear();

		update_textures();

		set_size(Vector2i(width, height));

		return;
	}

	_image->crop(width, height);
	_preview_image->crop(width, height);

	update_textures();

	set_size(Vector2i(width, height));
}

void PaintCanvas::resize_interpolate(const int width, const int height, Image::Interpolation p_interpolation) {
	Vector2i size = get_size();

	if (size.x == width && size.y == height) {
		return;
	}

	if (width <= 0 || height <= 0) {
		_image->clear();
		_preview_image->clear();

		update_textures();

		set_size(Vector2i(width, height));

		return;
	}

	_image->resize(width, height, p_interpolation);
	_preview_image->resize(width, height, p_interpolation);

	update_textures();

	set_size(Vector2i(width, height));
}

PoolVector2iArray PaintCanvas::select_color(const int p_x, const int p_y) {
	PoolVector2iArray same_color_pixels;

	Color color = get_pixel(p_x, p_y);
	for (int x = 0; x < get_size().x; ++x) {
		for (int y = 0; y < get_size().y; ++y) {
			Color pixel_color = get_pixel(x, y);

			if (pixel_color == color) {
				same_color_pixels.append(Vector2i(x, y));
			}
		}
	}

	return same_color_pixels;
}
PoolVector2iArray PaintCanvas::select_same_color(const int p_x, const int p_y) {
	return get_neighbouring_pixels(p_x, p_y);
}

// yoinked from
// https://www.geeksforgeeks.org/flood-fill-algorithm-implement-fill-paint/
PoolVector2iArray PaintCanvas::get_neighbouring_pixels(const int pos_x, const int pos_y) {
	PoolVector2iArray pixels;

	Vector<int> to_check_queue;
	Vector<int> checked_queue;

	to_check_queue.push_back(PaintUtilities::to_1D(pos_x, pos_y, get_size().x));

	Color color = get_pixel(pos_x, pos_y);

	while (!to_check_queue.empty()) {
		int idx = to_check_queue[0];
		to_check_queue.remove(0);
		Vector2i p = PaintUtilities::to_2D(idx, get_size().x);

		if (checked_queue.find(idx) != -1) {
			continue;
		}

		checked_queue.push_back(idx);

		if (get_pixel(p.x, p.y) != color) {
			continue;
		}

		// add to result
		pixels.push_back(p);

		// check neighbours
		int x = p.x - 1;
		int y = p.y;
		if (is_inside_canvas(x, y)) {
			idx = PaintUtilities::to_1D(x, y, get_size().x);
			to_check_queue.push_back(idx);
		}

		x = p.x + 1;
		if (is_inside_canvas(x, y)) {
			idx = PaintUtilities::to_1D(x, y, get_size().x);
			to_check_queue.push_back(idx);
		}

		x = p.x;
		y = p.y - 1;
		if (is_inside_canvas(x, y)) {
			idx = PaintUtilities::to_1D(x, y, get_size().x);
			to_check_queue.push_back(idx);
		}

		y = p.y + 1;
		if (is_inside_canvas(x, y)) {
			idx = PaintUtilities::to_1D(x, y, get_size().x);
			to_check_queue.push_back(idx);
		}
	}

	return pixels;
}

Ref<Image> PaintCanvas::get_image() {
	return _image;
}
Ref<Image> PaintCanvas::get_preview_image() {
	return _preview_image;
}

Ref<ImageTexture> PaintCanvas::get_image_texture() {
	return _image_texture;
}
Ref<ImageTexture> PaintCanvas::get_preview_image_texture() {
	return _preview_image_texture;
}

Ref<Image> PaintCanvas::_get_save_image() {
	return _image;
}

PoolByteArray PaintCanvas::get_image_data_compressed() {
	PoolByteArray arr;
#ifdef MODULE_LZ4_ENABLED
	if (_image->empty()) {
		return arr;
	}

	PoolByteArray data = _image->get_data();

	int size = data.size();
	PoolByteArray::Read dr = data.read();
	const uint8_t *ch = dr.ptr();

	int bound = LZ4Compressor::LZ4_compressBound(size);
	int additional_data_size = 3 * sizeof(int);
	arr.resize(bound + additional_data_size);

	PoolByteArray::Write w = arr.write();
	uint8_t *wptr = w.ptr();
	Vector2i node_size = get_size();
	reinterpret_cast<int *>(wptr)[0] = node_size.x;
	reinterpret_cast<int *>(wptr)[1] = node_size.y;
	reinterpret_cast<int *>(wptr)[2] = data.size();

	int ns = LZ4Compressor::LZ4_compress_default(reinterpret_cast<const char *>(ch), reinterpret_cast<char *>(&wptr[additional_data_size]), size, bound);

	w.release();

	arr.resize(ns + additional_data_size);
#endif
	return arr;
}
void PaintCanvas::set_image_data_compressed(const PoolByteArray &data) {
#ifdef MODULE_LZ4_ENABLED
	int additional_data_size = 3 * sizeof(int);
	int ds = data.size();

	if (ds <= additional_data_size) {
		set_size(Vector2i());
		return;
	}

	PoolByteArray::Read dr = data.read();
	const uint8_t *ch = dr.ptr();

	Vector2i node_size;
	node_size.x = reinterpret_cast<const int *>(ch)[0];
	node_size.y = reinterpret_cast<const int *>(ch)[1];
	int size = reinterpret_cast<const int *>(ch)[2];

	PoolByteArray arr;
	arr.resize(size);
	PoolByteArray::Write w = arr.write();
	uint8_t *wptr = w.ptr();

	int ns = LZ4Compressor::LZ4_decompress_safe(reinterpret_cast<const char *>(&ch[additional_data_size]), reinterpret_cast<char *>(wptr), ds - additional_data_size, size);

	if (ns != size) {
		arr.resize(ns);
		ERR_PRINT("ns != size");
		return;
	}

	set_size(node_size);
	_image->create(node_size.x, node_size.y, false, Image::FORMAT_RGBA8, arr);
	update_textures();
#endif
}

void PaintCanvas::handle_draw(const Vector2 &local_position, const Ref<InputEvent> &event) {
	PaintProject *proj = get_paint_project();

	if (!proj) {
		ERR_FAIL_COND(!proj);
	}

	set_pixel(local_position.x, local_position.y, proj->get_current_color());
}

Color PaintCanvas::get_current_color() {
	PaintProject *proj = get_paint_project();

	if (!proj) {
		ERR_FAIL_COND_V(!proj, Color(1, 1, 1, 1));
	}

	return proj->get_current_color();
}

void PaintCanvas::update_mouse_position(const Vector2 &local_position, const Ref<InputEvent> &event) {
	if (event->get_device() == -1) {
		_mouse_position = get_global_mouse_position();
		_cell_mouse_position = local_position;

		_last_mouse_position = _mouse_position;
		_last_cell_mouse_position = local_position;
	}
}

void PaintCanvas::handle_left_mouse_button_down(const Vector2 &local_position, const Ref<InputEvent> &event) {
	update_mouse_position(local_position, event);

	switch (_current_tool) {
		case TOOL_CUT: {
			if (!event->is_pressed()) {
				commit_action();
			}

		} break;
		case TOOL_BUCKET: {
			if (!_current_action.is_valid()) {
				_current_action = get_action();
			}

			Array arr;
			arr.push_back(_cell_mouse_position);
			arr.push_back(_last_cell_mouse_position);
			arr.push_back(get_current_color());

			do_action(arr);
			commit_action();
		} break;
		case TOOL_COLORPICKER: {
			Color c = get_pixel(_cell_mouse_position.x, _cell_mouse_position.y);

			if (c.a < 0.00001) {
				return;
			}

			_picked_color = true;

			PaintProject *project = get_paint_project();

			if (project) {
				project->set_current_color(c);
			}
		} break;
		default:
			break;
	}

	tool_process(local_position, event);
}

void PaintCanvas::handle_left_mouse_button_up(const Vector2 &local_position, const Ref<InputEvent> &event) {
	update_mouse_position(local_position, event);

	if (_current_tool == TOOL_COLORPICKER) {
		if (_picked_color) {
			set_current_tool(get_previous_tool());
			_picked_color = false;
		}
	}

	tool_process(local_position, event);
}

void PaintCanvas::handle_right_mouse_button_down(const Vector2 &local_position, const Ref<InputEvent> &event) {
	update_mouse_position(local_position, event);

	switch (_current_tool) {
		case TOOL_CUT:
			if (!event->is_pressed()) {
				commit_action();
			}
			break;
		case TOOL_COLORPICKER:
			set_current_tool(get_previous_tool());
			break;
		case TOOL_PASTECUT:
			commit_action();
			set_current_tool(TOOL_PENCIL);
			break;
		case TOOL_BUCKET:
			set_current_tool(get_previous_tool());
			break;
		default:
			break;
	}

	tool_process(local_position, event);
}

void PaintCanvas::draw_brush_preview() {
	clear_preview();

	switch (_current_tool) {
		case TOOL_PASTECUT: {
			for (int idx = 0; idx < _selection_cells.size(); ++idx) {
				Vector2i pixel = _selection_cells[idx];
				Color color = _selection_colors[idx];
				pixel -= _cut_pos + _cut_size / 2;
				pixel.x += _cell_mouse_position.x;
				pixel.y += _cell_mouse_position.y;
				set_preview_pixel_v(pixel, color);
			}

			update_textures();
		} break;
		case TOOL_BRUSH: {
			PoolVector2iArray pixels = BrushPrefabs::get_brush(static_cast<BrushPrefabs::Type>(_brush_prefab), _brush_size);
			Color color = get_current_color();

			PoolVector2iArray::Read r = pixels.read();

			for (int i = 0; i < pixels.size(); ++i) {
				Vector2i pixel = r[i];
				set_preview_pixel(_cell_mouse_position.x + pixel.x, _cell_mouse_position.y + pixel.y, color);
				//print_error("ad " + String::num(cell_mouse_position.x + pixel.x) + " " + String::num(cell_mouse_position.y + pixel.y));
			}

			r.release();

			update_textures();
		} break;
		case TOOL_RAINBOW: {
			set_preview_pixel(_cell_mouse_position.x, _cell_mouse_position.y, Color(0.46875, 0.446777, 0.446777, 0.3));
			update_textures();
		} break;
		case TOOL_COLORPICKER: {
			set_preview_pixel(_cell_mouse_position.x, _cell_mouse_position.y, Color(0.866667, 0.847059, 0.847059, 0.3));
			update_textures();
		} break;
		default: {
			set_preview_pixel(_cell_mouse_position.x, _cell_mouse_position.y, get_current_color());
			update_textures();
		} break;
	}
}

void PaintCanvas::do_action(const Array &arr) {
	if (!_current_action.is_valid()) {
		return;
	}

	_current_action->do_action(arr);
	update_textures();
}

void PaintCanvas::commit_action() {
	if (!_current_action.is_valid()) {
		return;
	}

	_current_action->commit_action();

	_actions_history.push_back(_current_action);
	_redo_history.clear();
	update_textures();

	if (_current_tool == TOOL_CUT) {
		Ref<CutAction> ca = _current_action;

		ERR_FAIL_COND(!ca.is_valid());

		_cut_pos = ca->get_mouse_start_pos();
		_cut_size = ca->get_mouse_end_pos() - ca->get_mouse_start_pos();

		_selection_cells.clear();
		_selection_colors.clear();

		_selection_cells.append_array(ca->get_redo_cells());
		_selection_colors.append_array(ca->get_redo_colors());

		set_current_tool(TOOL_PASTECUT);
	}

	_current_action.unref();
}

void PaintCanvas::redo_action() {
	if (_redo_history.empty()) {
		//print("PaintCanvas: nothing to redo");
		return;
	}

	Ref<PaintAction> action = _redo_history[_redo_history.size() - 1];
	_redo_history.remove(_redo_history.size() - 1);

	if (!action.is_valid()) {
		return;
	}

	_actions_history.push_back(action);
	action->redo_action();
	update_textures();

	//print("PaintCanvas: redo action");
}

void PaintCanvas::undo_action() {
	if (_actions_history.empty()) {
		//print("PaintCanvas: nothing to undo");
		return;
	}

	Ref<PaintAction> action = _actions_history[_actions_history.size() - 1];
	_actions_history.remove(_actions_history.size() - 1);

	if (!action.is_valid()) {
		return;
	}

	_redo_history.push_back(action);
	action->undo_action();
	update_textures();

	//print("PaintCanvas: undo action")
}

bool PaintCanvas::has_point(const Vector2 &pos) {
	Vector2i size = get_size();

	if (pos.x < 0 || pos.y < 0 || pos.x > size.x || pos.y > size.y) {
		return false;
	}

	return true;
}

Ref<PaintAction> PaintCanvas::get_action() {
	Ref<PaintAction> action;

	switch (_current_tool) {
		case TOOL_PENCIL:
			action = Ref<PaintAction>(memnew(PencilAction));
			break;
		case TOOL_BRUSH:
			action = Ref<PaintAction>(memnew(BrushAction));
			break;
		case TOOL_LINE:
			action = Ref<PaintAction>(memnew(LineAction));
			break;
		case TOOL_RAINBOW:
			action = Ref<PaintAction>(memnew(RainbowAction));
			break;
		case TOOL_BUCKET:
			action = Ref<PaintAction>(memnew(BucketAction));
			break;
		case TOOL_RECT:
			action = Ref<PaintAction>(memnew(RectAction));
			break;
		case TOOL_DARKEN:
			action = Ref<PaintAction>(memnew(DarkenAction));
			break;
		case TOOL_BRIGHTEN:
			action = Ref<PaintAction>(memnew(BrightenAction));
			break;
		case TOOL_CUT:
			action = Ref<PaintAction>(memnew(CutAction));
			break;
		case TOOL_PASTECUT:
			action = Ref<PaintAction>(memnew(PasteCutAction));
			break;
		default:
			break;
	}

	if (action.is_valid()) {
		action->set_paint_canvas(this);
	}

	return action;
}

void PaintCanvas::_on_tool_changed() {
	if (_current_tool == TOOL_COLORPICKER) {
		if (_current_action.is_valid()) {
			_current_action.unref();
		}

		return;
	}

	if (get_previous_tool() == TOOL_CUT) {
		clear_preview();
	}

	_current_action = get_action();
}

void PaintCanvas::tool_process(const Vector2 &local_position, const Ref<InputEvent> &event) {
	if (_current_tool == TOOL_COLORPICKER) {
		return;
	}

	if (!_current_action.is_valid()) {
		_current_action = get_action();
	}

	if (_current_tool == TOOL_PENCIL || _current_tool == TOOL_LINE || _current_tool == TOOL_RECT) {
		Array arr;

		arr.push_back(_cell_mouse_position);
		arr.push_back(_last_cell_mouse_position);

		if (_mouse_button_down == BUTTON_LEFT) {
			arr.push_back(get_current_color());
		} else if (_mouse_button_down == BUTTON_RIGHT) {
			arr.push_back(Color(1, 1, 1, 0));
		}

		do_action(arr);
	} else if (_current_tool == TOOL_DARKEN || _current_tool == TOOL_BRIGHTEN || _current_tool == TOOL_CUT) {
		Array arr;

		arr.push_back(_cell_mouse_position);
		arr.push_back(_last_cell_mouse_position);
		arr.push_back(get_current_color());

		do_action(arr);
	} else if (_current_tool == TOOL_BRUSH) {
		Array arr;

		arr.push_back(_cell_mouse_position);
		arr.push_back(_last_cell_mouse_position);

		if (_mouse_button_down == BUTTON_LEFT) {
			arr.push_back(get_current_color());
		} else if (_mouse_button_down == BUTTON_RIGHT) {
			arr.push_back(Color(1, 1, 1, 0));
		}

		arr.push_back(_brush_prefab);
		arr.push_back(_brush_size);

		do_action(arr);
	} else if (_current_tool == TOOL_COLORPICKER) {
		// Nothing to do here
	} else if (_current_tool == TOOL_PASTECUT) {
		Array arr;

		arr.append(_cell_mouse_position);
		arr.append(_last_cell_mouse_position);
		arr.append(_selection_cells);
		arr.append(_selection_colors);
		arr.append(_cut_pos);
		arr.append(_cut_size);

		do_action(arr);
	} else if (_current_tool == TOOL_RAINBOW) {
		Array arr;

		arr.push_back(_cell_mouse_position);
		arr.push_back(_last_cell_mouse_position);

		do_action(arr);
	}
}

bool PaintCanvas::_forward_canvas_gui_input(const Ref<InputEvent> &event) {
	if (!is_visible_in_tree()) {
		return false;
	}

	Ref<InputEventMouseButton> iemb = event;
	if (iemb.is_valid()) {
		if (_mouse_down && _mouse_button_down != iemb->get_button_index()) {
			// Ignore it, but consume the event from the editor
			return true;
		}

		if (iemb->get_button_index() != BUTTON_LEFT && iemb->get_button_index() != BUTTON_RIGHT) {
			return false;
		}

		// This seems to be the easiest way to get local mouse position,
		// even though the event is available
		Vector2 local_position = get_local_mouse_position();

		if (_mouse_down) {
			if (!iemb->is_pressed()) {
				_mouse_down = false;
				_mouse_button_down = -1;

				if (_mouse_button_down == BUTTON_LEFT) {
					handle_left_mouse_button_up(local_position, iemb);
				}

				commit_action();
			}
		} else {
			if (has_point(local_position)) {
				_mouse_down = true;
				_mouse_button_down = iemb->get_button_index();

				clear_preview();

				if (_mouse_button_down == BUTTON_LEFT) {
					handle_left_mouse_button_down(local_position, iemb);
				} else if (_mouse_button_down == BUTTON_RIGHT) {
					handle_right_mouse_button_down(local_position, iemb);
				}

				return true;
			}
		}
	}

	Ref<InputEventMouseMotion> iemm = event;
	if (iemm.is_valid()) {
		Vector2 local_position = get_local_mouse_position();

		_mouse_position = get_global_mouse_position();
		_cell_mouse_position = local_position;

		if (has_point(local_position)) {
			_was_mouse_outside = false;

			if (_mouse_down) {
				//handle_draw(local_position, event)
				_cell_mouse_position = local_position;

				tool_process(local_position, event);
				update_textures();
				update();

				_last_mouse_position = _mouse_position;
				_last_cell_mouse_position = local_position;

				return true;
			} else {
				draw_brush_preview();
			}
		} else {
			if (!_was_mouse_outside) {
				clear_preview();
				_was_mouse_outside = true;
			}
		}

		_last_mouse_position = _mouse_position;
		_last_cell_mouse_position = local_position;
	}

	Ref<InputEventKey> iek = event;
	if (iek.is_valid()) {
		if (iek->is_echo() || !iek->is_pressed()) {
			return false;
		}

		int scancode = iek->get_physical_scancode_with_modifiers();

		bool undo = false;
		if (scancode == (KEY_Z | KEY_MASK_CTRL)) {
			undo = true;
		}

		bool redo = false;
		if (scancode == (KEY_Z | KEY_MASK_CTRL | KEY_MASK_SHIFT)) {
			redo = true;
		}

		if (!undo && !redo) {
			return false;
		}

		Vector2 local_position = get_local_mouse_position();

		if (has_point(local_position)) {
			if (redo) {
				redo_action();
				return true;
			}

			if (undo) {
				undo_action();
				return true;
			}
		}
	}

	return false;
}

PaintCanvas::PaintCanvas() {
	_symmetry_x = false;
	_symmetry_y = false;
	_alpha_locked = false;
	_brush_prefab = 0;
	_brush_size = 1;
	_current_tool = 0;
	_previous_tool = 0;
	_was_mouse_outside = true;

	_image.instance();
	_preview_image.instance();

	_image_texture.instance();
	_preview_image_texture.instance();

	_mouse_down = false;
	_mouse_button_down = -1;
	_picked_color = false;
}

PaintCanvas::~PaintCanvas() {
}

void PaintCanvas::_on_size_changed() {
	Vector2i size = get_size();

	if (size.x <= 0 || size.y <= 0) {
		_image->clear();
		_preview_image->clear();

		update_textures();

		return;
	}

	if (_image->empty()) {
		_image->create(size.x, size.y, false, Image::FORMAT_RGBA8);
	} else {
		_image->crop(size.x, size.y);
	}

	if (_preview_image->empty()) {
		_preview_image->create(size.x, size.y, false, Image::FORMAT_RGBA8);
	} else {
		_preview_image->crop(size.x, size.y);
	}

	update_textures();
}

void PaintCanvas::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_POSTINITIALIZE: {
			connect("size_changed", this, "_on_size_changed");
		} break;
		case NOTIFICATION_READY: {
			if (!is_connected("current_tool_changed", this, "_on_tool_changed")) {
				connect("current_tool_changed", this, "_on_tool_changed");
			}

			_on_tool_changed();
		} break;
		case NOTIFICATION_DRAW: {
			if (!_image->empty()) {
				draw_texture(_image_texture, Point2());
			}

			if (!_preview_image->empty()) {
				draw_texture(_preview_image_texture, Point2());
			}
		} break;
	}
}

void PaintCanvas::_bind_methods() {
	ADD_SIGNAL(MethodInfo("brush_prefab_changed"));
	ADD_SIGNAL(MethodInfo("brush_size_changed"));
	ADD_SIGNAL(MethodInfo("current_tool_changed"));

	ClassDB::bind_method(D_METHOD("get_symmetry_x"), &PaintCanvas::get_symmetry_x);
	ClassDB::bind_method(D_METHOD("set_symmetry_x", "val"), &PaintCanvas::set_symmetry_x);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "symmetry_x"), "set_symmetry_x", "get_symmetry_x");

	ClassDB::bind_method(D_METHOD("get_symmetry_y"), &PaintCanvas::get_symmetry_y);
	ClassDB::bind_method(D_METHOD("set_symmetry_y", "val"), &PaintCanvas::set_symmetry_y);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "symmetry_y"), "set_symmetry_y", "get_symmetry_y");

	ClassDB::bind_method(D_METHOD("get_alpha_locked"), &PaintCanvas::get_alpha_locked);
	ClassDB::bind_method(D_METHOD("set_alpha_locked", "val"), &PaintCanvas::set_alpha_locked);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "alpha_locked"), "set_alpha_locked", "get_alpha_locked");

	ClassDB::bind_method(D_METHOD("get_brush_prefab"), &PaintCanvas::get_brush_prefab);
	ClassDB::bind_method(D_METHOD("set_brush_prefab", "val"), &PaintCanvas::set_brush_prefab);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "brush_prefab"), "set_brush_prefab", "get_brush_prefab");

	ClassDB::bind_method(D_METHOD("get_brush_size"), &PaintCanvas::get_brush_size);
	ClassDB::bind_method(D_METHOD("set_brush_size", "val"), &PaintCanvas::set_brush_size);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "brush_size"), "set_brush_size", "get_brush_size");

	ClassDB::bind_method(D_METHOD("get_current_tool"), &PaintCanvas::get_current_tool);
	ClassDB::bind_method(D_METHOD("set_current_tool", "val"), &PaintCanvas::set_current_tool);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "current_tool"), "set_current_tool", "get_current_tool");

	ClassDB::bind_method(D_METHOD("get_previous_tool"), &PaintCanvas::get_previous_tool);

	ClassDB::bind_method(D_METHOD("get_image_data_compressed"), &PaintCanvas::get_image_data_compressed);
	ClassDB::bind_method(D_METHOD("set_image_data_compressed", "val"), &PaintCanvas::set_image_data_compressed);
	ADD_PROPERTY(PropertyInfo(Variant::POOL_BYTE_ARRAY, "image_data_compressed"), "set_image_data_compressed", "get_image_data_compressed");

	ClassDB::bind_method(D_METHOD("is_inside_canvas", "x", "y"), &PaintCanvas::is_inside_canvas);
	ClassDB::bind_method(D_METHOD("set_pixel_arr", "pixels", "color"), &PaintCanvas::set_pixel_arr);

	ClassDB::bind_method(D_METHOD("set_pixel_v", "pos", "color"), &PaintCanvas::set_pixel_v);
	ClassDB::bind_method(D_METHOD("set_pixel", "x", "y", "color"), &PaintCanvas::set_pixel);

	ClassDB::bind_method(D_METHOD("get_pixel_v", "pos"), &PaintCanvas::get_pixel_v);
	ClassDB::bind_method(D_METHOD("get_pixel", "x", "y"), &PaintCanvas::get_pixel);

	ClassDB::bind_method(D_METHOD("set_preview_pixel_v", "pos", "color"), &PaintCanvas::set_preview_pixel_v);
	ClassDB::bind_method(D_METHOD("set_preview_pixel", "x", "y", "color"), &PaintCanvas::set_preview_pixel);

	ClassDB::bind_method(D_METHOD("get_preview_pixel_v", "pos"), &PaintCanvas::get_preview_pixel_v);
	ClassDB::bind_method(D_METHOD("get_preview_pixel", "x", "y"), &PaintCanvas::get_preview_pixel);

	ClassDB::bind_method(D_METHOD("validate_pixel_v", "pos"), &PaintCanvas::validate_pixel_v);

	ClassDB::bind_method(D_METHOD("clear"), &PaintCanvas::clear);
	ClassDB::bind_method(D_METHOD("clear_preview"), &PaintCanvas::clear_preview);
	ClassDB::bind_method(D_METHOD("update_textures"), &PaintCanvas::update_textures);

	ClassDB::bind_method(D_METHOD("resize_crop", "width", "height"), &PaintCanvas::resize_crop);
	ClassDB::bind_method(D_METHOD("resize_interpolate", "width", "height", "interpolation"), &PaintCanvas::resize_interpolate, Image::INTERPOLATE_BILINEAR);

	ClassDB::bind_method(D_METHOD("select_color", "x", "y"), &PaintCanvas::select_color);
	ClassDB::bind_method(D_METHOD("select_same_color", "x", "y"), &PaintCanvas::select_same_color);
	ClassDB::bind_method(D_METHOD("get_neighbouring_pixels", "x", "y"), &PaintCanvas::get_neighbouring_pixels);

	ClassDB::bind_method(D_METHOD("get_image"), &PaintCanvas::get_image);
	ClassDB::bind_method(D_METHOD("get_preview_image"), &PaintCanvas::get_preview_image);

	ClassDB::bind_method(D_METHOD("get_image_texture"), &PaintCanvas::get_image_texture);
	ClassDB::bind_method(D_METHOD("get_preview_image_texture"), &PaintCanvas::get_preview_image_texture);

	ClassDB::bind_method(D_METHOD("handle_draw", "local_position", "event"), &PaintCanvas::handle_draw);
	ClassDB::bind_method(D_METHOD("get_current_color"), &PaintCanvas::get_current_color);

	ClassDB::bind_method(D_METHOD("draw_brush_preview"), &PaintCanvas::draw_brush_preview);

	ClassDB::bind_method(D_METHOD("do_action", "arr"), &PaintCanvas::do_action);
	ClassDB::bind_method(D_METHOD("commit_action"), &PaintCanvas::commit_action);
	ClassDB::bind_method(D_METHOD("redo_action"), &PaintCanvas::redo_action);
	ClassDB::bind_method(D_METHOD("undo_action"), &PaintCanvas::undo_action);

	ClassDB::bind_method(D_METHOD("has_point", "pos"), &PaintCanvas::has_point);

	ClassDB::bind_method(D_METHOD("get_action"), &PaintCanvas::get_action);
	ClassDB::bind_method(D_METHOD("_on_tool_changed"), &PaintCanvas::_on_tool_changed);
	ClassDB::bind_method(D_METHOD("tool_process", "local_position", "event"), &PaintCanvas::tool_process);

	ClassDB::bind_method(D_METHOD("_on_size_changed"), &PaintCanvas::_on_size_changed);

	BIND_ENUM_CONSTANT(TOOL_PENCIL);
	BIND_ENUM_CONSTANT(TOOL_BRUSH);
	BIND_ENUM_CONSTANT(TOOL_BUCKET);
	BIND_ENUM_CONSTANT(TOOL_RAINBOW);
	BIND_ENUM_CONSTANT(TOOL_LINE);
	BIND_ENUM_CONSTANT(TOOL_RECT);
	BIND_ENUM_CONSTANT(TOOL_DARKEN);
	BIND_ENUM_CONSTANT(TOOL_BRIGHTEN);
	BIND_ENUM_CONSTANT(TOOL_COLORPICKER);
	BIND_ENUM_CONSTANT(TOOL_CUT);
	BIND_ENUM_CONSTANT(TOOL_PASTECUT);
}
