#ifndef IMAGE_INDEXER_H
#define IMAGE_INDEXER_H

#include "core/image.h"
#include "core/reference.h"
#include "core/variant.h"
#include "core/containers/oa_hash_map.h"

class ImageIndexer : public Reference {
	GDCLASS(ImageIndexer, Reference);

public:
	PoolColorArray get_colors();

	PoolIntArray index_image(Ref<Image> image);
	void reset();

	PoolByteArray indices_to_argb8_data(const PoolIntArray &indices);

	ImageIndexer();
	~ImageIndexer();

protected:
	static void _bind_methods();

private:
	PoolColorArray _colors;
	OAHashMap<Color, int> _col_map;
};

#endif
