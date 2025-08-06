/**************************************************************************/
/*  fbx_material.h                                                        */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#ifndef FBX_MATERIAL_H
#define FBX_MATERIAL_H

#include "tools/import_utils.h"

#include "core/reference.h"
#include "core/ustring.h"

struct FBXMaterial : public Reference {
	String material_name = String();
	bool warning_non_pbr_material = false;
	FBXDocParser::Material *material = nullptr;

	/* Godot materials
	 *** Texture Maps:
	 * Albedo - color, texture
	 * Metallic - specular, metallic, texture
	 * Roughness - roughness, texture
	 * Emission - color, texture
	 * Normal Map - scale, texture
	 * Ambient Occlusion - texture
	 * Refraction - scale, texture
	 *** Has Settings for:
	 * UV1 - SCALE, OFFSET
	 * UV2 - SCALE, OFFSET
	 *** Flags for
	 * Transparent
	 * Cull Mode
	 */

	enum class MapMode {
		AlbedoM = 0,
		MetallicM,
		SpecularM,
		EmissionM,
		RoughnessM,
		NormalM,
		AmbientOcclusionM,
		RefractionM,
		ReflectionM,
	};

	/* Returns the string representation of the TextureParam enum */
	static String get_texture_param_name(Material3D::TextureParam param) {
		switch (param) {
			case Material3D::TEXTURE_ALBEDO:
				return "TEXTURE_ALBEDO";
			case Material3D::TEXTURE_METALLIC:
				return "TEXTURE_METALLIC";
			case Material3D::TEXTURE_ROUGHNESS:
				return "TEXTURE_ROUGHNESS";
			case Material3D::TEXTURE_EMISSION:
				return "TEXTURE_EMISSION";
			case Material3D::TEXTURE_NORMAL:
				return "TEXTURE_NORMAL";
			case Material3D::TEXTURE_RIM:
				return "TEXTURE_RIM";
			case Material3D::TEXTURE_CLEARCOAT:
				return "TEXTURE_CLEARCOAT";
			case Material3D::TEXTURE_FLOWMAP:
				return "TEXTURE_FLOWMAP";
			case Material3D::TEXTURE_AMBIENT_OCCLUSION:
				return "TEXTURE_AMBIENT_OCCLUSION";
			case Material3D::TEXTURE_DEPTH:
				return "TEXTURE_DEPTH";
			case Material3D::TEXTURE_SUBSURFACE_SCATTERING:
				return "TEXTURE_SUBSURFACE_SCATTERING";
			case Material3D::TEXTURE_TRANSMISSION:
				return "TEXTURE_TRANSMISSION";
			case Material3D::TEXTURE_REFRACTION:
				return "TEXTURE_REFRACTION";
			case Material3D::TEXTURE_DETAIL_MASK:
				return "TEXTURE_DETAIL_MASK";
			case Material3D::TEXTURE_DETAIL_ALBEDO:
				return "TEXTURE_DETAIL_ALBEDO";
			case Material3D::TEXTURE_DETAIL_NORMAL:
				return "TEXTURE_DETAIL_NORMAL";
			case Material3D::TEXTURE_MAX:
				return "TEXTURE_MAX";
			default:
				return "broken horribly";
		}
	};

	// TODO make this static?
	const std::map<std::string, Material3D::Feature> fbx_feature_mapping_desc = {
		/* Transparent */
		{ "TransparentColor", Material3D::Feature::FEATURE_TRANSPARENT },
		{ "Maya|opacity", Material3D::Feature::FEATURE_TRANSPARENT }
	};

	// TODO make this static?
	const std::map<std::string, Material3D::TextureParam> fbx_texture_mapping_desc = {
		/* Diffuse */
		{ "Maya|base", Material3D::TextureParam::TEXTURE_ALBEDO },
		{ "DiffuseColor", Material3D::TextureParam::TEXTURE_ALBEDO },
		{ "Maya|DiffuseTexture", Material3D::TextureParam::TEXTURE_ALBEDO },
		{ "Maya|baseColor", Material3D::TextureParam::TEXTURE_ALBEDO },
		{ "Maya|baseColor|file", Material3D::TextureParam::TEXTURE_ALBEDO },
		{ "3dsMax|Parameters|base_color_map", Material3D::TextureParam::TEXTURE_ALBEDO },
		{ "Maya|TEX_color_map|file", Material3D::TextureParam::TEXTURE_ALBEDO },
		{ "Maya|TEX_color_map", Material3D::TextureParam::TEXTURE_ALBEDO },
		/* Emission */
		{ "EmissiveColor", Material3D::TextureParam::TEXTURE_EMISSION },
		{ "EmissiveFactor", Material3D::TextureParam::TEXTURE_EMISSION },
		{ "Maya|emissionColor", Material3D::TextureParam::TEXTURE_EMISSION },
		{ "Maya|emissionColor|file", Material3D::TextureParam::TEXTURE_EMISSION },
		{ "3dsMax|Parameters|emission_map", Material3D::TextureParam::TEXTURE_EMISSION },
		{ "Maya|TEX_emissive_map", Material3D::TextureParam::TEXTURE_EMISSION },
		{ "Maya|TEX_emissive_map|file", Material3D::TextureParam::TEXTURE_EMISSION },
		/* Metallic */
		{ "Maya|metalness", Material3D::TextureParam::TEXTURE_METALLIC },
		{ "Maya|metalness|file", Material3D::TextureParam::TEXTURE_METALLIC },
		{ "3dsMax|Parameters|metalness_map", Material3D::TextureParam::TEXTURE_METALLIC },
		{ "Maya|TEX_metallic_map", Material3D::TextureParam::TEXTURE_METALLIC },
		{ "Maya|TEX_metallic_map|file", Material3D::TextureParam::TEXTURE_METALLIC },
		{ "SpecularColor", Material3D::TextureParam::TEXTURE_METALLIC },
		{ "Maya|specularColor", Material3D::TextureParam::TEXTURE_METALLIC },
		{ "Maya|SpecularTexture", Material3D::TextureParam::TEXTURE_METALLIC },
		{ "Maya|SpecularTexture|file", Material3D::TextureParam::TEXTURE_METALLIC },

		/* Roughness */
		// Arnold Roughness Map
		{ "Maya|specularRoughness", Material3D::TextureParam::TEXTURE_ROUGHNESS },

		{ "3dsMax|Parameters|roughness_map", Material3D::TextureParam::TEXTURE_ROUGHNESS },
		{ "Maya|TEX_roughness_map", Material3D::TextureParam::TEXTURE_ROUGHNESS },
		{ "Maya|TEX_roughness_map|file", Material3D::TextureParam::TEXTURE_ROUGHNESS },

		/* Normal */
		{ "NormalMap", Material3D::TextureParam::TEXTURE_NORMAL },
		//{ "Bump", Material3D::TextureParam::TEXTURE_NORMAL },
		//{ "3dsMax|Parameters|bump_map", Material3D::TextureParam::TEXTURE_NORMAL },
		{ "Maya|NormalTexture", Material3D::TextureParam::TEXTURE_NORMAL },
		//{ "Maya|normalCamera", Material3D::TextureParam::TEXTURE_NORMAL },
		//{ "Maya|normalCamera|file", Material3D::TextureParam::TEXTURE_NORMAL },
		{ "Maya|TEX_normal_map", Material3D::TextureParam::TEXTURE_NORMAL },
		{ "Maya|TEX_normal_map|file", Material3D::TextureParam::TEXTURE_NORMAL },
		/* AO */
		{ "Maya|TEX_ao_map", Material3D::TextureParam::TEXTURE_AMBIENT_OCCLUSION },
		{ "Maya|TEX_ao_map|file", Material3D::TextureParam::TEXTURE_AMBIENT_OCCLUSION },

		//{ "Maya|diffuseRoughness", Material3D::TextureParam::UNSUPPORTED },
		//{ "Maya|diffuseRoughness|file", Material3D::TextureParam::UNSUPPORTED },
		//{ "ShininessExponent", Material3D::TextureParam::UNSUPPORTED },
		//{ "ReflectionFactor", Material3D::TextureParam::UNSUPPORTED },
		//{ "TransparentColor",Material3D::TextureParam::TEXTURE_CHANNEL_ALPHA },
		//{ "TransparencyFactor",Material3D::TextureParam::TEXTURE_CHANNEL_ALPHA }
	};

	// TODO make this static?
	enum PropertyDesc {
		PROPERTY_DESC_NOT_FOUND,
		PROPERTY_DESC_ALBEDO_COLOR,
		PROPERTY_DESC_TRANSPARENT,
		PROPERTY_DESC_METALLIC,
		PROPERTY_DESC_ROUGHNESS,
		PROPERTY_DESC_SPECULAR,
		PROPERTY_DESC_SPECULAR_COLOR,
		PROPERTY_DESC_SHINYNESS,
		PROPERTY_DESC_COAT,
		PROPERTY_DESC_COAT_ROUGHNESS,
		PROPERTY_DESC_EMISSIVE,
		PROPERTY_DESC_EMISSIVE_COLOR,
		PROPERTY_DESC_IGNORE
	};

	const std::map<std::string, PropertyDesc> fbx_properties_desc = {
		/* Albedo */
		{ "DiffuseColor", PROPERTY_DESC_ALBEDO_COLOR },
		{ "Maya|baseColor", PROPERTY_DESC_ALBEDO_COLOR },

		/* Specular */
		{ "Maya|specular", PROPERTY_DESC_SPECULAR },
		{ "Maya|specularColor", PROPERTY_DESC_SPECULAR_COLOR },

		/* Specular roughness - arnold roughness map */
		{ "Maya|specularRoughness", PROPERTY_DESC_ROUGHNESS },

		/* Transparent */
		{ "Opacity", PROPERTY_DESC_TRANSPARENT },
		{ "TransparencyFactor", PROPERTY_DESC_TRANSPARENT },
		{ "Maya|opacity", PROPERTY_DESC_TRANSPARENT },

		/* Metallic */
		{ "Shininess", PROPERTY_DESC_METALLIC },
		{ "Reflectivity", PROPERTY_DESC_METALLIC },
		{ "Maya|metalness", PROPERTY_DESC_METALLIC },
		{ "Maya|metallic", PROPERTY_DESC_METALLIC },

		/* Roughness */
		{ "Maya|roughness", PROPERTY_DESC_ROUGHNESS },

		/* Coat */
		//{ "Maya|coat", PROPERTY_DESC_COAT },

		/* Coat roughness */
		//{ "Maya|coatRoughness", PROPERTY_DESC_COAT_ROUGHNESS },

		/* Emissive */
		{ "Maya|emission", PROPERTY_DESC_EMISSIVE },
		{ "Maya|emissive", PROPERTY_DESC_EMISSIVE },

		/* Emissive color */
		{ "EmissiveColor", PROPERTY_DESC_EMISSIVE_COLOR },
		{ "Maya|emissionColor", PROPERTY_DESC_EMISSIVE_COLOR },

		/* Ignore */
		{ "Maya|diffuseRoughness", PROPERTY_DESC_IGNORE },
		{ "Maya", PROPERTY_DESC_IGNORE },
		{ "Diffuse", PROPERTY_DESC_ALBEDO_COLOR },
		{ "Maya|TypeId", PROPERTY_DESC_IGNORE },
		{ "Ambient", PROPERTY_DESC_IGNORE },
		{ "AmbientColor", PROPERTY_DESC_IGNORE },
		{ "ShininessExponent", PROPERTY_DESC_IGNORE },
		{ "Specular", PROPERTY_DESC_IGNORE },
		{ "SpecularColor", PROPERTY_DESC_IGNORE },
		{ "SpecularFactor", PROPERTY_DESC_IGNORE },
		//{ "BumpFactor", PROPERTY_DESC_IGNORE },
		{ "Maya|exitToBackground", PROPERTY_DESC_IGNORE },
		{ "Maya|indirectDiffuse", PROPERTY_DESC_IGNORE },
		{ "Maya|indirectSpecular", PROPERTY_DESC_IGNORE },
		{ "Maya|internalReflections", PROPERTY_DESC_IGNORE },
		{ "DiffuseFactor", PROPERTY_DESC_IGNORE },
		{ "AmbientFactor", PROPERTY_DESC_IGNORE },
		{ "ReflectionColor", PROPERTY_DESC_IGNORE },
		{ "Emissive", PROPERTY_DESC_IGNORE },
		{ "Maya|coatColor", PROPERTY_DESC_IGNORE },
		{ "Maya|coatNormal", PROPERTY_DESC_IGNORE },
		{ "Maya|coatIOR", PROPERTY_DESC_IGNORE },
	};

	struct TextureFileMapping {
		Material3D::TextureParam map_mode = Material3D::TEXTURE_ALBEDO;
		String name = String();
		const FBXDocParser::Texture *texture = nullptr;
	};

	/* storing the texture properties like color */
	template <class T>
	struct TexturePropertyMapping : Reference {
		Material3D::TextureParam map_mode = Material3D::TextureParam::TEXTURE_ALBEDO;
		const T property = T();
	};

	static void add_search_string(String p_filename, String p_current_directory, String search_directory, Vector<String> &texture_search_paths);

	static String find_texture_path_by_filename(const String p_filename, const String p_current_directory);

	String get_material_name() const;

	void set_imported_material(FBXDocParser::Material *p_material);

	struct MaterialInfo {
		Vector<TextureFileMapping> textures;
		Vector<Material3D::Feature> features;
	};
	/// Extracts the material information.
	MaterialInfo extract_material_info(const FBXDocParser::Material *material) const;

	Ref<Material3D> import_material(ImportState &state);
};

#endif // FBX_MATERIAL_H
