
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
Tile::ActionMap Tile::generate_action_map(const WaveFormCollapse::Symmetry symmetry) {
	int sindx = static_cast<int>(symmetry);
	int size = rotation_map[sindx][0];

	ActionMap action_map;
	action_map.set_size(size);

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
Vector<Array2D<int>> Tile::generate_oriented(Array2D<int> data, WaveFormCollapse::Symmetry symmetry) {
	Vector<Array2D<int>> oriented;
	oriented.push_back(data);

	switch (symmetry) {
		case WaveFormCollapse::SYMMETRY_I:
		case WaveFormCollapse::SYMMETRY_BACKSLASH:
			oriented.push_back(data.rotated());
			break;
		case WaveFormCollapse::SYMMETRY_T:
		case WaveFormCollapse::SYMMETRY_L:
			oriented.push_back(data = data.rotated());
			oriented.push_back(data = data.rotated());
			oriented.push_back(data = data.rotated());
			break;
		case WaveFormCollapse::SYMMETRY_P:
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

void Tile::set_generate_data(const PoolIntArray &p_data, const int width, const int height) {
	data.clear();
	Array2D<int> d(p_data, height, width);
	data = generate_oriented(d, symmetry);
}

PoolIntArray Tile::data_get(const int index) {
	ERR_FAIL_INDEX_V(index, data.size(), PoolIntArray());

	const Array2D<int> arr = data.write[index];
	PoolIntArray ret;

	const int *p = arr.data.ptr();
	int s = arr.data.size();

	ret.resize(s);
	PoolIntArray::Write w = ret.write();

	for (int i = 0; i < s; ++i) {
		w[i] = p[i];
	}

	w.release();

	return ret;
}

void Tile::data_set(const int index, const PoolIntArray &p_data, const int width, const int height) {
	ERR_FAIL_INDEX(index, data.size());

	data.write[index].set_data(p_data, height, width);
}

void Tile::data_remove(const int index) {
	ERR_FAIL_INDEX(index, data.size());

	data.remove(index);
}

Tile::Tile() {
	symmetry = WaveFormCollapse::SYMMETRY_X;
	weight = 0;
}

Tile::Tile(WaveFormCollapse::Symmetry p_symmetry, double p_weight) {
	symmetry = p_symmetry;
	weight = p_weight;
}

// Create a tile with its differents orientations, its symmetries and its weight on the distribution of tiles.
Tile::Tile(const Vector<Array2D<int>> &p_data, WaveFormCollapse::Symmetry p_symmetry, double p_weight) {
	data = p_data;
	symmetry = p_symmetry;
	weight = p_weight;
}

// Create a tile with its base orientation, its symmetries and its weight on the distribution of tiles.
// The other orientations are generated with its first one.
Tile::Tile(const Array2D<int> &p_data, WaveFormCollapse::Symmetry p_symmetry, double p_weight) {
	data = generate_oriented(p_data, p_symmetry);
	symmetry = p_symmetry;
	weight = p_weight;
}

Tile::Tile(const PoolIntArray &p_data, const int width, const int height, WaveFormCollapse::Symmetry p_symmetry, double p_weight) {
	Array2D<int> d(p_data, height, width);

	data = generate_oriented(d, p_symmetry);
	symmetry = p_symmetry;
	weight = p_weight;
}

int TilingWaveFormCollapse::tile_add_generated(const PoolIntArray &data, const int width, const int height, const WaveFormCollapse::Symmetry symmetry, const float weight) {
	tiles.push_back(Tile(data, width, height, symmetry, weight));

	return tiles.size() - 1;
}
int TilingWaveFormCollapse::tile_add(const WaveFormCollapse::Symmetry symmetry, const float weight) {
	tiles.push_back(Tile(symmetry, weight));

	return tiles.size() - 1;
}
int TilingWaveFormCollapse::tile_create() {
	tiles.push_back(Tile());

	return tiles.size() - 1;
}
void TilingWaveFormCollapse::tile_remove(const int tile_index) {
	ERR_FAIL_INDEX(tile_index, tiles.size());

	tiles.remove(tile_index);
}
int TilingWaveFormCollapse::tile_count_get() {
	return tiles.size();
}

void TilingWaveFormCollapse::tile_data_add(const int tile_index, const PoolIntArray &data, const int width, const int height) {
	ERR_FAIL_INDEX(tile_index, tiles.size());

	Array2D<int> d(data, height, width);

	tiles.write[tile_index].data.push_back(d);
}
void TilingWaveFormCollapse::tile_data_generated_add(const int tile_index, const PoolIntArray &data, const int width, const int height) {
	ERR_FAIL_INDEX(tile_index, tiles.size());

	tiles.write[tile_index].set_generate_data(data, width, height);
}
PoolIntArray TilingWaveFormCollapse::tile_data_get(const int tile_index, const int data_index) {
	ERR_FAIL_INDEX_V(tile_index, tiles.size(), PoolIntArray());

	return tiles.write[tile_index].data_get(data_index);
}
void TilingWaveFormCollapse::tile_data_set(const int tile_index, const int data_index, const PoolIntArray &data, const int width, const int height) {
	ERR_FAIL_INDEX(tile_index, tiles.size());

	tiles.write[tile_index].data_set(data_index, data, width, height);
}
void TilingWaveFormCollapse::tile_data_remove(const int tile_index, const int data_index) {
	ERR_FAIL_INDEX(tile_index, tiles.size());

	tiles.write[tile_index].data_remove(data_index);
}
void TilingWaveFormCollapse::tile_data_clear(const int tile_index) {
	ERR_FAIL_INDEX(tile_index, tiles.size());

	tiles.write[tile_index].data.clear();
}
int TilingWaveFormCollapse::tile_data_count_get(const int tile_index) {
	ERR_FAIL_INDEX_V(tile_index, tiles.size(), 0);

	return tiles[tile_index].data.size();
}
int TilingWaveFormCollapse::tile_data_required_count_get(const int tile_index) {
	ERR_FAIL_INDEX_V(tile_index, tiles.size(), 0);

	int symm_indx = static_cast<int>(tiles[tile_index].symmetry);

	return Tile::rotation_map[symm_indx][0];
}

int TilingWaveFormCollapse::tile_width_get(const int tile_index, const int data_index) {
	ERR_FAIL_INDEX_V(tile_index, tiles.size(), 0);
	ERR_FAIL_INDEX_V(data_index, tiles[tile_index].data.size(), 0);

	return tiles[tile_index].data[data_index].width;
}
int TilingWaveFormCollapse::tile_height_get(const int tile_index, const int data_index) {
	ERR_FAIL_INDEX_V(tile_index, tiles.size(), 0);
	ERR_FAIL_INDEX_V(data_index, tiles[tile_index].data.size(), 0);

	return tiles[tile_index].data[data_index].height;
}

WaveFormCollapse::Symmetry TilingWaveFormCollapse::tile_symmetry_get(const int tile_index) {
	ERR_FAIL_INDEX_V(tile_index, tiles.size(), WaveFormCollapse::SYMMETRY_X);

	return tiles[tile_index].symmetry;
}
void TilingWaveFormCollapse::tile_symmetry_set(const int tile_index, const WaveFormCollapse::Symmetry val) {
	ERR_FAIL_INDEX(tile_index, tiles.size());

	tiles.write[tile_index].symmetry = val;
}

float TilingWaveFormCollapse::tile_weight_get(const int tile_index) {
	ERR_FAIL_INDEX_V(tile_index, tiles.size(), 0);

	return tiles[tile_index].weight;
}
void TilingWaveFormCollapse::tile_weight_set(const int tile_index, const float val) {
	ERR_FAIL_INDEX(tile_index, tiles.size());

	tiles.write[tile_index].weight = val;
}

String TilingWaveFormCollapse::tile_name_get(const int tile_index) {
	ERR_FAIL_INDEX_V(tile_index, tiles.size(), String());

	return tiles[tile_index].name;
}
void TilingWaveFormCollapse::tile_name_set(const int tile_index, const String &val) {
	ERR_FAIL_INDEX(tile_index, tiles.size());

	tiles.write[tile_index].name = val;
}
int TilingWaveFormCollapse::tile_index_get(const String &tile_name) {
	for (int i = 0; i < tiles.size(); ++i) {
		if (tiles[i].name == tile_name) {
			return i;
		}
	}

	return -1;
}

int TilingWaveFormCollapse::neighbour_data_add(const int left, const int left_orientation, const int right, const int right_orientation) {
	ERR_FAIL_COND_V(!neighbour_data_validate(left, left_orientation), -1);
	ERR_FAIL_COND_V(!neighbour_data_validate(right, right_orientation), -1);

	NeighbourData d(left, left_orientation, right, right_orientation);

	neighbors.push_back(d);

	return neighbors.size() - 1;
}
int TilingWaveFormCollapse::neighbour_data_add_str(const String &left, const int left_orientation, const String &right, const int right_orientation) {
	int left_index = tile_index_get(left);
	int right_index = tile_index_get(right);

	ERR_FAIL_COND_V(left_index == -1, -1);
	ERR_FAIL_COND_V(right_index == -1, -1);

	return neighbour_data_add(left_index, left_orientation, right_index, right_orientation);
}

PoolIntArray TilingWaveFormCollapse::neighbour_data_get(const int index) {
	ERR_FAIL_INDEX_V(index, neighbors.size(), PoolIntArray());

	const NeighbourData &d = neighbors[index];

	PoolIntArray p;
	p.resize(4);

	PoolIntArray::Write w = p.write();

	for (int i = 0; i < 4; ++i) {
		w[i] = d.data[i];
	}

	w.release();

	return p;
}
void TilingWaveFormCollapse::neighbour_data_remove(const int index) {
	ERR_FAIL_INDEX(index, neighbors.size());

	neighbors.remove(index);
}

void TilingWaveFormCollapse::neighbour_data_set(const int index, const int left, const int left_orientation, const int right, const int right_orientation) {
	ERR_FAIL_INDEX(index, neighbors.size());

	ERR_FAIL_COND(!neighbour_data_validate(left, left_orientation));
	ERR_FAIL_COND(!neighbour_data_validate(right, right_orientation));

	NeighbourData d(left, left_orientation, right, right_orientation);

	neighbors.write[index] = d;
}

void TilingWaveFormCollapse::neighbour_data_set_str(const int index, const String &left, const int left_orientation, const String &right, const int right_orientation) {
	int left_index = tile_index_get(left);
	int right_index = tile_index_get(right);

	ERR_FAIL_COND(left_index == -1);
	ERR_FAIL_COND(right_index == -1);

	neighbour_data_set(index, left_index, left_orientation, right_index, right_orientation);
}

bool TilingWaveFormCollapse::neighbour_data_validate(const int tile_index, const int orientation) {
	ERR_FAIL_INDEX_V(tile_index, tiles.size(), false);

	if (orientation < 0) {
		return false;
	}

	int symm_indx = static_cast<int>(tiles[tile_index].symmetry);

	if (orientation >= Tile::rotation_map[symm_indx][0]) {
		return false;
	}

	return true;
}
bool TilingWaveFormCollapse::neighbour_data_validate_str(const String &tile_name, const int orientation) {
	int tile_index = tile_index_get(tile_name);

	ERR_FAIL_COND_V(tile_index == -1, false);

	return neighbour_data_validate(tile_index, orientation);
}

// Returns false if the given tile and orientation does not exist, or if the coordinates are not in the wave
bool TilingWaveFormCollapse::set_tile(int tile_id, int orientation, int i, int j) {
	if (tile_id >= static_cast<int>(oriented_tile_ids.size()) || orientation >= static_cast<int>(oriented_tile_ids[tile_id].size()) ||
			i >= _wave_height || j >= _wave_width) {
		return false;
	}

	int oriented_tile_id = oriented_tile_ids[tile_id][orientation];
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

	int id = 0;
	for (int i = 0; i < tiles.size(); i++) {
		oriented_tile_ids.push_back(Vector<int>());

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

		int tile1 = neighbour.data[0];
		int tile2 = neighbour.data[2];
		Tile::ActionMap action_map1 = Tile::generate_action_map(tiles[tile1].symmetry);
		Tile::ActionMap action_map2 = Tile::generate_action_map(tiles[tile2].symmetry);

		generate_propagator_add_helper(action_map1, action_map2, &dense_propagator, neighbour, 0, 2);
		generate_propagator_add_helper(action_map1, action_map2, &dense_propagator, neighbour, 1, 0);
		generate_propagator_add_helper(action_map1, action_map2, &dense_propagator, neighbour, 2, 1);
		generate_propagator_add_helper(action_map1, action_map2, &dense_propagator, neighbour, 3, 3);
		generate_propagator_add_helper(action_map1, action_map2, &dense_propagator, neighbour, 4, 1);
		generate_propagator_add_helper(action_map1, action_map2, &dense_propagator, neighbour, 5, 3);
		generate_propagator_add_helper(action_map1, action_map2, &dense_propagator, neighbour, 6, 2);
		generate_propagator_add_helper(action_map1, action_map2, &dense_propagator, neighbour, 7, 0);
	}

	Vector<PropagatorStateEntry> propagator;
	propagator.resize(nb_oriented_tiles);

	PropagatorStateEntry *propw = propagator.ptrw();

	for (int i = 0; i < nb_oriented_tiles; ++i) {
		for (int j = 0; j < nb_oriented_tiles; ++j) {
			for (int d = 0; d < 4; ++d) {
				if (dense_propagator[i].directions[d][j]) {
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

void TilingWaveFormCollapse::set_tile(int tile_id, int i, int j) {
	for (int p = 0; p < id_to_oriented_tile.size(); p++) {
		if (tile_id != static_cast<int>(p)) {
			remove_wave_pattern(i, j, p);
		}
	}
}

Array2D<int> TilingWaveFormCollapse::run() {
	Array2D<int> a = WaveFormCollapse::run();

	if (a.width == 0 && a.height == 0) {
		return Array2D<int>(0, 0);
	}

	return id_to_tiling(a);
}

// Translate the generic WFC result into the image result
Array2D<int> TilingWaveFormCollapse::id_to_tiling(Array2D<int> ids) {
	int size = tiles[0].data[0].height;
	Array2D<int> tiling(size * ids.height, size * ids.width);

	for (int i = 0; i < ids.height; i++) {
		for (int j = 0; j < ids.width; j++) {
			IdToTilePair oriented_tile = id_to_oriented_tile[ids.get(i, j)];

			for (int y = 0; y < size; y++) {
				for (int x = 0; x < size; x++) {
					tiling.get(i * size + y, j * size + x) = tiles[oriented_tile.id].data[oriented_tile.oriented_tile].get(y, x);
				}
			}
		}
	}

	return tiling;
}

bool TilingWaveFormCollapse::validate() {
	for (int i = 0; i < tiles.size(); ++i) {
		int symm_indx = static_cast<int>(tiles[i].symmetry);

		int symm_req_count = Tile::rotation_map[symm_indx][0];

		if (tiles[i].data.size() != symm_req_count) {
			return false;
		}
	}

	return true;
}

void TilingWaveFormCollapse::initialize() {
	ERR_FAIL_COND(!validate());

	generate_oriented_tile_ids();
	generate_propagator();

	WaveFormCollapse::initialize();
}

TilingWaveFormCollapse::TilingWaveFormCollapse() {
}
TilingWaveFormCollapse::~TilingWaveFormCollapse() {
}

void TilingWaveFormCollapse::_bind_methods() {
	ClassDB::bind_method(D_METHOD("tile_add_generated", "data", "width", "height", "symmetry", "weight"), &TilingWaveFormCollapse::tile_add_generated);
	ClassDB::bind_method(D_METHOD("tile_add", "symmetry", "weight"), &TilingWaveFormCollapse::tile_add);
	ClassDB::bind_method(D_METHOD("tile_create"), &TilingWaveFormCollapse::tile_create);
	ClassDB::bind_method(D_METHOD("tile_remove", "tile_index"), &TilingWaveFormCollapse::tile_remove);
	ClassDB::bind_method(D_METHOD("tile_count_get"), &TilingWaveFormCollapse::tile_count_get);

	ClassDB::bind_method(D_METHOD("tile_data_add", "tile_index", "data", "width", "height"), &TilingWaveFormCollapse::tile_data_add);
	ClassDB::bind_method(D_METHOD("tile_data_generated_add", "tile_index", "data", "width", "height"), &TilingWaveFormCollapse::tile_data_generated_add);
	ClassDB::bind_method(D_METHOD("tile_data_get", "tile_index", "data_index"), &TilingWaveFormCollapse::tile_data_get);
	ClassDB::bind_method(D_METHOD("tile_data_set", "tile_index", "data_index", "data", "width", "height"), &TilingWaveFormCollapse::tile_data_set);
	ClassDB::bind_method(D_METHOD("tile_data_remove", "tile_index", "data_index"), &TilingWaveFormCollapse::tile_data_remove);
	ClassDB::bind_method(D_METHOD("tile_data_clear", "tile_index"), &TilingWaveFormCollapse::tile_data_clear);
	ClassDB::bind_method(D_METHOD("tile_data_count_get", "tile_index"), &TilingWaveFormCollapse::tile_data_count_get);
	ClassDB::bind_method(D_METHOD("tile_data_required_count_get", "tile_index"), &TilingWaveFormCollapse::tile_data_required_count_get);

	ClassDB::bind_method(D_METHOD("tile_width_get", "tile_index", "data_index"), &TilingWaveFormCollapse::tile_width_get);
	ClassDB::bind_method(D_METHOD("tile_height_get", "tile_index", "data_index"), &TilingWaveFormCollapse::tile_height_get);

	ClassDB::bind_method(D_METHOD("tile_symmetry_get", "tile_index"), &TilingWaveFormCollapse::tile_symmetry_get);
	ClassDB::bind_method(D_METHOD("tile_symmetry_set", "tile_index", "val"), &TilingWaveFormCollapse::tile_symmetry_set);

	ClassDB::bind_method(D_METHOD("tile_weight_get", "tile_index"), &TilingWaveFormCollapse::tile_weight_get);
	ClassDB::bind_method(D_METHOD("tile_weight_set", "tile_index", "val"), &TilingWaveFormCollapse::tile_weight_set);

	ClassDB::bind_method(D_METHOD("tile_name_get", "tile_index"), &TilingWaveFormCollapse::tile_name_get);
	ClassDB::bind_method(D_METHOD("tile_name_set", "tile_index", "val"), &TilingWaveFormCollapse::tile_name_set);
	ClassDB::bind_method(D_METHOD("tile_index_get", "tile_name"), &TilingWaveFormCollapse::tile_index_get);

	ClassDB::bind_method(D_METHOD("neighbour_data_add", "left", "left_orientation", "right", "right_orientation"), &TilingWaveFormCollapse::neighbour_data_add);
	ClassDB::bind_method(D_METHOD("neighbour_data_add_str", "left", "left_orientation", "right", "right_orientation"), &TilingWaveFormCollapse::neighbour_data_add_str);
	ClassDB::bind_method(D_METHOD("neighbour_data_get", "index"), &TilingWaveFormCollapse::neighbour_data_get);
	ClassDB::bind_method(D_METHOD("neighbour_data_remove", "index"), &TilingWaveFormCollapse::neighbour_data_remove);
	ClassDB::bind_method(D_METHOD("neighbour_data_set", "tile_index", "left", "left_orientation", "right", "right_orientation"), &TilingWaveFormCollapse::neighbour_data_set);
	ClassDB::bind_method(D_METHOD("neighbour_data_set_str", "tile_index", "left", "left_orientation", "right", "right_orientation"), &TilingWaveFormCollapse::neighbour_data_set_str);
	ClassDB::bind_method(D_METHOD("neighbour_data_validate", "tile_index", "orientation"), &TilingWaveFormCollapse::neighbour_data_validate);
	ClassDB::bind_method(D_METHOD("neighbour_data_validate_str", "tile_name", "orientation"), &TilingWaveFormCollapse::neighbour_data_validate_str);
}

void TilingWaveFormCollapse::generate_propagator_add_helper(const Tile::ActionMap &action_map1, const Tile::ActionMap &action_map2,
		Vector<DensePropagatorHelper> *dense_propagator,
		const NeighbourData &neighbour, int action, int direction) {
	// --
	int tile1 = neighbour.data[0];
	int orientation1 = neighbour.data[1];
	int tile2 = neighbour.data[2];
	int orientation2 = neighbour.data[3];

	int temp_orientation1 = action_map1.map[action][orientation1];
	int temp_orientation2 = action_map2.map[action][orientation2];
	int oriented_tile_id1 = oriented_tile_ids[tile1][temp_orientation1];
	int oriented_tile_id2 = oriented_tile_ids[tile2][temp_orientation2];
	dense_propagator->write[oriented_tile_id1].directions[direction].write[oriented_tile_id2] = true;
	direction = get_opposite_direction(direction);
	dense_propagator->write[oriented_tile_id2].directions[direction].write[oriented_tile_id1] = true;
}