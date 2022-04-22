#ifndef TILING_WAVE_FORM_COLLAPSE_H
#define TILING_WAVE_FORM_COLLAPSE_H

#include "array_2d.h"
#include "core/vector.h"

#include "wave_form_collapse.h"

struct Tile {

	struct ActionMap {
		Vector<int> map[8];

		void set_size(int size) {
			for (int i = 0; i < 8; ++i) {
				map[i].resize(size);
			}
		}
	};

	static const uint8_t rotation_map[6][9];
	static const uint8_t reflection_map[6][9];

	Vector<Array2D<int>> data;
	WaveFormCollapse::Symmetry symmetry;
	double weight;

	static ActionMap generate_action_map(const WaveFormCollapse::Symmetry &symmetry);

	static Vector<Array2D<int>> generate_oriented(Array2D<int> data, WaveFormCollapse::Symmetry symmetry);

	Tile(const Vector<Array2D<int>> &p_data, WaveFormCollapse::Symmetry p_symmetry, double p_weight);
	Tile(const Array2D<int> &p_data, WaveFormCollapse::Symmetry p_symmetry, double p_weight);
};

class TilingWaveFormCollapse : public WaveFormCollapse {
	GDCLASS(TilingWaveFormCollapse, WaveFormCollapse);

public:
	struct NeighbourData {
		int data[4];

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
		int id;
		int oriented_tile;

		IdToTilePair() {
			id = 0;
			oriented_tile = 0;
		}

		IdToTilePair(int p_id, int p_oriented_tile) {
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

	void set_tile(int tile_id, int i, int j);
	bool set_tile(int tile_id, int orientation, int i, int j);

	Array2D<int> run();

	Array2D<int> id_to_tiling(Array2D<int> ids);

	void initialize();

	TilingWaveFormCollapse();
	~TilingWaveFormCollapse();

protected:
	static void _bind_methods();

private:
	void generate_propagator_add_helper(Tile::ActionMap *action_map1, Tile::ActionMap *action_map2,
			Vector<DensePropagatorHelper> *dense_propagator,
			const NeighbourData &neighbour,
			int action, int direction);

	Vector<Tile> tiles;
	Vector<NeighbourData> neighbors;

	Vector<IdToTilePair> id_to_oriented_tile;
	Vector<Vector<int>> oriented_tile_ids;
};

#endif // FAST_WFC_TILING_WFC_HPP_
