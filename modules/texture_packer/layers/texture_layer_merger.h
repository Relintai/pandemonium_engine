#ifndef TEXTURE_LAYER_MERGER_H
#define TEXTURE_LAYER_MERGER_H


#include "core/io/image.h"
#include "core/object/reference.h"
#include "core/containers/vector.h"

#include "core/math/rect2.h"
#include "scene/resources/texture.h"

class TextureLayerMerger : public Reference {
	GDCLASS(TextureLayerMerger, Reference);

public:
	int get_width() const;
	void set_width(const int p_value);

	int get_height() const;
	void set_height(const int p_value);

	uint32_t get_texture_flags() const;
	void set_texture_flags(const uint32_t p_flags);

	Color get_base_color() const;
	void set_base_color(const Color &p_color);

	Ref<Image> get_data() const;
	void set_data(const Ref<Image> &p_image);

	Ref<ImageTexture> get_result_as_texture() const;

	void add_texture(const Ref<Texture> &p_texture, const Color &p_color = Color(1, 1, 1, 1), const Vector2 &p_position = Vector2(), Rect2 p_rect = Rect2());

	Ref<Texture> get_texture(const int p_index);
	void set_texture(const int p_index, const Ref<Texture> &p_texture);

	Color get_color(const int p_index);
	void set_color(const int p_index, const Color &p_color);

	Vector2 get_position(const int p_index);
	void set_position(const int p_index, const Vector2 &p_position);

	Rect2 get_rect(const int p_index);
	void set_rect(const int p_index, const Rect2 &p_rect);

	void remove_texture(const int p_index);
	int get_texture_count();
	void clear();

	void merge();

	TextureLayerMerger();
	~TextureLayerMerger();

protected:
	void write_base_color_to_array(PoolVector<uint8_t> &data);

	static void _bind_methods();

	struct LayerMergerEntry {
		Ref<Texture> texture;
		Color color;
		Rect2 rect;
		Vector2 position;
	};

private:
	int _width;
	int _height;

	uint32_t _texture_flags;

	Color _base_color;

	Ref<Image> _image;

	Vector<LayerMergerEntry> _entries;
};

#endif
