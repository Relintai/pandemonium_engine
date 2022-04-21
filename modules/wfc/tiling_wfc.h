#ifndef FAST_WFC_TILING_WFC_HPP_
#define FAST_WFC_TILING_WFC_HPP_

#include "core/vector.h"
#include <unordered_map>

#include "array_2d.h"
#include "wfc.h"

// The distinct symmetries of a tile.
// It represents how the tile behave when it is rotated or reflected
enum class Symmetry {
	X,
	T,
	I,
	L,
	backslash,
	P
};

/**
// Return the number of possible distinct orientations for a tile.
// An orientation is a combination of rotations and reflections.
 */
constexpr uint32_t nb_of_possible_orientations(const Symmetry &symmetry) {
	switch (symmetry) {
		case Symmetry::X:
			return 1;
		case Symmetry::I:
		case Symmetry::backslash:
			return 2;
		case Symmetry::T:
		case Symmetry::L:
			return 4;
		default:
			return 8;
	}
}

// A tile that can be placed on the board.
template <typename T>
struct Tile {
	Vector<Array2D<T>> data; // The different orientations of the tile
	Symmetry symmetry; // The symmetry of the tile
	double weight; // Its weight on the distribution of presence of tiles

	// Generate the map associating an orientation id to the orientation
	// id obtained when rotating 90° anticlockwise the tile.
	static Vector<uint32_t> generate_rotation_map(const Symmetry &symmetry) {
		switch (symmetry) {
			case Symmetry::X:
				return { 0 };
			case Symmetry::I:
			case Symmetry::backslash:
				return { 1, 0 };
			case Symmetry::T:
			case Symmetry::L:
				return { 1, 2, 3, 0 };
			case Symmetry::P:
			default:
				return { 1, 2, 3, 0, 5, 6, 7, 4 };
		}
	}

	// Generate the map associating an orientation id to the orientation
	// id obtained when reflecting the tile along the x axis.
	static Vector<uint32_t> generate_reflection_map(const Symmetry &symmetry) {
		switch (symmetry) {
			case Symmetry::X:
				return { 0 };
			case Symmetry::I:
				return { 0, 1 };
			case Symmetry::backslash:
				return { 1, 0 };
			case Symmetry::T:
				return { 0, 3, 2, 1 };
			case Symmetry::L:
				return { 1, 0, 3, 2 };
			case Symmetry::P:
			default:
				return { 4, 7, 6, 5, 0, 3, 2, 1 };
		}
	}

	// Generate the map associating an orientation id and an action to the
	// resulting orientation id.
	// Actions 0, 1, 2, and 3 are 0°, 90°, 180°, and 270° anticlockwise rotations.
	// Actions 4, 5, 6, and 7 are actions 0, 1, 2, and 3 preceded by a reflection
	// on the x axis.
	static Vector<Vector<uint32_t>> generate_action_map(const Symmetry &symmetry) {
		Vector<uint32_t> rotation_map = generate_rotation_map(symmetry);
		Vector<uint32_t> reflection_map = generate_reflection_map(symmetry);
		int size = rotation_map.size();
		Vector<Vector<uint32_t>> action_map(8, Vector<uint32_t>(size));

		for (int i = 0; i < size; ++i) {
			action_map[0][i] = i;
		}

		for (int a = 1; a < 4; ++a) {
			for (int i = 0; i < size; ++i) {
				action_map[a][i] = rotation_map[action_map[a - 1][i]];
			}
		}
		for (int i = 0; i < size; ++i) {
			action_map[4][i] = reflection_map[action_map[0][i]];
		}
		for (int a = 5; a < 8; ++a) {
			for (int i = 0; i < size; ++i) {
				action_map[a][i] = rotation_map[action_map[a - 1][i]];
			}
		}
		return action_map;
	}

	// Generate all distincts rotations of a 2D array given its symmetries;
	static Vector<Array2D<T>> generate_oriented(Array2D<T> data, Symmetry symmetry) {
		Vector<Array2D<T>> oriented;
		oriented.push_back(data);

		switch (symmetry) {
			case Symmetry::I:
			case Symmetry::backslash:
				oriented.push_back(data.rotated());
				break;
			case Symmetry::T:
			case Symmetry::L:
				oriented.push_back(data = data.rotated());
				oriented.push_back(data = data.rotated());
				oriented.push_back(data = data.rotated());
				break;
			case Symmetry::P:
				oriented.push_back(data = data.rotated());
				oriented.push_back(data = data.rotated());
				oriented.push_back(data = data.rotated());
				oriented.push_back(data = data.rotated().reflected());
				oriented.push_back(data = data.rotated());
				oriented.push_back(data = data.rotated());
				oriented.push_back(data = data.rotated());
				break;
			default:
				break;
		}

		return oriented;
	}

	// Create a tile with its differents orientations, its symmetries and its
	// weight on the distribution of tiles.
	Tile(Vector<Array2D<T>> data, Symmetry symmetry, double weight) :
			data(data), symmetry(symmetry), weight(weight) {}

	// Create a tile with its base orientation, its symmetries and its
	// weight on the distribution of tiles.
	// The other orientations are generated with its first one.
	Tile(Array2D<T> data, Symmetry symmetry, double weight) :
			data(generate_oriented(data, symmetry)), symmetry(symmetry), weight(weight) {}
};

struct TilingWFCOptions {
	bool periodic_output;
};

// Class generating a new image with the tiling WFC algorithm.
template <typename T>
class TilingWFC {
private:
	Vector<Tile<T>> tiles;
	Vector<std::pair<uint32_t, uint32_t>> id_to_oriented_tile;
	Vector<Vector<uint32_t>> oriented_tile_ids;

	TilingWFCOptions options;

	WFC wfc;

public:
	uint32_t height;
	uint32_t width;

private:
	// Generate mapping from id to oriented tiles and vice versa.
	static std::pair<Vector<std::pair<uint32_t, uint32_t>>, Vector<Vector<uint32_t>>> generate_oriented_tile_ids(const Vector<Tile<T>> &tiles) {
		Vector<std::pair<uint32_t, uint32_t>> id_to_oriented_tile;
		Vector<Vector<uint32_t>> oriented_tile_ids;

		uint32_t id = 0;
		for (int i = 0; i < tiles.size(); i++) {
			oriented_tile_ids.push_back({});
			for (int j = 0; j < tiles[i].data.size(); j++) {
				id_to_oriented_tile.push_back({ i, j });
				oriented_tile_ids[i].push_back(id);
				id++;
			}
		}

		return { id_to_oriented_tile, oriented_tile_ids };
	}

	struct DensePropagatorHelper {
		Vector<bool> directions[4];

		void resize(const int size) {
			for (int i = 0; i < 4; ++i) {
				directions[i].resize(size);
				directions[i].fill(false);
			}
		}
	};

	// Generate the propagator which will be used in the wfc algorithm.
	static Vector<PropagatorStateEntry> generate_propagator(
			const Vector<NeighbourData> &neighbors,
			Vector<Tile<T>> tiles,
			Vector<std::pair<uint32_t, uint32_t>> id_to_oriented_tile,
			Vector<Vector<uint32_t>> oriented_tile_ids) {

		size_t nb_oriented_tiles = id_to_oriented_tile.size();

		Vector<DensePropagatorHelper> dense_propagator;
		dense_propagator.resize(nb_oriented_tiles);
		for (int i = 0; i < nb_oriented_tiles; ++i) {
			dense_propagator.write[i].resize(nb_oriented_tiles);
		}

		for (auto neighbor : neighbors) {
			uint32_t tile1 = std::get<0>(neighbor);
			uint32_t orientation1 = std::get<1>(neighbor);
			uint32_t tile2 = std::get<2>(neighbor);
			uint32_t orientation2 = std::get<3>(neighbor);
			Vector<Vector<uint32_t>> action_map1 = Tile<T>::generate_action_map(tiles[tile1].symmetry);
			Vector<Vector<uint32_t>> action_map2 = Tile<T>::generate_action_map(tiles[tile2].symmetry);

			auto add = [&](uint32_t action, uint32_t direction) {
				uint32_t temp_orientation1 = action_map1[action][orientation1];
				uint32_t temp_orientation2 = action_map2[action][orientation2];
				uint32_t oriented_tile_id1 = oriented_tile_ids[tile1][temp_orientation1];
				uint32_t oriented_tile_id2 = oriented_tile_ids[tile2][temp_orientation2];
				dense_propagator[oriented_tile_id1][direction][oriented_tile_id2] = true;
				direction = get_opposite_direction(direction);
				dense_propagator[oriented_tile_id2][direction][oriented_tile_id1] = true;
			};

			add(0, 2);
			add(1, 0);
			add(2, 1);
			add(3, 3);
			add(4, 1);
			add(5, 3);
			add(6, 2);
			add(7, 0);
		}

		Vector<PropagatorStateEntry> propagator(nb_oriented_tiles);

		for (size_t i = 0; i < nb_oriented_tiles; ++i) {
			for (size_t j = 0; j < nb_oriented_tiles; ++j) {
				for (size_t d = 0; d < 4; ++d) {
					if (dense_propagator[i][d][j]) {
						propagator[i][d].push_back(j);
					}
				}
			}
		}

		return propagator;
	}

	// Get probability of presence of tiles.
	static Vector<double> get_tiles_weights(const Vector<Tile<T>> &tiles) {
		Vector<double> frequencies;

		for (int i = 0; i < tiles.size(); ++i) {
			for (int j = 0; j < tiles[i].data.size(); ++j) {
				frequencies.push_back(tiles[i].weight / tiles[i].data.size());
			}
		}

		return frequencies;
	}

	// Translate the generic WFC result into the image result
	Array2D<T> id_to_tiling(Array2D<uint32_t> ids) {
		uint32_t size = tiles[0].data[0].height;
		Array2D<T> tiling(size * ids.height, size * ids.width);

		for (uint32_t i = 0; i < ids.height; i++) {
			for (uint32_t j = 0; j < ids.width; j++) {
				std::pair<uint32_t, uint32_t> oriented_tile = id_to_oriented_tile[ids.get(i, j)];

				for (uint32_t y = 0; y < size; y++) {
					for (uint32_t x = 0; x < size; x++) {
						tiling.get(i * size + y, j * size + x) = tiles[oriented_tile.first].data[oriented_tile.second].get(y, x);
					}
				}
			}
		}

		return tiling;
	}

	void set_tile(uint32_t tile_id, uint32_t i, uint32_t j) {
		for (int p = 0; p < id_to_oriented_tile.size(); p++) {
			if (tile_id != p) {
				wfc.remove_wave_pattern(i, j, p);
			}
		}
	}

public:
	struct NeighbourData {
		uint32_t data[4];

		NeighbourData() {
			for (int i = 0; i < 4; ++i) {
				direction[i] = 0;
			}
		}
	};

	// Construct the TilingWFC class to generate a tiled image.
	TilingWFC(
			const Vector<Tile<T>> &tiles,
			const Vector<NeighbourData> &neighbors,
			const uint32_t height, const uint32_t width,
			const TilingWFCOptions &options, int seed) :
			tiles(tiles),
			id_to_oriented_tile(generate_oriented_tile_ids(tiles).first),
			oriented_tile_ids(generate_oriented_tile_ids(tiles).second),
			options(options),
			wfc(options.periodic_output, seed, get_tiles_weights(tiles),
					generate_propagator(neighbors, tiles, id_to_oriented_tile,
							oriented_tile_ids),
					height, width),
			height(height),
			width(width) {}

	// Set the tile at a specific position.
	// Returns false if the given tile and orientation does not exist,
	// or if the coordinates are not in the wave
	bool set_tile(uint32_t tile_id, uint32_t orientation, uint32_t i, uint32_t j) {
		if (tile_id >= oriented_tile_ids.size() || orientation >= oriented_tile_ids[tile_id].size() || i >= height || j >= width) {
			return false;
		}

		uint32_t oriented_tile_id = oriented_tile_ids[tile_id][orientation];
		set_tile(oriented_tile_id, i, j);
		
		return true;
	}

	// Run the tiling wfc and return the result if the algorithm succeeded
	Array2D<T> run() {
		Array2D<uint32_t> a = wfc.run();

		if (a.width == 0 && a.height == 0) {
			return Array2D<T>(0, 0);
		}

		return id_to_tiling(a);
	}
};

#endif // FAST_WFC_TILING_WFC_HPP_
