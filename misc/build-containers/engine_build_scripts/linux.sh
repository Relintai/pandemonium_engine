#!/bin/bash
set -e

export PATH="${GODOT_SDK_LINUX_X86_64}/bin:${BASE_PATH}"

export HTTP_SERVER_COMMANDS=" module_bmp_enabled=no module_broken_seals_module_enabled=no module_cscript_enabled=no module_csg_enabled=no module_dds_enabled=no module_enet_enabled=no module_entity_spell_system_enabled=no module_fastnoise_enabled=no module_freetype_enabled=no module_gridmap_enabled=no module_hdr_enabled=no module_layered_tile_map_enabled=no module_material_maker_enabled=no module_mesh_data_resource_enabled=no module_mesh_utils_enabled=no module_minimp3_enabled=no module_navigation_enabled=no module_navigation_geometry_parsers_enabled=no module_network_synchronizer_enabled=no module_ogg_enabled=no module_opensimplex_enabled=no module_opus_enabled=no module_paint_enabled=no module_navigation_mesh_generator_enabled=no module_props_enabled=no module_props_2d_enabled=no module_pvr_enabled=no module_regex_enabled=no module_skeleton_2d_enabled=no module_skeleton_3d_enabled=no module_squish_enabled=no module_stb_vorbis_enabled=no module_steering_ai_enabled=no module_terraman_enabled=no module_terraman_2d_enabled=no module_texture_packer_enabled=no module_tga_enabled=no module_theora_enabled=no module_tile_map_enabled=no module_ui_extensions_enabled=no module_unit_test_enabled=no module_upnp_enabled=no module_vertex_lights_2d_enabled=no module_vertex_lights_3d_enabled=no module_vhacd_enabled=no module_vorbis_enabled=no module_voxelman_enabled=no module_wfc_enabled=no module_cvtt_enabled=no module_editor_code_editor_enabled=no module_etc_enabled=no module_gltf_enabled=no module_plugin_refresher_enabled=no module_shader_editor_enabled=no module_text_editor_enabled=no module_tinyexr_enabled=no disable_advanced_gui=yes disable_3d=yes "

# Linux http server (templates) 64bit
scons production=yes ${HTTP_SERVER_COMMANDS} tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=server bits=64 "$@"
scons ${HTTP_SERVER_COMMANDS} tools=no target=release custom_modules_shared=no debug_symbols=no platform=server bits=64 "$@"

mv -f bin/pandemonium_server.x11.opt.64 bin/pandemonium_http_server.x11.opt.64
mv -f bin/pandemonium_server.x11.opt.debug.64 bin/pandemonium_http_server.x11.opt.debug.64

# Linux editor 64 bit
scons production=yes tools=yes target=release_debug custom_modules_shared=no debug_symbols=no platform=x11 bits=64 "$@"

# Linux templates 64 bit
scons production=yes tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=x11 bits=64 "$@"
scons production=yes tools=no target=release custom_modules_shared=no debug_symbols=no platform=x11 bits=64 "$@"

# Linux headless (editor) 64 bit
scons production=yes tools=yes target=release_debug custom_modules_shared=no debug_symbols=no platform=server bits=64 "$@"

# Linux server (templates) 64 bit
scons production=yes tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=server bits=64 "$@"
scons production=yes tools=no target=release custom_modules_shared=no debug_symbols=no platform=server bits=64 "$@"

export PATH="${GODOT_SDK_LINUX_X86_32}/bin:${BASE_PATH}"

# Linux http server (templates) 32 bit
scons production=yes ${HTTP_SERVER_COMMANDS} tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=server bits=32 "$@"
scons ${HTTP_SERVER_COMMANDS} tools=no target=release custom_modules_shared=no debug_symbols=no platform=server bits=32 "$@"

mv -f bin/pandemonium_server.x11.opt.32 bin/pandemonium_http_server.x11.opt.32
mv -f bin/pandemonium_server.x11.opt.debug.32 bin/pandemonium_http_server.x11.opt.debug.32

# Linux editor 32 bit
scons production=yes tools=yes target=release_debug custom_modules_shared=no debug_symbols=no platform=x11 bits=32 "$@"

# Linux templates 32 bit
scons production=yes tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=x11 bits=32 "$@"
scons production=yes tools=no target=release custom_modules_shared=no debug_symbols=no platform=x11 bits=32 "$@"

# Linux headless (editor) 32 bit
scons production=yes tools=yes target=release_debug custom_modules_shared=no debug_symbols=no platform=server bits=32 "$@"

# Linux server (templates) 32 bit
scons production=yes tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=server bits=32 "$@"
scons production=yes tools=no target=release custom_modules_shared=no debug_symbols=no platform=server bits=32 "$@"

export PATH="${GODOT_SDK_LINUX_ARM64}/bin:${BASE_PATH}"

# Linux http server (templates) arm64
scons production=yes ${HTTP_SERVER_COMMANDS} tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=server arch=arm64 "$@"
scons ${HTTP_SERVER_COMMANDS} tools=no target=release custom_modules_shared=no debug_symbols=no platform=server arch=arm64 "$@"

mv -f bin/pandemonium_server.x11.opt.arm64 bin/pandemonium_http_server.x11.opt.arm64
mv -f bin/pandemonium_server.x11.opt.debug.arm64 bin/pandemonium_http_server.x11.opt.debug.arm64

# Linux editor arm64
scons production=yes tools=yes target=release_debug custom_modules_shared=no debug_symbols=no platform=x11 arch=arm64 "$@"

# Linux templates arm64
scons production=yes tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=x11 arch=arm64 "$@"
scons production=yes tools=no target=release custom_modules_shared=no debug_symbols=no platform=x11 arch=arm64 "$@"

# Linux headless (editor) arm64
scons production=yes tools=yes target=release_debug custom_modules_shared=no debug_symbols=no platform=server arch=arm64 "$@"

# Linux server (templates) arm64
scons production=yes tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=server arch=arm64 "$@"
scons production=yes tools=no target=release custom_modules_shared=no debug_symbols=no platform=server arch=arm64 "$@"

export PATH="${GODOT_SDK_LINUX_ARM32}/bin:${BASE_PATH}"

# Linux http server (templates) arm64
scons production=yes ${HTTP_SERVER_COMMANDS} tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=server arch=arm "$@"
scons ${HTTP_SERVER_COMMANDS} tools=no target=release custom_modules_shared=no debug_symbols=no platform=server arch=arm "$@"

mv -f bin/pandemonium_server.x11.opt.arm bin/pandemonium_http_server.x11.opt.arm
mv -f bin/pandemonium_server.x11.opt.debug.arm bin/pandemonium_http_server.x11.opt.debug.arm

# Linux editor arm
scons production=yes tools=yes target=release_debug custom_modules_shared=no debug_symbols=no platform=x11 arch=arm "$@"

# Linux templates arm
scons production=yes tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=x11 arch=arm "$@"
scons production=yes tools=no target=release custom_modules_shared=no debug_symbols=no platform=x11 arch=arm "$@"

# Linux headless (editor) arm
scons production=yes tools=yes target=release_debug custom_modules_shared=no debug_symbols=no platform=server arch=arm "$@"

# Linux server (templates) arm
scons production=yes tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=server arch=arm "$@"
scons production=yes tools=no target=release custom_modules_shared=no debug_symbols=no platform=server arch=arm "$@"

