#ifndef TILING_WAVE_FORM_COLLAPSE_H
#define TILING_WAVE_FORM_COLLAPSE_H

#include "array_2d.h"
#include "core/vector.h"

#include "wave_form_collapse.h"

struct Tile {
	enum Symmetry {
		SYMMETRY_X = 0,
		SYMMETRY_T,
		SYMMETRY_I,
		SYMMETRY_L,
		SYMMETRY_BACKSLASH,
		SYMMETRY_P
	};

	struct ActionMap {
		Vector<uint32_t> map[8];

		void set_size(int size) {
			for (int i = 0; i < 8; ++i) {
				map[i].resize(size);
			}
		}
	};

	static const uint8_t rotation_map[6][9];
	static const uint8_t reflection_map[6][9];

	Vector<Array2D<uint32_t>> data;
	Symmetry symmetry;
	double weight;

	static ActionMap generate_action_map(const Symmetry &symmetry);

	static Vector<Array2D<uint32_t>> generate_oriented(Array2D<uint32_t> data, Symmetry symmetry);

	Tile(const Vector<Array2D<uint32_t>> &p_data, Symmetry p_symmetry, double p_weight);
	Tile(const Array2D<uint32_t> &p_data, Symmetry p_symmetry, double p_weight);
};

class TilingWaveFormCollapse : WaveFormCollapse {
	GDCLASS(TilingWaveFormCollapse, WaveFormCollapse);

public:
	struct NeighbourData {
		uint32_t data[4];

		NeighbourData() {
			for (int i = 0; i < 4; ++i) {
				data[i] = 0;
			}
		}
	};

	struct DensePropagatorHelper {
		Vector<bool> directions[4];

		void resize(const int size) {
			for (int i = 0; i < 4; ++i) {
				directions[i].resize(size);
				directions[i].fill(false);
			}
		}
	};

	struct IdToTilePair {
		uint32_t id;
		uint32_t oriented_tile;

		IdToTilePair() {
			id = 0;
			oriented_tile = 0;
		}

		IdToTilePair(uint32_t p_id, uint32_t p_oriented_tile) {
			id = p_id;
			oriented_tile = p_oriented_tile;
		}
	};

public:
	void set_tiles(const Vector<Tile> &p_tiles);
	void set_neighbours(const Vector<NeighbourData> &p_neighbors);

	void generate_oriented_tile_ids();

	void generate_propagator();

	static Vector<double> get_tiles_weights(const Vector<Tile> &tiles);

	void set_tile(uint32_t tile_id, uint32_t i, uint32_t j);
	bool set_tile(uint32_t tile_id, uint32_t orientation, uint32_t i, uint32_t j);

	Array2D<uint32_t> do_run();

	Array2D<uint32_t> id_to_tiling(Array2D<uint32_t> ids);

	void initialize();

	TilingWaveFormCollapse();
	~TilingWaveFormCollapse();

protected:
	static void _bind_methods();

private:
	void generate_propagator_add_helper(Tile::ActionMap *action_map1, Tile::ActionMap *action_map2,
			Vector<DensePropagatorHelper> *dense_propagator,
			const NeighbourData &neighbour,
			uint32_t action, uint32_t direction);

	Vector<Tile> tiles;
	Vector<NeighbourData> neighbors;

	Vector<IdToTilePair> id_to_oriented_tile;
	Vector<Vector<uint32_t>> oriented_tile_ids;
};

#endif // FAST_WFC_TILING_WFC_HPP_
