#ifndef PROP_2D_DATA_LIGHT_H
#define PROP_2D_DATA_LIGHT_H


#include "core/math/color.h"

#include "prop_2d_data_entry.h"

class Prop2DDataLight : public Prop2DDataEntry {
	GDCLASS(Prop2DDataLight, Prop2DDataEntry);

public:
	Color get_light_color() const;
	void set_light_color(const Color value);

	int get_light_size_x() const;
	void set_light_size_x(const int value);

	int get_light_size_y() const;
	void set_light_size_y(const int value);

	float get_texture_scale() const;
	void set_texture_scale(const float value);

	float get_energy() const;
	void set_energy(const float value);

	Ref<Texture> get_texture() const;
	void set_texture(const Ref<Texture> value);

	bool _processor_handles(Node *node);
	void _processor_process(Ref<Prop2DData> prop_data, Node *node, const Transform2D &transform, Ref<Prop2DDataEntry> entry = Ref<Prop2DDataEntry>());
	Node *_processor_get_node_for(const Transform2D &transform, Node *node = nullptr);

	Prop2DDataLight();
	~Prop2DDataLight();

protected:
	static void _bind_methods();

private:
	Color _light_color;
	int _light_size_x;
	int _light_size_y;
	float _texture_scale;
	float _energy;
	Ref<Texture> _texture;
};

#endif
