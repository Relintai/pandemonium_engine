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
