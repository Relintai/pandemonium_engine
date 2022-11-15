#ifndef PAINT_CANVAS_H
#define PAINT_CANVAS_H

#include "core/object/reference.h"

#include "paint_node.h"

class Image;
class ImageTexture;

class PaintCanvas : public PaintNode {
	GDCLASS(PaintCanvas, PaintNode);

public:
	bool get_symmetry_x() const;
	void set_symmetry_x(const bool val);

	bool get_symmetry_y() const;
	void set_symmetry_y(const bool val);

	bool get_alpha_locked() const;
	void set_alpha_locked(const bool val);

	bool is_inside_canvas(const int x, const int y);

	void set_pixel_arr(const PoolVector2iArray &pixels, const Color &color);

	void set_pixel_v(const Vector2i &pos, const Color &color);
	void set_pixel(const int x, const int y, const Color &color);

	Color get_pixel_v(const Vector2i &pos);
	Color get_pixel(const int x, const int y);

	void set_preview_pixel_v(const Vector2i &pos, const Color &color);
	void set_preview_pixel(const int x, const int y, const Color &color);

	Color get_preview_pixel_v(const Vector2i &pos);
	Color get_preview_pixel(const int x, const int y);
	
	bool validate_pixel_v(const Vector2i &pos) const;

	void clear();
	void update_textures();

	PaintCanvas();
	~PaintCanvas();

protected:
	void _notification(int p_what);

	static void _bind_methods();

	bool _symmetry_x;
	bool _symmetry_y;
	bool _alpha_locked;

	Ref<Image> _image;
	Ref<Image> _preview_image;

	Ref<ImageTexture> _image_texture;
	Ref<ImageTexture> _preview_image_texture;
};

#endif
