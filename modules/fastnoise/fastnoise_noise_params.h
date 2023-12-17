#ifndef FASTNOISE_NOISE_PARAMS_H
#define FASTNOISE_NOISE_PARAMS_H

/*************************************************************************/
/*  fastnoise_noise_params.h                                             */
/*************************************************************************/
/*                         This file is part of:                         */
/*                          PANDEMONIUM ENGINE                           */
/*             https://github.com/Relintai/pandemonium_engine            */
/*************************************************************************/
/* Copyright (c) 2022-present Péter Magyar.                              */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "core/object/resource.h"

#include "noise.h"

class FastnoiseNoiseParams : public Resource {
	GDCLASS(FastnoiseNoiseParams, Resource);

public:
	int get_seed() const { return _seed; }
	void set_seed(int seed) { _seed = seed; }

	void set_noise_type(FastNoise::Type noise_type) { _noise_type = noise_type; }
	FastNoise::Type get_noise_type() const { return _noise_type; }

	void set_interpolation(FastNoise::Interpolation interp) { _interp = interp; }
	FastNoise::Interpolation get_interpolation() const { return _interp; }

	void set_frequency(real_t freq) { _freq = freq; }
	real_t get_frequency() const { return _freq; }

	void set_fractal_octaves(uint32_t octave_count) { _octave_count = octave_count; }
	uint32_t get_fractal_octaves() const { return _octave_count; }

	void set_fractal_lacunarity(real_t lacunarity) { _lacunarity = lacunarity; }
	real_t get_fractal_lacunarity() const { return _lacunarity; }

	void set_fractal_gain(real_t gain) { _gain = gain; }
	real_t get_fractal_gain() const { return _gain; }

	void set_fractal_type(FastNoise::FractalType type) { _type = type; }
	FastNoise::FractalType get_fractal_type() const { return _type; }

	void set_cellular_distance_function(FastNoise::CellularDistanceFunction func) { _func = func; }
	FastNoise::CellularDistanceFunction get_cellular_distance_function() const { return _func; }

	void set_cellular_return_type(FastNoise::CellularReturnType ret) { _ret = ret; }
	FastNoise::CellularReturnType get_cellular_return_type() const { return _ret; }

	void set_cellular_jitter(real_t jitter) { _jitter = jitter; }
	real_t get_cellular_jitter() const { return _jitter; }

	void set_gradient_perturbation_amplitude(real_t amp) { _amp = amp; }
	real_t get_gradient_perturbation_amplitude() const { return _amp; }

	void setup_noise(Ref<FastNoise> noise);

	FastnoiseNoiseParams();
	~FastnoiseNoiseParams();

protected:
	static void _bind_methods();

private:
	int _seed;
	FastNoise::Type _noise_type;
	FastNoise::Interpolation _interp;
	real_t _freq;
	unsigned int _octave_count;
	real_t _lacunarity;
	real_t _gain;
	FastNoise::FractalType _type;
	FastNoise::CellularDistanceFunction _func;
	FastNoise::CellularReturnType _ret;
	real_t _jitter;
	real_t _amp;
};

#endif
