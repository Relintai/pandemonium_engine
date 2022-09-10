/*
Copyright (c) 2019-2022 Péter Magyar

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "terrain_2d_job.h"

#include "../default/terrain_2d_chunk_default.h"

#include "../../../opensimplex/open_simplex_noise.h"

const String Terrain2DJob::BINDING_STRING_ACTIVE_BUILD_PHASE_TYPE = "Normal,Process,Physics Process";

Terrain2DJob::ActiveBuildPhaseType Terrain2DJob::get_build_phase_type() {
	return _build_phase_type;
}
void Terrain2DJob::set_build_phase_type(Terrain2DJob::ActiveBuildPhaseType build_phase_type) {
	_build_phase_type = build_phase_type;
}

void Terrain2DJob::set_chunk(const Ref<Terrain2DChunk> &chunk) {
	_chunk = chunk;

	_in_tree = true;
}

int Terrain2DJob::get_phase() {
	return _phase;
}
void Terrain2DJob::set_phase(const int phase) {
	_phase = phase;
}
void Terrain2DJob::next_phase() {
	++_phase;
}

bool Terrain2DJob::get_build_done() {
	return _build_done;
}
void Terrain2DJob::set_build_done(const bool val) {
	_build_done = val;
}

void Terrain2DJob::next_job() {
	ERR_FAIL_COND(!_chunk.is_valid());

	_chunk->job_next();
	set_build_done(true);
}

void Terrain2DJob::reset() {
	call("_reset");
}
void Terrain2DJob::_reset() {
	_build_done = false;
	_phase = 0;
}

void Terrain2DJob::_execute() {
	ActiveBuildPhaseType origpt = _build_phase_type;

	while (!get_cancelled() && _in_tree && !_build_done && origpt == _build_phase_type && !should_return()) {
		execute_phase();
	}

	if (!_in_tree) {
		_chunk.unref();
	}
}

void Terrain2DJob::execute_phase() {
	call("_execute_phase");
}

void Terrain2DJob::_execute_phase() {
	next_job();
}

void Terrain2DJob::process(const float delta) {
	if (has_method("_process"))
		call("_process", delta);
}
void Terrain2DJob::physics_process(const float delta) {
	if (has_method("_physics_process"))
		call("_physics_process", delta);
}

//Data Management functions
void Terrain2DJob::generate_ao() {
}

void Terrain2DJob::generate_random_ao(int seed, int octaves, int period, float persistence, float scale_factor) {
	ERR_FAIL_COND(!_chunk.is_valid());

	int margin_start = _chunk->get_margin_start();
	int margin_end = _chunk->get_margin_end();

	int size_x = _chunk->get_size_x();
	int size_y = _chunk->get_size_y();

	int position_x = _chunk->get_position_x();
	int position_y = _chunk->get_position_y();

	int psx = position_x * size_x;
	int psy = position_y * size_y;

	float cx = _chunk->get_cell_size_x();
	float cy = _chunk->get_cell_size_y();

	Ref<OpenSimplexNoise> noise;
	noise.instance();

	noise->set_seed(seed);
	noise->set_octaves(octaves);
	noise->set_period(period);
	noise->set_persistence(persistence);

	for (int x = -margin_start; x < size_x + margin_end; ++x) {
		for (int y = -margin_start; y < size_y + margin_end; ++y) {
			float xx = x + psx;
			float yy = y + psy;
			xx /= cx;
			yy /= cy;

			float val = noise->get_noise_2d(xx, yy);

			val *= scale_factor;

			if (val > 1)
				val = 1;

			if (val < 0)
				val = -val;

			_chunk->set_voxel(int(val * 255.0), x, y, Terrain2DChunkDefault::DEFAULT_CHANNEL_RANDOM_AO);
		}
	}
}

Array Terrain2DJob::merge_mesh_array(Array arr) const {
	ERR_FAIL_COND_V(arr.size() != RenderingServer::ARRAY_MAX, arr);

	PoolVector3Array verts = arr[RenderingServer::ARRAY_VERTEX];
	PoolVector3Array normals = arr[RenderingServer::ARRAY_NORMAL];
	PoolVector2Array uvs = arr[RenderingServer::ARRAY_TEX_UV];
	PoolColorArray colors = arr[RenderingServer::ARRAY_COLOR];
	PoolIntArray indices = arr[RenderingServer::ARRAY_INDEX];

	bool has_normals = normals.size() > 0;
	bool has_uvs = uvs.size() > 0;
	bool has_colors = colors.size() > 0;

	int i = 0;
	while (i < verts.size()) {
		Vector3 v = verts[i];

		Array equals;
		for (int j = i + 1; j < verts.size(); ++j) {
			Vector3 vc = verts[j];

			if (Math::is_equal_approx(v.x, vc.x) && Math::is_equal_approx(v.y, vc.y) && Math::is_equal_approx(v.z, vc.z))
				equals.push_back(j);
		}

		for (int k = 0; k < equals.size(); ++k) {
			int rem = equals[k];
			int remk = rem - k;

			verts.remove(remk);

			if (has_normals)
				normals.remove(remk);
			if (has_uvs)
				uvs.remove(remk);
			if (has_colors)
				colors.remove(remk);

			for (int j = 0; j < indices.size(); ++j) {
				int indx = indices[j];

				if (indx == remk)
					indices.set(j, i);
				else if (indx > remk)
					indices.set(j, indx - 1);
			}
		}

		++i;
	}

	arr[RenderingServer::ARRAY_VERTEX] = verts;

	if (has_normals)
		arr[RenderingServer::ARRAY_NORMAL] = normals;
	if (has_uvs)
		arr[RenderingServer::ARRAY_TEX_UV] = uvs;
	if (has_colors)
		arr[RenderingServer::ARRAY_COLOR] = colors;

	arr[RenderingServer::ARRAY_INDEX] = indices;

	return arr;
}
Array Terrain2DJob::bake_mesh_array_uv(Array arr, Ref<Texture> tex, const float mul_color) const {
	ERR_FAIL_COND_V(arr.size() != RenderingServer::ARRAY_MAX, arr);
	ERR_FAIL_COND_V(!tex.is_valid(), arr);

	Ref<Image> img = tex->get_data();

	ERR_FAIL_COND_V(!img.is_valid(), arr);

	Vector2 imgsize = img->get_size();

	PoolVector2Array uvs = arr[RenderingServer::ARRAY_TEX_UV];
	PoolColorArray colors = arr[RenderingServer::ARRAY_COLOR];

	if (colors.size() < uvs.size())
		colors.resize(uvs.size());

	img->lock();

	for (int i = 0; i < uvs.size(); ++i) {
		Vector2 uv = uvs[i];
		uv *= imgsize;

		int ux = static_cast<int>(CLAMP(uv.x, 0, imgsize.x - 1));
		int uy = static_cast<int>(CLAMP(uv.y, 0, imgsize.y - 1));

		Color c = img->get_pixel(ux, uy);

		colors.set(i, colors[i] * c * mul_color);
	}

	img->unlock();

	arr[RenderingServer::ARRAY_COLOR] = colors;

	return arr;
}

void Terrain2DJob::chunk_exit_tree() {
	_in_tree = false;

	if (get_complete()) {
		_chunk.unref();
	} else {
		set_cancelled(true);
	}
}

Terrain2DJob::Terrain2DJob() {
	_in_tree = false;

	_build_phase_type = BUILD_PHASE_TYPE_NORMAL;
	_build_done = true;
	_phase = 0;
}

Terrain2DJob::~Terrain2DJob() {
	_chunk.unref();
}

void Terrain2DJob::_bind_methods() {
	BIND_VMETHOD(MethodInfo("_process", PropertyInfo(Variant::REAL, "delta")));
	BIND_VMETHOD(MethodInfo("_physics_process", PropertyInfo(Variant::REAL, "delta")));

	ClassDB::bind_method(D_METHOD("get_build_phase_type"), &Terrain2DJob::get_build_phase_type);
	ClassDB::bind_method(D_METHOD("set_build_phase_type", "value"), &Terrain2DJob::set_build_phase_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "build_phase_type", PROPERTY_HINT_ENUM, BINDING_STRING_ACTIVE_BUILD_PHASE_TYPE), "set_build_phase_type", "get_build_phase_type");

	ClassDB::bind_method(D_METHOD("set_chunk", "chunk"), &Terrain2DJob::set_chunk);

	ClassDB::bind_method(D_METHOD("get_phase"), &Terrain2DJob::get_phase);
	ClassDB::bind_method(D_METHOD("set_phase", "phase"), &Terrain2DJob::set_phase);
	ClassDB::bind_method(D_METHOD("next_phase"), &Terrain2DJob::next_phase);

	ClassDB::bind_method(D_METHOD("get_build_done"), &Terrain2DJob::get_build_done);
	ClassDB::bind_method(D_METHOD("set_build_done", "val"), &Terrain2DJob::set_build_done);

	ClassDB::bind_method(D_METHOD("next_job"), &Terrain2DJob::next_job);

	BIND_VMETHOD(MethodInfo("_reset"));

	ClassDB::bind_method(D_METHOD("reset"), &Terrain2DJob::reset);
	ClassDB::bind_method(D_METHOD("_reset"), &Terrain2DJob::_reset);

	ClassDB::bind_method(D_METHOD("_execute"), &Terrain2DJob::_execute);

	BIND_VMETHOD(MethodInfo("_execute_phase"));

	ClassDB::bind_method(D_METHOD("execute_phase"), &Terrain2DJob::execute_phase);
	ClassDB::bind_method(D_METHOD("_execute_phase"), &Terrain2DJob::_execute_phase);

	ClassDB::bind_method(D_METHOD("generate_ao"), &Terrain2DJob::generate_ao);
	ClassDB::bind_method(D_METHOD("generate_random_ao", "seed", "octaves", "period", "persistence", "scale_factor"), &Terrain2DJob::generate_random_ao, DEFVAL(4), DEFVAL(30), DEFVAL(0.3), DEFVAL(0.6));

	ClassDB::bind_method(D_METHOD("chunk_exit_tree"), &Terrain2DJob::chunk_exit_tree);
}
