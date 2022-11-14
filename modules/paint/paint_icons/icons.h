#ifndef PAINT_ICONS_H
#define PAINT_ICONS_H

#include "paint_icons.h"

#include "core/io/image.h"
#include "scene/resources/texture.h"

class PaintIcons {
public:
	template <class T>
	static Ref<Texture> make_icon(T p_src) {
		float scale = 1;

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

	static Ref<Texture> make_icon_arrow_down_png() {
		return make_icon(arrow_down_png);
	}

	static Ref<Texture> make_icon_arrow_up_png()  {
		return make_icon(arrow_up_png);
	}

	static Ref<Texture> make_icon_brush_circle_png()  {
		return make_icon(brush_circle_png);
	}

	static Ref<Texture> make_icon_brush_circle_hovered_png()  {
		return make_icon(brush_circle_hovered_png);
	}

	static Ref<Texture> make_icon_brush_h_line_png()  {
		return make_icon(brush_h_line_png);
	}

	static Ref<Texture> make_icon_brush_h_line_hovered_png()  {
		return make_icon(brush_h_line_hovered_png);
	}

	static Ref<Texture> make_icon_brush_rect_png()  {
		return make_icon(brush_rect_png);
	}

	static Ref<Texture> make_icon_brush_rect_hovered_png()  {
		return make_icon(brush_rect_hovered_png);
	}

	static Ref<Texture> make_icon_brush_v_line_png()  {
		return make_icon(brush_v_line_png);
	}

	static Ref<Texture> make_icon_brush_v_line_hovered_png()  {
		return make_icon(brush_v_line_hovered_png);
	}

	static Ref<Texture> make_icon_grid_png()  {
		return make_icon(grid_png);
	}

	static Ref<Texture> make_icon_lock_layer_png()  {
		return make_icon(lock_layer_png);
	}

	static Ref<Texture> make_icon_lock_layer_1_png()  {
		return make_icon(lock_layer_1_png);
	}

	static Ref<Texture> make_icon_minidotta_png()  {
		return make_icon(minidotta_png);
	}

	static Ref<Texture> make_icon_minidotta_invis_png()  {
		return make_icon(minidotta_invis_png);
	}

    static Ref<Texture> make_icon_unlock_layer_png()  {
		return make_icon(unlock_layer_png);
	}
};

#endif
