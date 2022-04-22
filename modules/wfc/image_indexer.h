#ifndef IMAGE_INDEXER_H
#define IMAGE_INDEXER_H

#include "core/image.h"
#include "core/reference.h"
#include "core/variant.h"

class ImageIndexer : public Reference {
	GDCLASS(ImageIndexer, Reference);

public:
	PoolColorArray get_colors();
	PoolIntArray get_color_indices();

	void index_image(Ref<Image> image);

	PoolByteArray indices_to_argb8_data(const PoolIntArray &indices);

	ImageIndexer();
	~ImageIndexer();

protected:
	static void _bind_methods();

private:
	PoolColorArray _colors;
	PoolIntArray _color_indices;
};

#endif
