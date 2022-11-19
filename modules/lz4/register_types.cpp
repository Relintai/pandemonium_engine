
#include "register_types.h"

#include "lz4_compressor.h"

void register_lz4_types() {
	ClassDB::register_class<LZ4Compressor>();
}

void unregister_lz4_types() {
}
