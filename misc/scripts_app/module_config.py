
pandemonium_branch = 'master'

engine_repository = [ ['https://github.com/Relintai/pandemonium_engine.git', 'git@github.com:Relintai/pandemonium_engine.git'], 'pandemonium_engine', '' ]

# Relative to this script's directory
module_install_folder = './pandemonium_engine/modules/'

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
custom_module_folders = ""

slim_args = ''

# Normal Modules

#slim_args += 'module_bmp_enabled=no '
#slim_args += 'module_broken_seals_module_enabled=no '
#slim_args += 'module_cscript_enabled=no '
#slim_args += 'module_csg_enabled=no '
#slim_args += 'module_database_enabled=no '
#slim_args += 'module_database_sqlite_enabled=no '
#slim_args += 'module_dds_enabled=no '
#slim_args += 'module_enet_enabled=no '
#slim_args += 'module_entity_spell_system_enabled=no '
#slim_args += 'module_fastnoise_enabled=no ' 
#slim_args += 'module_freetype_enabled=no ' # Module cannot be disabled with the tools (editor) build
#slim_args += 'module_gdnative_enabled=no ' 
#slim_args += 'module_gdscript_enabled=no ' 
#slim_args += 'module_gridmap_enabled=no '
#slim_args += 'module_hdr_enabled=no '
#slim_args += 'module_http_server_simple_enabled=no '
#slim_args += 'module_jpg_enabled=no '
#slim_args += 'module_lz4_enabled=no '
#slim_args += 'module_material_maker_enabled=no '
#slim_args += 'module_mbedtls_enabled=no '
#slim_args += 'module_mesh_data_resource_enabled=no '
#slim_args += 'module_mesh_utils_enabled=no '
#slim_args += 'module_minimp3_enabled=no '
#slim_args += 'module_navigation_enabled=no ' # The default (working) navigationserver backend. Currently at least one navigation server backend need to be present, so keep at least one active.
#slim_args += 'module_navigation_dummy_enabled=no ' # The dummy navigationserver backend.
#slim_args += 'module_navigation_geometry_parsers_enabled=no '
#slim_args += 'module_navigation_mesh_generator_enabled=no '
#slim_args += 'module_network_synchronizer_enabled=no '
#slim_args += 'module_ogg_enabled=no '
#slim_args += 'module_opensimplex_enabled=no '
#slim_args += 'module_opus_enabled=no '
#slim_args += 'module_paint_enabled=no '
#slim_args += 'module_procedural_tree_3d_enabled=no '
#slim_args += 'module_props_enabled=no '
#slim_args += 'module_props_2d_enabled=no '
#slim_args += 'module_pvr_enabled=no '
#slim_args += 'module_regex_enabled=no ' # Module cannot be disabled with the tools (editor) build
#slim_args += 'module_skeleton_2d_enabled=no '
#slim_args += 'module_skeleton_3d_enabled=no '
#slim_args += 'module_smtp_enabled=no '
#slim_args += 'module_squish_enabled=no '
#slim_args += 'module_stb_vorbis_enabled=no '
#slim_args += 'module_steering_ai_enabled=no '
#slim_args += 'module_svg_enabled=no ' # If module is disabled in editor builds, editor icons won't appear.
#slim_args += 'module_terraman_enabled=no '
#slim_args += 'module_terraman_2d_enabled=no '
#slim_args += 'module_texture_packer_enabled=no '
#slim_args += 'module_tga_enabled=no '
#slim_args += 'module_theora_enabled=no '
#slim_args += 'module_tile_map_enabled=no '
#slim_args += 'module_ui_extensions_enabled=no '
#slim_args += 'module_unit_test_enabled=no '
#slim_args += 'module_upnp_enabled=no '
#slim_args += 'module_users_enabled=no '
#slim_args += 'module_vhacd_enabled=no '
#slim_args += 'module_vorbis_enabled=no '
#slim_args += 'module_voxelman_enabled=no '
#slim_args += 'module_web_enabled=no '
#slim_args += 'module_websocket_enabled=no '
#slim_args += 'module_wfc_enabled=no '
#slim_args += 'module_jsonrpc_enabled=no ' # JSONRPC implementation. Currently only used by gdscript's lsp (when it provides autocomplete for an another ide).
#slim_args += 'module_layered_tile_map_enabled=no '
#slim_args += 'module_totp_enabled=no '
#slim_args += 'module_vertex_lights_2d_enabled=no '
#slim_args += 'module_vertex_lights_3d_enabled=no '
#slim_args += 'module_xatlas_unwrap_enabled=no ' # GLES3 lihgtmapper

# Editor Modules

#slim_args += 'module_cvtt_enabled=no '
#slim_args += 'module_editor_code_editor_enabled=no '
#slim_args += 'module_etc_enabled=no '
#slim_args += 'module_gltf_enabled=no '
#slim_args += 'module_plugin_refresher_enabled=no '
#slim_args += 'module_shader_editor_enabled=no '
#slim_args += 'module_text_editor_enabled=no '
#slim_args += 'module_tinyexr_enabled=no '
#slim_args += 'module_denoise_enabled=no ' # GLES3 lihgtmapper
#slim_args += 'module_lightmapper_cpu_enabled=no ' # GLES3 lihgtmapper 
#slim_args += 'module_raycast_enabled=no ' # GLES3 lihgtmapper 
#slim_args += 'module_fbx_enabled=no '

#slim_args += 'disable_advanced_gui=yes ' # Cannot be used with the tools (editor) build
#slim_args += 'disable_3d=yes ' # Cannot be used with the tools (editor) build
#slim_args += 'disable_gles3=yes '

slim_args_editor = slim_args
