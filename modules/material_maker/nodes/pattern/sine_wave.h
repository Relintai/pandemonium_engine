#ifndef MM_SINE_WAVE_H
#define MM_SINE_WAVE_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMSineWave : public MMNode {
	GDCLASS(MMSineWave, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_image();
	void set_image(const Ref<MMNodeUniversalProperty> &val);

	float get_amplitude() const;
	void set_amplitude(const float val);

	float get_frequency() const;
	void set_frequency(const float val);

	float get_phase() const;
	void set_phase(const float val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	void _render(const Ref<MMMaterial> &material);
	Color _get_value_for(const Vector2 &uv, const int pseed);

	MMSineWave();
	~MMSineWave();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> image;
	float amplitude;
	float frequency;
	float phase;
};

#endif
