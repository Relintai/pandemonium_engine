#ifndef TILING_WAVE_FORM_COLLAPSE_H
#define TILING_WAVE_FORM_COLLAPSE_H

/*************************************************************************/
/*  tiling_wave_form_collapse.h                                          */
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

#include "array_2d.h"
#include "core/containers/vector.h"

#include "wave_form_collapse.h"

struct Tile {
	struct ActionMap {
		Vector<int> map[8];

		void set_size(int size) {
			for (int i = 0; i < 8; ++i) {
				map[i].resize(size);
			}
		}

		void zero() {
			for (int i = 0; i < 8; ++i) {
				map[i].fill(0);
			}
		}
	};

	static const uint8_t ROTATION_MAP[6][9];
	static const uint8_t REFLECTION_MAP[6][9];

	String name;
	Vector<Array2D<int>> data;
	WaveFormCollapse::Symmetry symmetry;
	double weight;

	static ActionMap generate_action_map(const WaveFormCollapse::Symmetry symmetry);
	static Vector<Array2D<int>> generate_oriented(Array2D<int> data, WaveFormCollapse::Symmetry symmetry);

	void set_generate_data(const PoolIntArray &p_data, const int width, const int height);

	PoolIntArray data_get(const int index);
	void data_set(const int index, const PoolIntArray &p_data, const int width, const int height);
	void data_remove(const int index);

	Tile();
	Tile(WaveFormCollapse::Symmetry p_symmetry, double p_weight);
	Tile(const Vector<Array2D<int>> &p_data, WaveFormCollapse::Symmetry p_symmetry, double p_weight);
	Tile(const Array2D<int> &p_data, WaveFormCollapse::Symmetry p_symmetry, double p_weight);
	Tile(const PoolIntArray &p_data, const int width, const int height, WaveFormCollapse::Symmetry p_symmetry, double p_weight);
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

		NeighbourData(const int n1, const int n2, const int n3, const int n4) {
			data[0] = n1;
			data[1] = n2;
			data[2] = n3;
			data[3] = n4;
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
	int tile_add_generated(const PoolIntArray &data, const int width, const int height, const WaveFormCollapse::Symmetry symmetry, const float weight);
	int tile_add(const WaveFormCollapse::Symmetry symmetry, const float weight);
	int tile_create();
	void tile_remove(const int tile_index);
	int tile_count_get();

	void tile_data_add(const int tile_index, const PoolIntArray &data, const int width, const int height);
	void tile_data_generated_add(const int tile_index, const PoolIntArray &data, const int width, const int height);
	PoolIntArray tile_data_get(const int tile_index, const int data_index);
	void tile_data_set(const int tile_index, const int data_index, const PoolIntArray &data, const int width, const int height);
	void tile_data_remove(const int tile_index, const int data_index);
	void tile_data_clear(const int tile_index);
	int tile_data_count_get(const int tile_index);
	int tile_data_required_count_get(const int tile_index);

	int tile_width_get(const int tile_index, const int data_index);
	int tile_height_get(const int tile_index, const int data_index);

	WaveFormCollapse::Symmetry tile_symmetry_get(const int tile_index);
	void tile_symmetry_set(const int tile_index, const WaveFormCollapse::Symmetry val);

	float tile_weight_get(const int tile_index);
	void tile_weight_set(const int tile_index, const float val);

	String tile_name_get(const int tile_index);
	void tile_name_set(const int tile_index, const String &val);
	int tile_index_get(const String &tile_name);

	int neighbour_data_add(const int left, const int left_orientation, const int right, const int right_orientation);
	int neighbour_data_add_str(const String &left, const int left_orientation, const String &right, const int right_orientation);
	PoolIntArray neighbour_data_get(const int index);
	void neighbour_data_remove(const int index);
	void neighbour_data_set(const int index, const int left, const int left_orientation, const int right, const int right_orientation);
	void neighbour_data_set_str(const int index, const String &left, const int left_orientation, const String &right, const int right_orientation);
	bool neighbour_data_validate(const int tile_index, const int orientation);
	bool neighbour_data_validate_str(const String &tile_name, const int orientation);

	void set_tiles(const Vector<Tile> &p_tiles);
	void set_neighbours(const Vector<NeighbourData> &p_neighbors);

	void generate_oriented_tile_ids();
	void generate_propagator();

	static Vector<double> get_tiles_weights(const Vector<Tile> &tiles);

	void set_tile(int tile_id, int i, int j);
	bool set_tile(int tile_id, int orientation, int i, int j);

	Array2D<int> run();

	Array2D<int> id_to_tiling(Array2D<int> ids);

	bool validate();
	void initialize();

	TilingWaveFormCollapse();
	~TilingWaveFormCollapse();

protected:
	static void _bind_methods();

private:
	void generate_propagator_add_helper(const Tile::ActionMap &action_map1, const Tile::ActionMap &action_map2,
			Vector<DensePropagatorHelper> *dense_propagator,
			const NeighbourData &neighbour,
			int action, int direction);

	Vector<Tile> _tiles;
	Vector<NeighbourData> _neighbors;

	Vector<IdToTilePair> _id_to_oriented_tile;
	Vector<Vector<int>> _oriented_tile_ids;
};

#endif
