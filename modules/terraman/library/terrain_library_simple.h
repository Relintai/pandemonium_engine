#ifndef TERRAIN_LIBRARY_SIMPLE_H
#define TERRAIN_LIBRARY_SIMPLE_H


#include "core/object/resource.h"

#include "terrain_library.h"

#include "scene/resources/material/material.h"

#include "../data/terrain_light.h"
#include "terrain_surface_simple.h"

class TerrainSurfaceSimple;
class TerrainMesher;

class TerrainLibrarySimple : public TerrainLibrary {
	GDCLASS(TerrainLibrarySimple, TerrainLibrary)

public:
	int get_atlas_columns() const;
	void set_atlas_columns(int s);

	int get_atlas_rows() const;
	void set_atlas_rows(int s);

	Ref<TerrainSurface> terra_surface_get(const int index);
	void terra_surface_add(Ref<TerrainSurface> value);
	void terra_surface_set(const int index, Ref<TerrainSurface> value);
	void terra_surface_remove(const int index);
	int terra_surface_get_num() const;
	void terra_surfaces_clear();

	Vector<Variant> get_terra_surfaces();
	void set_terra_surfaces(const Vector<Variant> &surfaces);

	void refresh_rects();

	TerrainLibrarySimple();
	~TerrainLibrarySimple();

protected:
	static void _bind_methods();

private:
	Vector<Ref<TerrainSurfaceSimple>> _terra_surfaces;

	//atlas
	int _atlas_columns;
	int _atlas_rows;
};

#endif // TERRAIN_LIBRARY_H
