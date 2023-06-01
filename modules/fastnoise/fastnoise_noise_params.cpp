#include "fastnoise_noise_params.h"

void FastnoiseNoiseParams::setup_noise(Ref<FastNoise> noise) {
	noise->set_seed(_seed);
	noise->set_noise_type(_noise_type);
	noise->set_interpolation(_interp);
	noise->set_frequency(_freq);
	noise->set_fractal_octaves(_octave_count);
	noise->set_fractal_lacunarity(_lacunarity);
	noise->set_fractal_gain(_gain);
	noise->set_fractal_type(_type);
	noise->set_cellular_distance_function(_func);
	noise->set_cellular_return_type(_ret);
	noise->set_gradient_perturbation_amplitude(_amp);
}

FastnoiseNoiseParams::FastnoiseNoiseParams() {
	_seed = 1337;
	_noise_type = FastNoise::TYPE_SIMPLEX;
	_interp = FastNoise::INTERP_QUINTIC;
	_freq = 0.01;
	_octave_count = 3;
	_lacunarity = 2;
	_gain = 0.5;
	_type = FastNoise::FRACTAL_FBM;
	_func = FastNoise::DISTANCE_EUCLIDEAN;
	_ret = FastNoise::RETURN_CELL_VALUE;
	_jitter = 0.45;
	_amp = 1;
}
FastnoiseNoiseParams::~FastnoiseNoiseParams() {
}

void FastnoiseNoiseParams::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_seed", "seed"), &FastnoiseNoiseParams::set_seed);
	ClassDB::bind_method(D_METHOD("get_seed"), &FastnoiseNoiseParams::get_seed);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "seed"), "set_seed", "get_seed");

	ClassDB::bind_method(D_METHOD("set_noise_type", "type"), &FastnoiseNoiseParams::set_noise_type);
	ClassDB::bind_method(D_METHOD("get_noise_type"), &FastnoiseNoiseParams::get_noise_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "noise_type", PROPERTY_HINT_ENUM, FastNoise::BINDING_STRING_TYPE), "set_noise_type", "get_noise_type");

	ClassDB::bind_method(D_METHOD("set_interpolation", "interp"), &FastnoiseNoiseParams::set_interpolation);
	ClassDB::bind_method(D_METHOD("get_interpolation"), &FastnoiseNoiseParams::get_interpolation);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "interpolation", PROPERTY_HINT_ENUM, FastNoise::BINDING_STRING_INTERPOLATION), "set_interpolation", "get_interpolation");

	ClassDB::bind_method(D_METHOD("set_frequency", "freq"), &FastnoiseNoiseParams::set_frequency);
	ClassDB::bind_method(D_METHOD("get_frequency"), &FastnoiseNoiseParams::get_frequency);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "frequency"), "set_frequency", "get_frequency");

	ClassDB::bind_method(D_METHOD("set_fractal_gain", "gain"), &FastnoiseNoiseParams::set_fractal_gain);
	ClassDB::bind_method(D_METHOD("get_fractal_gain"), &FastnoiseNoiseParams::get_fractal_gain);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "fractal_gain"), "set_fractal_gain", "get_fractal_gain");

	ClassDB::bind_method(D_METHOD("set_fractal_type", "type"), &FastnoiseNoiseParams::set_fractal_type);
	ClassDB::bind_method(D_METHOD("get_fractal_type"), &FastnoiseNoiseParams::get_fractal_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "fractal_type", PROPERTY_HINT_ENUM, FastNoise::BINDING_STRING_FRACTAL_TYPE), "set_fractal_type", "get_fractal_type");

	ClassDB::bind_method(D_METHOD("set_fractal_octaves", "octave_count"), &FastnoiseNoiseParams::set_fractal_octaves);
	ClassDB::bind_method(D_METHOD("get_fractal_octaves"), &FastnoiseNoiseParams::get_fractal_octaves);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "fractal_octaves"), "set_fractal_octaves", "get_fractal_octaves");

	ClassDB::bind_method(D_METHOD("set_fractal_lacunarity", "lacunarity"), &FastnoiseNoiseParams::set_fractal_lacunarity);
	ClassDB::bind_method(D_METHOD("get_fractal_lacunarity"), &FastnoiseNoiseParams::get_fractal_lacunarity);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "fractal_lacunarity"), "set_fractal_lacunarity", "get_fractal_lacunarity");

	ClassDB::bind_method(D_METHOD("set_cellular_distance_function", "func"), &FastnoiseNoiseParams::set_cellular_distance_function);
	ClassDB::bind_method(D_METHOD("get_cellular_distance_function"), &FastnoiseNoiseParams::get_cellular_distance_function);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "cellular_distance_function", PROPERTY_HINT_ENUM, FastNoise::BINDING_STRING_CELLULAR_DISTANCE_FUNCTION), "set_cellular_distance_function", "get_cellular_distance_function");

	ClassDB::bind_method(D_METHOD("set_cellular_return_type", "ret"), &FastnoiseNoiseParams::set_cellular_return_type);
	ClassDB::bind_method(D_METHOD("get_cellular_return_type"), &FastnoiseNoiseParams::get_cellular_return_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "cellular_return_type", PROPERTY_HINT_ENUM, FastNoise::BINDING_STRING_CELLULAR_RETURN_TYPE), "set_cellular_return_type", "get_cellular_return_type");

	ClassDB::bind_method(D_METHOD("set_cellular_jitter", "jitter"), &FastnoiseNoiseParams::set_cellular_jitter);
	ClassDB::bind_method(D_METHOD("get_cellular_jitter"), &FastnoiseNoiseParams::get_cellular_jitter);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "cellular_jitter"), "set_cellular_jitter", "get_cellular_jitter");

	ClassDB::bind_method(D_METHOD("set_gradient_perturbation_amplitude", "amp"), &FastnoiseNoiseParams::set_gradient_perturbation_amplitude);
	ClassDB::bind_method(D_METHOD("get_gradient_perturbation_amplitude"), &FastnoiseNoiseParams::get_gradient_perturbation_amplitude);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "gradient_perturbation_amplitude"), "set_gradient_perturbation_amplitude", "get_gradient_perturbation_amplitude");

	ClassDB::bind_method(D_METHOD("setup_noise", "noise"), &FastnoiseNoiseParams::setup_noise);
}
