#include "register_types.h"

#include "core/version.h"

#if VERSION_MAJOR < 4
#include "core/class_db.h"
#else
#include "core/object/class_db.h"
#endif

#include "noise.h"
#include "fastnoise_noise_params.h"


void register_fastnoise_types() {

	ClassDB::register_class<FastNoise>();
	ClassDB::register_class<FastnoiseNoiseParams>();
}


void unregister_fastnoise_types() {

}


