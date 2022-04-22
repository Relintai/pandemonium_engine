
#include "tiling_wave_form_collapse.h"

// 0th index is the count
// Generate the map associating an orientation id to the orientation
// id obtained when rotating 90° anticlockwise the tile.
const uint8_t Tile::rotation_map[6][9] = {
	{ 1, 0 }, // SYMMETRY_X
	{ 4, 1, 2, 3, 0 }, // SYMMETRY_T
	{ 2, 1, 0 }, // SYMMETRY_I
	{ 4, 1, 2, 3, 0 }, // SYMMETRY_L
	{ 2, 1, 0 }, // SYMMETRY_BACKSLASH
	{ 8, 1, 2, 3, 0, 5, 6, 7, 4 }, // SYMMETRY_P
};

// 0th index is the count
// Generate the map associating an orientation id to the orientation
// id obtained when rotating 90° anticlockwise the tile.
const uint8_t Tile::reflection_map[6][9] = {
	{ 1, 0 }, // SYMMETRY_X
	{ 4, 0, 3, 2, 1 }, // SYMMETRY_T
	{ 2, 0, 1 }, // SYMMETRY_I
	{ 4, 1, 0, 3, 2 }, // SYMMETRY_L
	{ 2, 1, 0 }, // SYMMETRY_BACKSLASH
	{ 8, 4, 7, 6, 5, 0, 3, 2, 1 }, // SYMMETRY_P
};

// Generate the map associating an orientation id and an action to the
// resulting orientation id.
// Actions 0, 1, 2, and 3 are 0°, 90°, 180°, and 270° anticlockwise rotations.
// Actions 4, 5, 6, and 7 are actions 0, 1, 2, and 3 preceded by a reflection
// on the x axis.
Tile::ActionMap Tile::generate_action_map(const Symmetry &symmetry) {
	int sindx = static_cast<int>(symmetry);
	int size = rotation_map[sindx][0];

	ActionMap action_map;

	for (int i = 0; i < size; ++i) {
		action_map.map[0].write[i] = i;
	}

	for (int a = 1; a < 4; ++a) {
		for (int i = 0; i < size; ++i) {
			action_map.map[a].write[i] = rotation_map[sindx][action_map.map[a - 1][i] + 1];
		}
	}

	for (int i = 0; i < size; ++i) {
		action_map.map[4].write[i] = reflection_map[sindx][action_map.map[0][i] + 1];
	}

	for (int a = 5; a < 8; ++a) {
		for (int i = 0; i < size; ++i) {
			action_map.map[a].write[i] = rotation_map[sindx][action_map.map[a - 1][i] + 1];
		}
	}

	return action_map;
}

// Generate all distincts rotations of a 2D array given its symmetries;
Vector<Array2D<uint32_t>> Tile::generate_oriented(Array2D<uint32_t> data, Symmetry symmetry) {
	Vector<Array2D<uint32_t>> oriented;
	oriented.push_back(data);

	switch (symmetry) {
		case SYMMETRY_I:
		case SYMMETRY_BACKSLASH:
			oriented.push_back(data.rotated());
			break;
		case SYMMETRY_T:
		case SYMMETRY_L:
			oriented.push_back(data = data.rotated());
			oriented.push_back(data = data.rotated());
			oriented.push_back(data = data.rotated());
			break;
		case SYMMETRY_P:
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

// Create a tile with its differents orientations, its symmetries and its weight on the distribution of tiles.
Tile::Tile(const Vector<Array2D<uint32_t>> &p_data, Symmetry p_symmetry, double p_weight) {
	data = p_data;
	symmetry = p_symmetry;
	weight = p_weight;
}

// Create a tile with its base orientation, its symmetries and its weight on the distribution of tiles.
// The other orientations are generated with its first one.
Tile::Tile(const Array2D<uint32_t> &p_data, Symmetry p_symmetry, double p_weight) {
	data = generate_oriented(p_data, p_symmetry);
	symmetry = p_symmetry;
	weight = p_weight;
}

// Returns false if the given tile and orientation does not exist, or if the coordinates are not in the wave
bool TilingWaveFormCollapse::set_tile(uint32_t tile_id, uint32_t orientation, uint32_t i, uint32_t j) {
	if (tile_id >= oriented_tile_ids.size() || orientation >= oriented_tile_ids[tile_id].size() || i >= get_height() || j >= get_width()) {
		return false;
	}

	uint32_t oriented_tile_id = oriented_tile_ids[tile_id][orientation];
	set_tile(oriented_tile_id, i, j);

	return true;
}

void TilingWaveFormCollapse::set_tiles(const Vector<Tile> &p_tiles) {
	tiles = p_tiles;
}

void TilingWaveFormCollapse::set_neighbours(const Vector<NeighbourData> &p_neighbors) {
	neighbors = p_neighbors;
}

// Generate mapping from id to oriented tiles and vice versa.
void TilingWaveFormCollapse::generate_oriented_tile_ids() {
	id_to_oriented_tile.clear();
	oriented_tile_ids.clear();

	uint32_t id = 0;
	for (int i = 0; i < tiles.size(); i++) {
		oriented_tile_ids.push_back({});
		for (int j = 0; j < tiles[i].data.size(); j++) {
			id_to_oriented_tile.push_back(IdToTilePair(i, j));
			oriented_tile_ids.write[i].push_back(id);
			id++;
		}
	}
}

// Generate the propagator which will be used in the wfc algorithm.
void TilingWaveFormCollapse::generate_propagator() {
	int nb_oriented_tiles = id_to_oriented_tile.size();

	Vector<DensePropagatorHelper> dense_propagator;
	dense_propagator.resize(nb_oriented_tiles);
	for (int i = 0; i < nb_oriented_tiles; ++i) {
		dense_propagator.write[i].resize(nb_oriented_tiles);
	}

	int size = neighbors.size();
	for (int i = 0; i < size; ++i) {
		const NeighbourData &neighbour = neighbors[i];

		uint32_t tile1 = neighbour.data[0];
		uint32_t tile2 = neighbour.data[2];
		Tile::ActionMap action_map1 = Tile::generate_action_map(tiles[tile1].symmetry);
		Tile::ActionMap action_map2 = Tile::generate_action_map(tiles[tile2].symmetry);

		generate_propagator_add_helper(&action_map1, &action_map2, &dense_propagator, neighbour, 0, 2);
		generate_propagator_add_helper(&action_map1, &action_map2, &dense_propagator, neighbour, 1, 0);
		generate_propagator_add_helper(&action_map1, &action_map2, &dense_propagator, neighbour, 2, 1);
		generate_propagator_add_helper(&action_map1, &action_map2, &dense_propagator, neighbour, 3, 3);
		generate_propagator_add_helper(&action_map1, &action_map2, &dense_propagator, neighbour, 4, 1);
		generate_propagator_add_helper(&action_map1, &action_map2, &dense_propagator, neighbour, 5, 3);
		generate_propagator_add_helper(&action_map1, &action_map2, &dense_propagator, neighbour, 6, 2);
		generate_propagator_add_helper(&action_map1, &action_map2, &dense_propagator, neighbour, 7, 0);
	}

	Vector<PropagatorStateEntry> propagator;
	propagator.resize(nb_oriented_tiles);

	PropagatorStateEntry *propw = propagator.ptrw();

	for (size_t i = 0; i < nb_oriented_tiles; ++i) {
		for (size_t j = 0; j < nb_oriented_tiles; ++j) {
			for (size_t d = 0; d < 4; ++d) {
				if (propw[i].directions[d][j]) {
					propw[i].directions[d].push_back(j);
				}
			}
		}
	}

	set_propagator_state(propagator);
}

// Get probability of presence of tiles.
Vector<double> TilingWaveFormCollapse::get_tiles_weights(const Vector<Tile> &tiles) {
	Vector<double> frequencies;

	for (int i = 0; i < tiles.size(); ++i) {
		for (int j = 0; j < tiles[i].data.size(); ++j) {
			frequencies.push_back(tiles[i].weight / tiles[i].data.size());
		}
	}

	return frequencies;
}

void TilingWaveFormCollapse::set_tile(uint32_t tile_id, uint32_t i, uint32_t j) {
	for (int p = 0; p < id_to_oriented_tile.size(); p++) {
		if (tile_id != p) {
			remove_wave_pattern(i, j, p);
		}
	}
}

Array2D<uint32_t> TilingWaveFormCollapse::do_run() {
	Array2D<uint32_t> a = run();

	if (a.width == 0 && a.height == 0) {
		return Array2D<uint32_t>(0, 0);
	}

	return id_to_tiling(a);
}

// Translate the generic WFC result into the image result
Array2D<uint32_t> TilingWaveFormCollapse::id_to_tiling(Array2D<uint32_t> ids) {
	uint32_t size = tiles[0].data[0].height;
	Array2D<uint32_t> tiling(size * ids.height, size * ids.width);

	for (uint32_t i = 0; i < ids.height; i++) {
		for (uint32_t j = 0; j < ids.width; j++) {
			IdToTilePair oriented_tile = id_to_oriented_tile[ids.get(i, j)];

			for (uint32_t y = 0; y < size; y++) {
				for (uint32_t x = 0; x < size; x++) {
					tiling.get(i * size + y, j * size + x) = tiles[oriented_tile.id].data[oriented_tile.oriented_tile].get(y, x);
				}
			}
		}
	}

	return tiling;
}

void TilingWaveFormCollapse::initialize() {
	generate_oriented_tile_ids();

	WaveFormCollapse::initialize();
}

TilingWaveFormCollapse::TilingWaveFormCollapse() {
}
TilingWaveFormCollapse::~TilingWaveFormCollapse() {
}

void TilingWaveFormCollapse::_bind_methods() {
}

void TilingWaveFormCollapse::generate_propagator_add_helper(Tile::ActionMap *action_map1, Tile::ActionMap *action_map2,
		Vector<DensePropagatorHelper> *dense_propagator,
		const NeighbourData &neighbour, uint32_t action, uint32_t direction) {
	// --
	uint32_t tile1 = neighbour.data[0];
	uint32_t orientation1 = neighbour.data[1];
	uint32_t tile2 = neighbour.data[2];
	uint32_t orientation2 = neighbour.data[3];

	uint32_t temp_orientation1 = action_map1->map[action][orientation1];
	uint32_t temp_orientation2 = action_map2->map[action][orientation2];
	uint32_t oriented_tile_id1 = oriented_tile_ids[tile1][temp_orientation1];
	uint32_t oriented_tile_id2 = oriented_tile_ids[tile2][temp_orientation2];
	dense_propagator->write[oriented_tile_id1].directions[direction].write[oriented_tile_id2] = true;
	direction = get_opposite_direction(direction);
	dense_propagator->write[oriented_tile_id2].directions[direction].write[oriented_tile_id1] = true;
}