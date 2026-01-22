
pandemonium_branch = 'master'

engine_repository = [ ['https://github.com/Relintai/pandemonium_engine.git', 'git@github.com:Relintai/pandemonium_engine.git'], 'pandemonium_engine', '' ]

# Relative to this script's directory
# The modules that get downloaded will be copied to this folder.
#
# Default.
module_install_folder = './pandemonium_engine/modules/'
# This folder is .gitignored by the engine. Make sure you use the alternative custom_module_folders setting.
#module_install_folder = './pandemonium_engine/custom_modules/'

module_repositories = [
    #[ ['https://github.com/Relintai/entity_spell_system.git', 'git@github.com:Relintai/entity_spell_system.git'], 'entity_spell_system', '' ],
    #[ ['https://github.com/Relintai/ui_extensions.git', 'git@github.com:Relintai/ui_extensions.git'], 'ui_extensions', '' ],
    #[ ['https://github.com/Relintai/texture_packer.git', 'git@github.com:Relintai/texture_packer.git'], 'texture_packer', '' ],
    #[ ['https://github.com/Relintai/godot_fastnoise.git', 'git@github.com:Relintai/godot_fastnoise.git'], 'fastnoise', '' ],
    #[ ['https://github.com/Relintai/mesh_data_resource.git', 'git@github.com:Relintai/mesh_data_resource.git'], 'mesh_data_resource', '' ],
    #[ ['https://github.com/Relintai/props.git', 'git@github.com:Relintai/props.git'], 'props', '' ],
    #[ ['https://github.com/Relintai/mesh_utils.git', 'git@github.com:Relintai/mesh_utils.git'], 'mesh_utils', '' ],
    #[ ['https://github.com/Relintai/broken_seals_module.git', 'git@github.com:Relintai/broken_seals_module.git'], 'broken_seals_module', '' ],
    #[ ['https://github.com/Relintai/thread_pool.git', 'git@github.com:Relintai/thread_pool.git'], 'thread_pool', '' ],
    #[ ['https://github.com/Relintai/terraman.git', 'git@github.com:Relintai/terraman.git'], 'terraman', '' ],
]

removed_modules = [
    #[ ['https://github.com/Relintai/voxelman.git', 'git@github.com:Relintai/voxelman.git'], 'voxelman', '' ],
    #[ ['https://github.com/Relintai/procedural_animations.git', 'git@github.com:Relintai/procedural_animations.git'], 'procedural_animations', '' ],
    #[ ['https://github.com/Relintai/world_generator.git', 'git@github.com:Relintai/world_generator.git'], 'world_generator', '' ],
]

addon_repositories = [
]

third_party_addon_repositories = [
]

# Relative to the engine directory
# if not an emtpy string this will be passed to the engine's build script
# as the custom_modules= parameter.
custom_module_folders = ""
# Use this if you are using the alternative .gitignored "custom_modules" module_install_folder
#custom_module_folders = "custom_modules"


# When compiling the slim editor ("_slim" build word)
# These commands will be passed to the build.
slim_args_editor = ''

# Normal Modules

#slim_args_editor += 'module_bmp_enabled=no '
#slim_args_editor += 'module_broken_seals_module_enabled=no '
#slim_args_editor += 'module_cscript_enabled=no '
#slim_args_editor += 'module_csg_enabled=no '
#slim_args_editor += 'module_database_enabled=no '
#slim_args_editor += 'module_database_sqlite_enabled=no '
#slim_args_editor += 'module_dds_enabled=no '
#slim_args_editor += 'module_enet_enabled=no '
#slim_args_editor += 'module_entity_spell_system_enabled=no '
#slim_args_editor += 'module_fastnoise_enabled=no ' 
#slim_args_editor += 'module_freetype_enabled=no ' # Module cannot be disabled with the tools (editor) build
#slim_args_editor += 'module_gdnative_enabled=no ' 
#slim_args_editor += 'module_gdscript_enabled=no ' 
#slim_args_editor += 'module_gridmap_enabled=no '
#slim_args_editor += 'module_hdr_enabled=no '
#slim_args_editor += 'module_http_server_simple_enabled=no '
#slim_args_editor += 'module_jpg_enabled=no '
#slim_args_editor += 'module_lz4_enabled=no '
#slim_args_editor += 'module_material_maker_enabled=no '
#slim_args_editor += 'module_mbedtls_enabled=no '
#slim_args_editor += 'module_mesh_data_resource_enabled=no '
#slim_args_editor += 'module_mesh_utils_enabled=no '
#slim_args_editor += 'module_minimp3_enabled=no '
#slim_args_editor += 'module_navigation_enabled=no ' # The default (working) navigationserver backend. Currently at least one navigation server backend need to be present, so keep at least one active.
#slim_args_editor += 'module_navigation_dummy_enabled=no ' # The dummy navigationserver backend.
#slim_args_editor += 'module_navigation_geometry_parsers_enabled=no '
#slim_args_editor += 'module_navigation_mesh_generator_enabled=no '
#slim_args_editor += 'module_network_synchronizer_enabled=no '
#slim_args_editor += 'module_ogg_enabled=no '
#slim_args_editor += 'module_opensimplex_enabled=no '
#slim_args_editor += 'module_opus_enabled=no '
#slim_args_editor += 'module_paint_enabled=no '
#slim_args_editor += 'module_procedural_tree_3d_enabled=no '
#slim_args_editor += 'module_props_enabled=no '
#slim_args_editor += 'module_props_2d_enabled=no '
#slim_args_editor += 'module_pvr_enabled=no '
#slim_args_editor += 'module_regex_enabled=no ' # Module cannot be disabled with the tools (editor) build
#slim_args_editor += 'module_skeleton_2d_enabled=no '
#slim_args_editor += 'module_skeleton_3d_enabled=no '
#slim_args_editor += 'module_smtp_enabled=no '
#slim_args_editor += 'module_squish_enabled=no '
#slim_args_editor += 'module_stb_vorbis_enabled=no '
#slim_args_editor += 'module_steering_ai_enabled=no '
#slim_args_editor += 'module_svg_enabled=no ' # If module is disabled in editor builds, editor icons won't appear.
#slim_args_editor += 'module_terraman_enabled=no '
#slim_args_editor += 'module_terraman_2d_enabled=no '
#slim_args_editor += 'module_texture_packer_enabled=no '
#slim_args_editor += 'module_tga_enabled=no '
#slim_args_editor += 'module_theora_enabled=no '
#slim_args_editor += 'module_tile_map_enabled=no '
#slim_args_editor += 'module_ui_extensions_enabled=no '
#slim_args_editor += 'module_unit_test_enabled=no '
#slim_args_editor += 'module_upnp_enabled=no '
#slim_args_editor += 'module_users_enabled=no '
#slim_args_editor += 'module_vhacd_enabled=no '
#slim_args_editor += 'module_vorbis_enabled=no '
#slim_args_editor += 'module_voxelman_enabled=no '
#slim_args_editor += 'module_web_enabled=no '
#slim_args_editor += 'module_websocket_enabled=no '
#slim_args_editor += 'module_wfc_enabled=no '
#slim_args_editor += 'module_jsonrpc_enabled=no ' # JSONRPC implementation. Used by gdscript's and pscript's lsp (when it provides autocomplete for an another ide).
#slim_args_editor += 'module_layered_tile_map_enabled=no '
#slim_args_editor += 'module_totp_enabled=no '
#slim_args_editor += 'module_vertex_lights_2d_enabled=no '
#slim_args_editor += 'module_vertex_lights_3d_enabled=no '
#slim_args_editor += 'module_xatlas_unwrap_enabled=no ' # GLES3 lihgtmapper
#slim_args_editor += 'module_camera_server_enabled=no '
#slim_args_editor += 'module_camera_drivers_enabled=no ' # Depends on the camera server module.
#slim_args_editor += 'module_pscript_enabled=no '

# Editor Modules

#slim_args_editor += 'module_cvtt_enabled=no '
#slim_args_editor += 'module_editor_code_editor_enabled=no '
#slim_args_editor += 'module_etc_enabled=no '
#slim_args_editor += 'module_gltf_enabled=no '
#slim_args_editor += 'module_plugin_refresher_enabled=no '
#slim_args_editor += 'module_shader_editor_enabled=no '
#slim_args_editor += 'module_text_editor_enabled=no '
#slim_args_editor += 'module_tinyexr_enabled=no '
#slim_args_editor += 'module_denoise_enabled=no ' # GLES3 lihgtmapper
#slim_args_editor += 'module_lightmapper_cpu_enabled=no ' # GLES3 lihgtmapper 
#slim_args_editor += 'module_raycast_enabled=no ' # GLES3 lihgtmapper 
#slim_args_editor += 'module_fbx_enabled=no '

#slim_args_editor += 'disable_advanced_gui=yes ' # Cannot be used with the tools (editor) build
#slim_args_editor += 'disable_3d=yes ' # Cannot be used with the tools (editor) build
#slim_args_editor += 'disable_gles3=yes '

# When compiling the slim templates ("_slim" build word)
# These commands will be passed to the build.
slim_args = slim_args_editor

