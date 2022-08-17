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

#include "core/config/engine.h"

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

#include "nodes/sdf3d/sdf3d_color.h"
#include "nodes/sdf3d/sdf3d_render.h"

#include "nodes/sdf3d/sdf3d_tf_rotate.h"
#include "nodes/sdf3d/sdf3d_tf_scale.h"
#include "nodes/sdf3d/sdf3d_tf_translate.h"

#include "nodes/sdf3d/sdf3d_shape_box.h"
#include "nodes/sdf3d/sdf3d_shape_capsule.h"
#include "nodes/sdf3d/sdf3d_shape_cone.h"
#include "nodes/sdf3d/sdf3d_shape_cylinder.h"
#include "nodes/sdf3d/sdf3d_shape_sphere.h"
#include "nodes/sdf3d/sdf3d_shape_torus.h"

#include "nodes/sdf3d/sdf3d_op_bool.h"
#include "nodes/sdf3d/sdf3d_op_circle_repeat.h"
#include "nodes/sdf3d/sdf3d_op_elongation.h"
#include "nodes/sdf3d/sdf3d_op_morph.h"
#include "nodes/sdf3d/sdf3d_op_repeat.h"
#include "nodes/sdf3d/sdf3d_op_revolution.h"
#include "nodes/sdf3d/sdf3d_op_rounded.h"
#include "nodes/sdf3d/sdf3d_op_smooth_bool.h"

#include "nodes/sdf2d/sd_show.h"

#include "nodes/sdf2d/sd_shape_arc.h"
#include "nodes/sdf2d/sd_shape_box.h"
#include "nodes/sdf2d/sd_shape_circle.h"
#include "nodes/sdf2d/sd_shape_line.h"
#include "nodes/sdf2d/sd_shape_polygon.h"
#include "nodes/sdf2d/sd_shape_rhombus.h"

#include "nodes/sdf2d/sd_tf_rotate.h"
#include "nodes/sdf2d/sd_tf_scale.h"
#include "nodes/sdf2d/sd_tf_translate.h"

#include "nodes/sdf2d/sd_tf_translate.h"

#include "nodes/sdf2d/sd_op_annular_shape.h"
#include "nodes/sdf2d/sd_op_bool.h"
#include "nodes/sdf2d/sd_op_circle_repeat.h"
#include "nodes/sdf2d/sd_op_morph.h"
#include "nodes/sdf2d/sd_op_repeat.h"
#include "nodes/sdf2d/sd_op_rounded_shape.h"
#include "nodes/sdf2d/sd_op_smooth_bool.h"

#include "nodes/pattern/beehive.h"
#include "nodes/pattern/bricks.h"
#include "nodes/pattern/iching.h"
#include "nodes/pattern/pattern.h"
#include "nodes/pattern/runes.h"
#include "nodes/pattern/scratches.h"
#include "nodes/pattern/sine_wave.h"
#include "nodes/pattern/truchet.h"
#include "nodes/pattern/weave.h"

#include "nodes/other/output_image.h"

#include "nodes/noise/anisotropic_noise.h"
#include "nodes/noise/color_noise.h"
#include "nodes/noise/color_value.h"
#include "nodes/noise/fbm_noise.h"
#include "nodes/noise/noise.h"
#include "nodes/noise/voronoi.h"

#include "nodes/gradient/circular_gradient.h"
#include "nodes/gradient/gradient.h"
#include "nodes/gradient/radial_gradient.h"

#include "nodes/filter/adjust_hsv.h"
#include "nodes/filter/blend.h"
#include "nodes/filter/blur_gaussian.h"
#include "nodes/filter/brightness_contrast.h"
#include "nodes/filter/colorize.h"
#include "nodes/filter/combine.h"
#include "nodes/filter/decompose.h"
#include "nodes/filter/emboss.h"
#include "nodes/filter/fill_channel.h"
#include "nodes/filter/fill_to_color.h"
#include "nodes/filter/fill_to_position.h"
#include "nodes/filter/fill_to_random_color.h"
#include "nodes/filter/fill_to_random_grey.h"
#include "nodes/filter/fill_to_size.h"
#include "nodes/filter/fill_to_uv.h"
#include "nodes/filter/greyscale.h"
#include "nodes/filter/invert.h"
#include "nodes/filter/make_tileable.h"
#include "nodes/filter/mm_math.h"
#include "nodes/filter/quantize.h"
#include "nodes/filter/swap_channels.h"
#include "nodes/filter/tonality.h"

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

	ClassDB::register_class<MMSdf3dColor>();
	MMAlgos::register_node_class("SDF3D", "MMSdf3dColor");
	ClassDB::register_class<MMSdf3dRender>();
	MMAlgos::register_node_class("SDF3D", "MMSdf3dRender");

	ClassDB::register_class<MMSdf3dTfTranslate>();
	MMAlgos::register_node_class("SDF3D - TF", "MMSdf3dTfTranslate");
	ClassDB::register_class<MMSdf3dTfScale>();
	MMAlgos::register_node_class("SDF3D - TF", "MMSdf3dTfScale");
	ClassDB::register_class<MMSdf3dTfRotate>();
	MMAlgos::register_node_class("SDF3D - TF", "MMSdf3dTfRotate");

	ClassDB::register_class<MMSdf3dShapeTorus>();
	MMAlgos::register_node_class("SDF3D - Shape", "MMSdf3dShapeTorus");
	ClassDB::register_class<MMSdf3dShapeSphere>();
	MMAlgos::register_node_class("SDF3D - Shape", "MMSdf3dShapeSphere");
	ClassDB::register_class<MMSdf3dShapeCylinder>();
	MMAlgos::register_node_class("SDF3D - Shape", "MMSdf3dShapeCylinder");
	ClassDB::register_class<MMSdf3dShapeCone>();
	MMAlgos::register_node_class("SDF3D - Shape", "MMSdf3dShapeCone");
	ClassDB::register_class<MMSdf3dShapeCapsule>();
	MMAlgos::register_node_class("SDF3D - Shape", "MMSdf3dShapeCapsule");
	ClassDB::register_class<MMSdf3dShapeBox>();
	MMAlgos::register_node_class("SDF3D - Shape", "MMSdf3dShapeBox");

	ClassDB::register_class<MMSdf3dOpSmoothBool>();
	MMAlgos::register_node_class("SDF3D - OP", "MMSdf3dOpSmoothBool");
	ClassDB::register_class<MMSdf3dOpRounded>();
	MMAlgos::register_node_class("SDF3D - OP", "MMSdf3dOpRounded");
	ClassDB::register_class<MMSdf3dOpRevolution>();
	MMAlgos::register_node_class("SDF3D - OP", "MMSdf3dOpRevolution");
	ClassDB::register_class<MMSdf3dOpRepeat>();
	MMAlgos::register_node_class("SDF3D - OP", "MMSdf3dOpRepeat");
	ClassDB::register_class<MMSdf3dOpMorph>();
	MMAlgos::register_node_class("SDF3D - OP", "MMSdf3dOpMorph");
	ClassDB::register_class<MMSdf3dOpElongation>();
	MMAlgos::register_node_class("SDF3D - OP", "MMSdf3dOpElongation");
	ClassDB::register_class<MMSdf3dOpCircleRepeat>();
	MMAlgos::register_node_class("SDF3D - OP", "MMSdf3dOpCircleRepeat");
	ClassDB::register_class<MMSdf3dOpBool>();
	MMAlgos::register_node_class("SDF3D - OP", "MMSdf3dOpBool");

	ClassDB::register_class<SSSdShow>();
	MMAlgos::register_node_class("SDF2D", "SSSdShow");

	ClassDB::register_class<MMSdShapeRhombus>();
	MMAlgos::register_node_class("SDF2D - Shape", "MMSdShapeRhombus");
	ClassDB::register_class<MMSdShapePolygon>();
	MMAlgos::register_node_class("SDF2D - Shape", "MMSdShapePolygon");
	ClassDB::register_class<MMSdShapeLine>();
	MMAlgos::register_node_class("SDF2D - Shape", "MMSdShapeLine");
	ClassDB::register_class<MMSdShapeCircle>();
	MMAlgos::register_node_class("SDF2D - Shape", "MMSdShapeCircle");
	ClassDB::register_class<MMSdShapeBox>();
	MMAlgos::register_node_class("SDF2D - Shape", "MMSdShapeBox");
	ClassDB::register_class<MMSdShapeArc>();
	MMAlgos::register_node_class("SDF2D - Shape", "MMSdShapeArc");

	ClassDB::register_class<MMSdTfTranslate>();
	MMAlgos::register_node_class("SDF2D - TF", "MMSdTfTranslate");
	ClassDB::register_class<MMSdTfScale>();
	MMAlgos::register_node_class("SDF2D - TF", "MMSdTfScale");
	ClassDB::register_class<MMSdTfRotate>();
	MMAlgos::register_node_class("SDF2D - TF", "MMSdTfRotate");

	ClassDB::register_class<MMSdOpSmoothBool>();
	MMAlgos::register_node_class("SDF2D - OP", "MMSdOpSmoothBool");
	ClassDB::register_class<MMSdOpRoundedShape>();
	MMAlgos::register_node_class("SDF2D - OP", "MMSdOpRoundedShape");
	ClassDB::register_class<MMSdOpRepeat>();
	MMAlgos::register_node_class("SDF2D - OP", "MMSdOpRepeat");
	ClassDB::register_class<MMSdOpMorph>();
	MMAlgos::register_node_class("SDF2D - OP", "MMSdOpMorph");
	ClassDB::register_class<MMSdOpCircleRepeat>();
	MMAlgos::register_node_class("SDF2D - OP", "MMSdOpCircleRepeat");
	ClassDB::register_class<MMSdOpBool>();
	MMAlgos::register_node_class("SDF2D - OP", "MMSdOpBool");
	ClassDB::register_class<MMSdOpAnnularShape>();
	MMAlgos::register_node_class("SDF2D - OP", "MMSdOpAnnularShape");

	ClassDB::register_class<MMWeave>();
	MMAlgos::register_node_class("Patterns", "MMWeave");
	ClassDB::register_class<MMTruchet>();
	MMAlgos::register_node_class("Patterns", "MMTruchet");
	ClassDB::register_class<MMSineWave>();
	MMAlgos::register_node_class("Patterns", "MMSineWave");
	ClassDB::register_class<MMScratches>();
	MMAlgos::register_node_class("Patterns", "MMScratches");
	ClassDB::register_class<MMRunes>();
	MMAlgos::register_node_class("Patterns", "MMRunes");
	ClassDB::register_class<MMPattern>();
	MMAlgos::register_node_class("Patterns", "MMPattern");
	ClassDB::register_class<MMIching>();
	MMAlgos::register_node_class("Patterns", "MMIching");
	ClassDB::register_class<MMBricks>();
	MMAlgos::register_node_class("Patterns", "MMBricks");
	ClassDB::register_class<MMBeehive>();
	MMAlgos::register_node_class("Patterns", "MMBeehive");

	ClassDB::register_class<MMOutputImage>();
	MMAlgos::register_node_class("Output", "MMOutputImage");

	ClassDB::register_class<MMVoronoi>();
	MMAlgos::register_node_class("Noise", "MMVoronoi");
	ClassDB::register_class<MMNoise>();
	MMAlgos::register_node_class("Noise", "MMNoise");
	ClassDB::register_class<MMFbmNoise>();
	MMAlgos::register_node_class("Noise", "MMFbmNoise");
	ClassDB::register_class<MMColorValue>();
	MMAlgos::register_node_class("Noise", "MMColorValue");
	ClassDB::register_class<MMColorNoise>();
	MMAlgos::register_node_class("Noise", "MMColorNoise");
	ClassDB::register_class<MMAnisotropicNoise>();
	MMAlgos::register_node_class("Noise", "MMAnisotropicNoise");

	ClassDB::register_class<MMRadialGradient>();
	MMAlgos::register_node_class("Gradient", "MMRadialGradient");
	ClassDB::register_class<MMGradient>();
	MMAlgos::register_node_class("Gradient", "MMGradient");
	ClassDB::register_class<MMCircularGradient>();
	MMAlgos::register_node_class("Gradient", "MMCircularGradient");

	ClassDB::register_class<MMTonality>();
	MMAlgos::register_node_class("Filter", "MMTonality");
	ClassDB::register_class<MMSwapChannels>();
	MMAlgos::register_node_class("Filter", "MMSwapChannels");
	ClassDB::register_class<MMQuantize>();
	MMAlgos::register_node_class("Filter", "MMQuantize");
	ClassDB::register_class<MMMath>();
	MMAlgos::register_node_class("Filter", "MMMath");
	ClassDB::register_class<MMMakeTileable>();
	MMAlgos::register_node_class("Filter", "MMMakeTileable");
	ClassDB::register_class<MMInvert>();
	MMAlgos::register_node_class("Filter", "MMInvert");
	ClassDB::register_class<MMGreyscale>();
	MMAlgos::register_node_class("Filter", "MMGreyscale");
	ClassDB::register_class<MMFillToUv>();
	MMAlgos::register_node_class("Filter", "MMFillToUv");
	ClassDB::register_class<MMFillToSize>();
	MMAlgos::register_node_class("Filter", "MMFillToSize");
	ClassDB::register_class<MMFillToRandomGrey>();
	MMAlgos::register_node_class("Filter", "MMFillToRandomGrey");
	ClassDB::register_class<MMFillToRandomColor>();
	MMAlgos::register_node_class("Filter", "MMFillToRandomColor");
	ClassDB::register_class<MMFillToPosition>();
	MMAlgos::register_node_class("Filter", "MMFillToPosition");
	ClassDB::register_class<MMFillToColor>();
	MMAlgos::register_node_class("Filter", "MMFillToColor");
	ClassDB::register_class<MMFillChannel>();
	MMAlgos::register_node_class("Filter", "MMFillChannel");
	ClassDB::register_class<MMEmboss>();
	MMAlgos::register_node_class("Filter", "MMEmboss");
	ClassDB::register_class<MMDecompose>();
	MMAlgos::register_node_class("Filter", "MMDecompose");
	ClassDB::register_class<MMCombine>();
	MMAlgos::register_node_class("Filter", "MMCombine");
	ClassDB::register_class<MMColorize>();
	MMAlgos::register_node_class("Filter", "MMColorize");
	ClassDB::register_class<MMBrightnessContrast>();
	MMAlgos::register_node_class("Filter", "MMBrightnessContrast");
	ClassDB::register_class<MMBlurGaussian>();
	MMAlgos::register_node_class("Filter", "MMBlurGaussian");
	ClassDB::register_class<MMBlend>();
	MMAlgos::register_node_class("Filter", "MMBlend");
	ClassDB::register_class<MMAdjustHsv>();
	MMAlgos::register_node_class("Filter", "MMAdjustHsv");

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
