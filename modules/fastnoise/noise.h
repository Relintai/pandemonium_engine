#ifndef FASTNOISE_NOISE_H
#define FASTNOISE_NOISE_H

#include "core/object/reference.h"
#include "core/ustring.h"

#include "lib/FastNoise.h"

#include "core/containers/pool_vector.h"

typedef fastnoise::FastNoise _FastNoise;

class FastNoise : public Reference {
	GDCLASS(FastNoise, Reference)

public:
	static const String BINDING_STRING_TYPE;
	static const String BINDING_STRING_INTERPOLATION;
	static const String BINDING_STRING_FRACTAL_TYPE;
	static const String BINDING_STRING_CELLULAR_DISTANCE_FUNCTION;
	static const String BINDING_STRING_CELLULAR_RETURN_TYPE;

	// Enums Pandemonium-style (same values)

	enum Type {
		TYPE_VALUE = _FastNoise::Value,
		TYPE_VALUE_FRACTAL = _FastNoise::ValueFractal,
		TYPE_PERLIN = _FastNoise::Perlin,
		TYPE_PERLIN_FRACTAL = _FastNoise::PerlinFractal,
		TYPE_SIMPLEX = _FastNoise::Simplex,
		TYPE_SIMPLEX_FRACTAL = _FastNoise::SimplexFractal,
		TYPE_CELLULAR = _FastNoise::Cellular,
		TYPE_WHITE_NOISE = _FastNoise::WhiteNoise,
		TYPE_CUBIC = _FastNoise::Cubic,
		TYPE_CUBIC_FRACTAL = _FastNoise::CubicFractal
	};

	enum Interpolation {
		INTERP_LINEAR = _FastNoise::Linear,
		INTERP_QUINTIC = _FastNoise::Quintic,
		INTERP_HERMITE = _FastNoise::Hermite
	};

	enum FractalType {
		FRACTAL_FBM = _FastNoise::FBM,
		FRACTAL_BILLOW = _FastNoise::Billow,
		FRACTAL_RIGID_MULTI = _FastNoise::RigidMulti
	};

	enum CellularDistanceFunction {
		DISTANCE_EUCLIDEAN = _FastNoise::Euclidean,
		DISTANCE_MANHATTAN = _FastNoise::Manhattan,
		DISTANCE_NATURAL = _FastNoise::Natural
	};

	enum CellularReturnType {
		RETURN_CELL_VALUE = _FastNoise::CellValue,
		RETURN_NOISE_LOOKUP = _FastNoise::NoiseLookup,
		RETURN_DISTANCE = _FastNoise::Distance,
		RETURN_DISTANCE_2 = _FastNoise::Distance2,
		RETURN_DISTANCE_2_ADD = _FastNoise::Distance2Add,
		RETURN_DISTANCE_2_SUB = _FastNoise::Distance2Sub,
		RETURN_DISTANCE_2_MUL = _FastNoise::Distance2Mul,
		RETURN_DISTANCE_2_DIV = _FastNoise::Distance2Div
	};

	FastNoise();

	// Methods (Pandemonium-style mappings to FastNoise)

	int get_seed() const { return _noise.GetSeed(); }
	void set_seed(int seed) { _noise.SetSeed(seed); }

	void set_noise_type(Type noise_type) { _noise.SetNoiseType((_FastNoise::NoiseType)noise_type); }
	Type get_noise_type() const { return (Type)_noise.GetNoiseType(); }

	void set_interpolation(Interpolation interp) { _noise.SetInterp((_FastNoise::Interp)interp); }
	Interpolation get_interpolation() const { return (Interpolation)_noise.GetInterp(); }

	void set_frequency(real_t freq) { _noise.SetFrequency(freq); }
	real_t get_frequency() const { return _noise.GetFrequency(); }

	void set_fractal_octaves(unsigned int octave_count) { _noise.SetFractalOctaves(octave_count); }
	int get_fractal_octaves() const { return _noise.GetFractalOctaves(); }

	void set_fractal_lacunarity(real_t lacunarity) { _noise.SetFractalLacunarity(lacunarity); }
	real_t get_fractal_lacunarity() const { return _noise.GetFractalLacunarity(); }

	void set_fractal_gain(real_t gain) { _noise.SetFractalGain(gain); }
	real_t get_fractal_gain() const { return _noise.GetFractalGain(); }

	void set_fractal_type(FractalType type) { _noise.SetFractalType((_FastNoise::FractalType)type); }
	FractalType get_fractal_type() const { return (FractalType)_noise.GetFractalType(); }

	void set_cellular_distance_function(CellularDistanceFunction func) { _noise.SetCellularDistanceFunction((_FastNoise::CellularDistanceFunction)func); }
	CellularDistanceFunction get_cellular_distance_function() const { return (CellularDistanceFunction)_noise.GetCellularDistanceFunction(); }

	void set_cellular_return_type(CellularReturnType ret) { _noise.SetCellularReturnType((_FastNoise::CellularReturnType)ret); }
	CellularReturnType get_cellular_return_type() const { return (CellularReturnType)_noise.GetCellularReturnType(); }

	void set_cellular_noise_lookup(Ref<FastNoise> other_noise);
	Ref<FastNoise> get_cellular_noise_lookup() const { return _cellular_lookup_ref; }

	void set_cellular_distance_2_indices(int index0, int index1);
	PoolIntArray get_cellular_distance_2_indices() const;

	void set_cellular_jitter(real_t jitter) { _noise.SetCellularJitter(jitter); }
	real_t get_cellular_jitter() const { return _noise.GetCellularJitter(); }

	void set_gradient_perturbation_amplitude(real_t amp) { _noise.SetGradientPerturbAmp(amp); }
	real_t get_gradient_perturbation_amplitude() const { return _noise.GetGradientPerturbAmp(); }

	// 2D

	float get_noise_2d(float x, float y) { return _noise.GetNoise(x, y); }

	//float get_gradient_2d(float x, float y) { return _noise.GetGradient(x, y); }
	//float get_simplex_2d(float x, float y) { return _noise.GetSimplex(x, y); }
	//float get_cellular_2d(float x, float y) { return _noise.GetCellular(x, y); }
	//float get_cellular_hq_2d(float x, float y) { return _noise.GetCellularHQ(x, y); }
	//float get_white_noise_2d(float x, float y) { return _noise.GetWhiteNoise(x, y); }
	//float get_value_2d(float x, float y) { return _noise.GetValue(x, y); }

	// 3D

	float get_noise_3d(float x, float y, float z) { return _noise.GetNoise(x, y, z); }

	//float get_gradient_3d(float x, float y, float z) { return _noise.GetGradient(x, y, z); }
	//float get_simplex_3d(float x, float y, float z) { return _noise.GetSimplex(x, y, z); }
	//float get_cellular_3d(float x, float y, float z) { return _noise.GetCellular(x, y, z); }
	//float get_cellular_hq_3d(float x, float y, float z) { return _noise.GetCellularHQ(x, y, z); }
	//float get_white_noise_3d(float x, float y, float z) { return _noise.GetWhiteNoise(x, y, z); }
	//float get_value_2d(float x, float y, float z) { return _noise.GetValue(x, y, z); }

	// 4D

	float get_simplex_4d(float x, float y, float z, float w) { return _noise.GetSimplex(x, y, z, w); }
	float get_white_noise_4d(float x, float y, float z, float w) { return _noise.GetWhiteNoise(x, y, z, w); }

	// Convenience

	float get_noise_2dv(Vector2 pos) { return _noise.GetNoise(pos.x, pos.y); }
	float get_noise_3dv(Vector3 pos) { return _noise.GetNoise(pos.x, pos.y, pos.z); }

protected:
	static void _bind_methods();

private:
	_FastNoise _noise;
	Ref<FastNoise> _cellular_lookup_ref;
};

// Make Variant happy with custom enums
VARIANT_ENUM_CAST(FastNoise::Type)
VARIANT_ENUM_CAST(FastNoise::FractalType)
VARIANT_ENUM_CAST(FastNoise::Interpolation)
VARIANT_ENUM_CAST(FastNoise::CellularDistanceFunction)
VARIANT_ENUM_CAST(FastNoise::CellularReturnType)

#endif // FASTNOISE_NOISE_H
