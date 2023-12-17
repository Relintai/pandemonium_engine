#ifndef TERRAIN_ENVIRONMENT_DATA_H
#define TERRAIN_ENVIRONMENT_DATA_H


#include "core/math/color.h"
#include "core/object/resource.h"

#include "../defines.h"

#include "scene/3d/light.h"

#include "scene/3d/world_environment_3d.h"
#include "scene/main/node.h"

class TerrainEnvironmentData : public Resource {
	GDCLASS(TerrainEnvironmentData, Resource);

public:
	Ref<Environment3D> get_environment();
	void set_environment(const Ref<Environment3D> &value);

	Color get_color(const int index);
	void set_color(const int index, const Color &value);
	float get_energy(const int index);
	void set_energy(const int index, const float value);
	float get_indirect_energy(const int index);
	void set_indirect_energy(const int index, const float value);

	void setup(WorldEnvironment3D *world_environment, DirectionalLight *primary_light, DirectionalLight *secondary_light);
	void setup_bind(Node *world_environment, Node *primary_light, Node *secondary_light);

	TerrainEnvironmentData();
	~TerrainEnvironmentData();

public:
	enum {
		LIGHT_COUNT = 2,
	};

protected:
	static void _bind_methods();

private:
	Ref<Environment3D> _environment;

	Color _colors[LIGHT_COUNT];
	float _energies[LIGHT_COUNT];
	float _indirect_energies[LIGHT_COUNT];
};

#endif
