#!/bin/bash
set -e

# pkg-config wrongly points to lib instead of lib64 for arch-dependent header.
sed -i ${GODOT_SDK_LINUX_X86_64}/x86_64-godot-linux-gnu/sysroot/usr/lib/pkgconfig/dbus-1.pc -e "s@/lib@/lib64@g"

export PATH="${GODOT_SDK_LINUX_X86_64}/bin:${BASE_PATH}"

# Linux editor 64 bit
scons tools=yes target=release_debug custom_modules_shared=no debug_symbols=no platform=x11 bits=64 "$@" . 2>&1 | tee logs/linux_ed_64.log

# Linux templates 64 bit
scons tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=x11 bits=64 "$@" . 2>&1 | tee logs/linux_template_rd_64.log
scons tools=no target=release custom_modules_shared=no debug_symbols=no platform=x11 bits=64 "$@" . 2>&1 | tee logs/linux_template_r_64.log

# Linux headless (editor) 64bit
scons tools=yes target=release_debug custom_modules_shared=no debug_symbols=no platform=server bits=64 "$@" . 2>&1 | tee logs/headless.log

# Linux server (templates) 64bit
scons tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=server bits=64 "$@" . 2>&1 | tee logs/server_rd.log
scons tools=no target=release custom_modules_shared=no debug_symbols=no platform=server bits=64 "$@" . 2>&1 | tee logs/server_d.log

export PATH="${GODOT_SDK_LINUX_X86}/bin:${BASE_PATH}"

# Linux editor 32 bit
scons tools=yes target=release_debug custom_modules_shared=no debug_symbols=no platform=x11 bits=32 "$@" . 2>&1 | tee logs/linux_ed_32.log

# Linux templates 32 bit
scons tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=x11 bits=32 "$@" . 2>&1 | tee logs/linux_template_rd_32.log
scons tools=no target=release custom_modules_shared=no debug_symbols=no platform=x11 bits=32 "$@" . 2>&1 | tee logs/linux_template_r_32.log

export PATH="${GODOT_SDK_LINUX_ARMHF}/bin:${BASE_PATH}"

# Linux editor armhf (PI4)
scons tools=yes target=release_debug custom_modules_shared=no debug_symbols=no platform=x11 arch=armv7 CCFLAGS="-mcpu=cortex-a7 -mfpu=neon-vfpv4 -mfloat-abi=hard -mlittle-endian -munaligned-access" "$@" . 2>&1 | tee logs/linux_ed_armhf.log

# Linux templates armhf
scons tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=x11 arch=armv7 CCFLAGS="-mcpu=cortex-a7 -mfpu=neon-vfpv4 -mfloat-abi=hard -mlittle-endian -munaligned-access" "$@" . 2>&1 | tee logs/linux_template_rd_armhf.log
scons tools=no target=release custom_modules_shared=no debug_symbols=no platform=x11 arch=armv7 CCFLAGS="-mcpu=cortex-a7 -mfpu=neon-vfpv4 -mfloat-abi=hard -mlittle-endian -munaligned-access" "$@" . 2>&1 | tee logs/linux_template_r_armhf.log

# TODO add a way to postfix generated executable names

#export HTTP_SERVER_COMMANDS=" module_bmp_enabled=no module_broken_seals_module_enabled=no module_cscript_enabled=no module_dds_enabled=no module_enet_enabled=no module_entity_spell_system_enabled=no module_fastnoise_enabled=no module_freetype_enabled=no module_gridmap_enabled=no module_hdr_enabled=no module_jpg_enabled=no module_lz4_enabled=no module_material_maker_enabled=no module_mesh_data_resource_enabled=no module_mesh_utils_enabled=no module_minimp3_enabled=no module_navigation_enabled=no module_network_synchronizer_enabled=no module_ogg_enabled=no module_opensimplex_enabled=no module_opus_enabled=no module_paint_enabled=no module_props_enabled=no module_props_2d_enabled=no module_pvr_enabled=no module_regex_enabled=no module_skeleton_2d_enabled=no module_skeleton_3d_enabled=no module_squish_enabled=no module_stb_vorbis_enabled=no module_steering_ai_enabled=no module_svg_enabled=no module_terraman_enabled=no module_terraman_2d_enabled=no module_texture_packer_enabled=no module_tga_enabled=no module_theora_enabled=no module_tile_map_enabled=no module_ui_extensions_enabled=no module_unit_test_enabled=no module_upnp_enabled=no module_vhacd_enabled=no module_vorbis_enabled=no module_voxelman_enabled=no module_websocket_enabled=no module_wfc_enabled=no module_cvtt_enabled=no module_editor_code_editor_enabled=no module_etc_enabled=no module_gltf_enabled=no module_plugin_refresher_enabled=no module_shader_editor_enabled=no module_text_editor_enabled=no module_tinyexr_enabled=no disable_advanced_gui=yes disable_3d=yes "

# Linux http server (templates) 64bit
#scons ${HTTP_SERVER_COMMANDS} tools=no target=release_debug custom_modules_shared=no debug_symbols=no platform=server bits=64 "$@" . 2>&1 | tee logs/server_rd_http.log
#scons ${HTTP_SERVER_COMMANDS} tools=no target=release custom_modules_shared=no debug_symbols=no platform=server bits=64 "$@" . 2>&1 | tee logs/server_d_http.log

# Rename armhf to pi4
# Todo think about a better naming convention, especially if FRT is added to the builds as well

mv -f bin/pandemonium.x11.opt.armv7 bin/pandemonium.x11.opt.pi4
mv -f bin/pandemonium.x11.opt.debug.armv7 bin/pandemonium.x11.opt.debug.pi4
mv -f bin/pandemonium.x11.opt.tools.armv7 bin/pandemonium.x11.opt.tools.pi4
