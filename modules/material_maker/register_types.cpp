/*
Copyright (c) 2022 Péter Magyar

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

#include "register_types.h"

#include "core/engine.h"

#include "algos/mm_algos.h"
#include "algos/mm_algos_bind.h"

#include "nodes/mm_material.h"
#include "nodes/mm_node.h"
#include "nodes/mm_node_universal_property.h"

#include "nodes/bases/curve_base.h"
#include "nodes/bases/gradient_base.h"
#include "nodes/bases/polygon_base.h"

#include "editor/mat_maker_gd_editor.h"
#include "editor/mm_graph_node.h"

#if TOOLS_ENABLED
#include "editor_plugin.h"
#endif

#include "nodes/uniform/greyscale_uniform.h"
#include "nodes/uniform/uniform.h"

#include "nodes/transform/circle_map.h"
#include "nodes/transform/color_tiler.h"
#include "nodes/transform/kaleidoscope.h"
#include "nodes/transform/mirror.h"
#include "nodes/transform/repeat.h"
#include "nodes/transform/rotate.h"
#include "nodes/transform/scale.h"
#include "nodes/transform/shear.h"
#include "nodes/transform/tiler.h"
#include "nodes/transform/transform.h"
#include "nodes/transform/translate.h"

#include "nodes/simple/curve.h"
#include "nodes/simple/image.h"
#include "nodes/simple/shape.h"

#include "nodes/sdf3d/sdf3d_tf_rotate.h"
#include "nodes/sdf3d/sdf3d_tf_scale.h"
#include "nodes/sdf3d/sdf3d_tf_translate.h"

#include "nodes/sdf3d/sdf3d_shape_torus.h"

static _MMAlgos *_mm_algos_singleton = nullptr;

void register_material_maker_types() {
	ClassDB::register_class<_MMAlgos>();

	ClassDB::register_class<MMNodeUniversalProperty>();
	ClassDB::register_class<MMMaterial>();
	ClassDB::register_class<MMNode>();

	ClassDB::register_class<CurveBase>();
	ClassDB::register_class<GradientBase>();
	ClassDB::register_class<PolygonBase>();

	ClassDB::register_class<MMGraphNode>();
	ClassDB::register_class<MatMakerGDEditor>();

	ClassDB::register_class<MMUniform>();
	MMAlgos::register_node_class("Uniform", "MMUniform");
	ClassDB::register_class<MMGreyscaleUniform>();
	MMAlgos::register_node_class("Uniform", "MMGreyscaleUniform");

	ClassDB::register_class<MMTranslate>();
	MMAlgos::register_node_class("Transform", "MMTranslate");
	ClassDB::register_class<MMTransform>();
	MMAlgos::register_node_class("Transform", "MMTransform");
	ClassDB::register_class<MMTiler>();
	MMAlgos::register_node_class("Transform", "MMTiler");
	ClassDB::register_class<MMShear>();
	MMAlgos::register_node_class("Transform", "MMShear");
	ClassDB::register_class<MMScale>();
	MMAlgos::register_node_class("Transform", "MMScale");
	ClassDB::register_class<MMRotate>();
	MMAlgos::register_node_class("Transform", "MMRotate");
	ClassDB::register_class<MMRepeat>();
	MMAlgos::register_node_class("Transform", "MMRepeat");
	ClassDB::register_class<MMMirror>();
	MMAlgos::register_node_class("Transform", "MMMirror");
	ClassDB::register_class<MMKaleidoscope>();
	MMAlgos::register_node_class("Transform", "MMKaleidoscope");
	ClassDB::register_class<MMColorTiler>();
	MMAlgos::register_node_class("Transform", "MMColorTiler");
	ClassDB::register_class<MMCircleMap>();
	MMAlgos::register_node_class("Transform", "MMCircleMap");

	ClassDB::register_class<MMShape>();
	MMAlgos::register_node_class("Simple", "MMShape");
	ClassDB::register_class<MMImage>();
	MMAlgos::register_node_class("Simple", "MMImage");
	ClassDB::register_class<MMCurve>();
	MMAlgos::register_node_class("Simple", "MMCurve");

	ClassDB::register_class<MMSdf3dTfTranslate>();
	MMAlgos::register_node_class("SDF3D - TF", "MMSdf3dTfTranslate");
	ClassDB::register_class<MMSdf3dTfScale>();
	MMAlgos::register_node_class("SDF3D - TF", "MMSdf3dTfScale");
	ClassDB::register_class<MMSdf3dTfRotate>();
	MMAlgos::register_node_class("SDF3D - TF", "MMSdf3dTfRotate");

	ClassDB::register_class<MMSdf3dShapeTorus>();
	MMAlgos::register_node_class("SDF3D - Shape", "MMSdf3dShapeTorus");

	_mm_algos_singleton = memnew(_MMAlgos);
	Engine::get_singleton()->add_singleton(Engine::Singleton("MMAlgos", _MMAlgos::get_singleton()));

#if TOOLS_ENABLED
	EditorPlugins::add_by_type<MaterialMakerEditorPlugin>();
#endif
}

void unregister_material_maker_types() {
	if (_mm_algos_singleton) {
		memdelete(_mm_algos_singleton);
	}
}
