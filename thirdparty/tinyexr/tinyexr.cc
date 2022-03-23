#if defined(_WIN32)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#endif

// -- PANDEMONIUM start --
#include <zlib.h> // Should come before including tinyexr.
// -- PANDEMONIUM end --

#define TINYEXR_IMPLEMENTATION
#include "tinyexr.h"
