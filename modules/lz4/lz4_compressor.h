#ifndef LZ4_COMPRESSOR_H
#define LZ4_COMPRESSOR_H

#include "core/object/reference.h"
#include "core/string/ustring.h"
#include "core/variant/variant.h"

class LZ4Compressor : public Reference {
	GDCLASS(LZ4Compressor, Reference);

public:
	PoolByteArray compress_data(const PoolByteArray &data) const;
	PoolByteArray decompress_data(const PoolByteArray &data) const;

	LZ4Compressor();
	~LZ4Compressor();

private:
	static void _bind_methods();

private:
};

#endif
