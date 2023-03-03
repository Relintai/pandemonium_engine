
#include "mm_algos.h"

#include "core/math/math_funcs.h"
#include "core/math/vector4.h"

//pattern.mmg;
//----------------------;
//hlsl_defs.tmpl;
////define hlsl_atan(x,y) atan2(x, y);
////define mod(x,y) ((x)-(y)*floor((x)/(y)));
//inline float4 textureLod(sampler2D tex, float2 uv, float lod) {;
//	return tex2D(tex, uv);
//};
//inline float2 tofloat2(float x) {;
//	return float2(x, x);
//};
//inline float2 tofloat2(float x, float y) {;
//	return float2(x, y);
//};
//inline float3 tofloat3(float x) {;
//	return float3(x, x, x);
//};
//inline float3 tofloat3(float x, float y, float z) {;
//	return float3(x, y, z);
//};
//inline float3 tofloat3(float2 xy, float z) {;
//	return float3(xy.x, xy.y, z);
//};
//inline float3 tofloat3(float x, float2 yz) {;
//	return float3(x, yz.x, yz.y);
//};
//inline float4 tofloat4(float x, float y, float z, float w) {;
//	return float4(x, y, z, w);
//};
//inline float4 tofloat4(float x) {;
//	return float4(x, x, x, x);
//};
//inline float4 tofloat4(float x, float3 yzw) {;
//	return float4(x, yzw.x, yzw.y, yzw.z);
//};
//inline float4 tofloat4(float2 xy, float2 zw) {;
//	return float4(xy.x, xy.y, zw.x, zw.y);
//};
//inline float4 tofloat4(float3 xyz, float w) {;
//	return float4(xyz.x, xyz.y, xyz.z, w);
//};
//inline float2x2 tofloat2x2(float2 v1, float2 v2) {;
//	return float2x2(v1.x, v1.y, v2.x, v2.y);
//};
//----------------------;
//glsl_defs.tmpl;
//float rand(vec2 x) {;
//    return fract(cos(mod(dot(x, vec2(13.9898, 8.141)), 3.14)) * 43758.5453);
//};
//vec2 rand2(vec2 x) {;
//    return fract(cos(mod(vec2(dot(x, vec2(13.9898, 8.141)),;
//						      dot(x, vec2(3.4562, 17.398))), vec2(3.14))) * 43758.5453);
//};
//vec3 rand3(vec2 x) {;
//    return fract(cos(mod(vec3(dot(x, vec2(13.9898, 8.141)),;
//							  dot(x, vec2(3.4562, 17.398)),;
//                              dot(x, vec2(13.254, 5.867))), vec3(3.14))) * 43758.5453);
//};
//float param_rnd(float minimum, float maximum, float seed) {;
//	return minimum+(maximum-minimum)*rand(vec2(seed));
//};
//vec3 rgb2hsv(vec3 c) {;
//	vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
//	vec4 p = c.g < c.b ? vec4(c.bg, K.wz) : vec4(c.gb, K.xy);
//	vec4 q = c.r < p.x ? vec4(p.xyw, c.r) : vec4(c.r, p.yzx);
//;
//	float d = q.x - MIN(q.w, q.y);
//	float e = 1.0e-10;
//	return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
//};
//vec3 hsv2rgb(vec3 c) {;
//	vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
//	vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
//	return c.z * mix(K.xxx, CLAMP(p - K.xxx, 0.0, 1.0), c.y);
//};
//----------------------;

Vector3 MMAlgos::clampv3(const Vector3 &v, const Vector3 &mi, const Vector3 &ma) {
	Vector3 r;
	r.x = CLAMP(v.x, mi.x, ma.x);
	r.y = CLAMP(v.y, mi.y, ma.y);
	r.z = CLAMP(v.z, mi.z, ma.z);
	return r;
}

Color MMAlgos::floorc(const Color &a) {
	Color v = Color();
	v.r = Math::floor(a.r);
	v.g = Math::floor(a.g);
	v.b = Math::floor(a.b);
	v.a = Math::floor(a.a);
	return v;
}

Vector2 MMAlgos::floorv2(const Vector2 &a) {
	Vector2 v = Vector2();
	v.x = Math::floor(a.x);
	v.y = Math::floor(a.y);
	return v;
}

Vector3 MMAlgos::floorv3(const Vector3 &a) {
	Vector3 v = Vector3();
	v.x = Math::floor(a.x);
	v.y = Math::floor(a.y);
	v.z = Math::floor(a.z);
	return v;
}

Vector2 MMAlgos::smoothstepv2(const float a, const float b, const Vector2 &c) {
	Vector2 v = Vector2();
	v.x = Math::smoothstep(a, b, c.x);
	v.y = Math::smoothstep(a, b, c.y);
	return v;
}

Vector2 MMAlgos::maxv2(const Vector2 &a, const Vector2 &b) {
	Vector2 v = Vector2();
	v.x = MAX(a.x, b.x);
	v.y = MAX(a.y, b.y);
	return v;
}

Vector3 MMAlgos::maxv3(const Vector3 &a, const Vector3 &b) {
	Vector3 v = Vector3();
	v.x = MAX(a.x, b.x);
	v.y = MAX(a.y, b.y);
	v.z = MAX(a.z, b.z);
	return v;
}

Vector2 MMAlgos::absv2(const Vector2 &v) {
	Vector2 r;
	r.x = abs(v.x);
	r.y = abs(v.y);
	return r;
}

Vector3 MMAlgos::absv3(const Vector3 &v) {
	Vector3 r;
	r.x = abs(v.x);
	r.y = abs(v.y);
	r.z = abs(v.z);
	return r;
}

Vector2 MMAlgos::cosv2(const Vector2 &v) {
	Vector2 r;
	r.x = cos(v.x);
	r.y = cos(v.y);
	return r;
}

Vector3 MMAlgos::cosv3(const Vector3 &v) {
	Vector3 r;
	r.x = cos(v.x);
	r.y = cos(v.y);
	r.z = cos(v.z);
	return r;
}

Vector2 MMAlgos::powv2(const Vector2 &x, const Vector2 &y) {
	Vector2 r;
	r.x = pow(x.x, y.x);
	r.y = pow(x.y, y.y);
	return r;
}

Vector3 MMAlgos::modv3(const Vector3 &a, const Vector3 &b) {
	Vector3 v = Vector3();
	v.x = modf(a.x, b.x);
	v.y = modf(a.y, b.y);
	v.z = modf(a.z, b.z);
	return v;
}

Vector2 MMAlgos::modv2(const Vector2 &a, const Vector2 &b) {
	Vector2 v = Vector2();
	v.x = modf(a.x, b.x);
	v.y = modf(a.y, b.y);
	return v;
}

float MMAlgos::modf(const float x, const float y) {
	return x - y * Math::floor(x / y);
}

Vector2 MMAlgos::fractv2(const Vector2 &v) {
	Vector2 r;
	r.x = v.x - Math::floor(v.x);
	r.y = v.y - Math::floor(v.y);
	return r;
}

Vector3 MMAlgos::fractv3(const Vector3 &v) {
	Vector3 r;
	r.x = v.x - Math::floor(v.x);
	r.y = v.y - Math::floor(v.y);
	r.z = v.z - Math::floor(v.z);
	return r;
}

float MMAlgos::fract(const float f) {
	return f - Math::floor(f);
}

Vector2 MMAlgos::clampv2(const Vector2 &v, const Vector2 &pmin, const Vector2 &pmax) {
	Vector2 r;
	r.x = CLAMP(v.x, pmin.x, pmax.x);
	r.y = CLAMP(v.y, pmin.y, pmax.y);
	return r;
}

Vector2 MMAlgos::minv2(const Vector2 &v1, const Vector2 &v2) {
	Vector2 r;
	r.x = MIN(v1.x, v2.x);
	r.y = MIN(v1.y, v2.y);
	return r;
}

Vector3 MMAlgos::minv3(const Vector3 &v1, const Vector3 &v2) {
	Vector3 r;
	r.x = MIN(v1.x, v2.x);
	r.y = MIN(v1.y, v2.y);
	r.z = MIN(v1.z, v2.z);
	return r;
}

float MMAlgos::rand(const Vector2 &x) {
	return fract(cos(x.dot(Vector2(13.9898, 8.141))) * 43758.5453);
}

Vector2 MMAlgos::rand2(const Vector2 &x) {
	return fractv2(cosv2(Vector2(x.dot(Vector2(13.9898, 8.141)), x.dot(Vector2(3.4562, 17.398)))) * 43758.5453);
}

Vector3 MMAlgos::rand3(const Vector2 &x) {
	return fractv3(cosv3(Vector3(x.dot(Vector2(13.9898, 8.141)), x.dot(Vector2(3.4562, 17.398)), x.dot(Vector2(13.254, 5.867)))) * 43758.5453);
}

float MMAlgos::step(const float edge, const float x) {
	if (x < edge) {
		return 0.0;
	} else {
		return 1.0;
	}
}

Vector2 MMAlgos::stepv2(const Vector2 &edge, const Vector2 &x) {
	Vector2 ret;
	ret.x = step(edge.x, x.x);
	ret.y = step(edge.y, x.y);
	return ret;
}

Color MMAlgos::stepc(const Color &edge, const Color &x) {
	Color ret;
	ret.r = step(edge.r, x.r);
	ret.g = step(edge.g, x.g);
	ret.b = step(edge.b, x.b);
	ret.a = step(edge.a, x.a);
	return ret;
}

Color MMAlgos::mixc(const Color &a, const Color &b, const Color &t) {
	Color ret;

	ret.r = Math::lerp(a.r, b.r, t.r);
	ret.g = Math::lerp(a.g, b.g, t.g);
	ret.b = Math::lerp(a.b, b.b, t.b);
	ret.a = Math::lerp(a.a, b.a, t.a);

	return ret;
}

Vector2 MMAlgos::signv2(const Vector2 &x) {
	Vector2 ret;
	ret.x = SGN(x.x);
	ret.y = SGN(x.y);
	return ret;
}

Vector2 MMAlgos::transform(const Vector2 &uvi, const Vector2 &translate, const float rotate, const Vector2 &scale, const bool repeat) {
	Vector2 uv = uvi;
	Vector2 rv = Vector2();
	uv -= translate;
	uv -= Vector2(0.5, 0.5);
	rv.x = cos(rotate) * uv.x + sin(rotate) * uv.y;
	rv.y = -sin(rotate) * uv.x + cos(rotate) * uv.y;
	rv /= scale;
	rv += Vector2(0.5, 0.5);

	if ((repeat)) {
		return fractv2(rv);
	} else {
		return clampv2(rv, Vector2(0, 0), Vector2(1, 1));
	}
}

float MMAlgos::fractf(const float x) {
	return x - Math::floor(x);
}

//float mix_mul(float x, float y) {;
//	return x*y;
//};

float MMAlgos::mix_mul(const float x, const float y) {
	return x * y;
}

//float mix_add(float x, float y) {;
//	return MIN(x+y, 1.0);
//};

float MMAlgos::mix_add(const float x, const float y) {
	return MIN(x + y, 1.0);
}

//float mix_max(float x, float y) {;
//	return MAX(x, y);
//};

float MMAlgos::mix_max(const float x, const float y) {
	return MAX(x, y);
}

//float mix_min(float x, float y) {;
//	return MIN(x, y);
//};

float MMAlgos::mix_min(const float x, const float y) {
	return MIN(x, y);
}

//float mix_xor(float x, float y) {;
//	return MIN(x+y, 2.0-x-y);
//};

float MMAlgos::mix_xor(const float x, const float y) {
	return MIN(x + y, 2.0 - x - y);
}

//float mix_pow(float x, float y) {;
//	return pow(x, y);
//};

float MMAlgos::mix_pow(const float x, const float y) {
	return pow(x, y);
}

//float wave_constant(float x) {;
//	return 1.0;
//};

float MMAlgos::wave_constant(const float x) {
	return 1.0;
}

//float wave_sine(float x) {;
//	return 0.5-0.5*cos(3.14159265359*2.0*x);
//};

float MMAlgos::wave_sine(const float x) {
	return 0.5 - 0.5 * cos(3.14159265359 * 2.0 * x);
}

//float wave_triangle(float x) {;
//	x = fract(x);
//	return MIN(2.0*x, 2.0-2.0*x);
//};

float MMAlgos::wave_triangle(const float xx) {
	float x = fractf(xx);
	return MIN(2.0 * x, 2.0 - 2.0 * x);
}

//float wave_sawtooth(float x) {;
//	return fract(x);
//};

float MMAlgos::wave_sawtooth(const float x) {
	return fractf(x);
}

//float wave_square(float x) {;
//	return (fract(x) < 0.5) ? 0.0 : 1.0;
//};

float MMAlgos::wave_square(const float x) {
	if ((fractf(x) < 0.5)) {
		return 0.0;
	} else {
		return 1.0;
	}

	//float wave_bounce(float x) {;
	//	x = 2.0*(fract(x)-0.5);
	//	return sqrt(1.0-x*x);
	//};
}

float MMAlgos::wave_bounce(const float xx) {
	float x = 2.0 * (fractf(xx) - 0.5);
	return sqrt(1.0 - x * x);
}

Color MMAlgos::sinewave(const Vector2 &uv, const float amplitude, const float frequency, const float phase) {
	float f = 1.0 - abs(2.0 * (uv.y - 0.5) - amplitude * sin((frequency * uv.x + phase) * 6.28318530718));
	return Color(f, f, f, 1);
}

//from runes.mmg (old);

float MMAlgos::ThickLine(const Vector2 &uv, const Vector2 &posA, const Vector2 &posB, const float radiusInv) {
	Vector2 dir = posA - posB;
	float dirLen = dir.length();
	Vector2 dirN = dir.normalized();
	float dotTemp = CLAMP((uv - posB).dot(dirN), 0.0, dirLen);
	Vector2 proj = dotTemp * dirN + posB;
	float d1 = (uv - proj).length();
	float finalGray = CLAMP(1.0 - d1 * radiusInv, 0.0, 1.0);
	return finalGray;
}

// ===============  BLUR.GD ===============================;
//----------------------;
//directional_blur.mmg;
//----------------------;
//fast_blur.mmg;
//----------------------;
//gaussian_blur.mmg;
//----------------------;
//gaussian_blur_x.mmg;
//----------------------;
//gaussian_blur_y.mmg;
//----------------------;
//slope_blur.mmg;

// ============= CURVES.GD ======================;
//Based on MaterialMaker's curve.gd;
//Curve PoolRealArray: p.x, p.y, ls, rs,  p.x, p.y ....;
//class Point:;
//	var p : Vector2;
//	var ls : float;
//	var rs : float;
//func get_shader(name) -> String:;
//	var shader;
//	shader = "float "+name+"_curve_fct(float x) {\n";
//	for i in range(points.size()-1):;
//		if i < points.size()-2:;
//			shader += "if (x <= p_"+name+"_"+str(i+1)+"_x) ";
//;
//		shader += "{\n";
//		shader += "float dx = x - p_"+name+"_"+str(i)+"_x;\n";
//		shader += "float d = p_"+name+"_"+str(i+1)+"_x - p_"+name+"_"+str(i)+"_x;\n";
//		shader += "float t = dx/d;\n";
//		shader += "float omt = (1.0 - t);\n";
//		shader += "float omt2 = omt * omt;\n";
//		shader += "float omt3 = omt2 * omt;\n";
//		shader += "float t2 = t * t;\n";
//		shader += "float t3 = t2 * t;\n";
//		shader += "d /= 3.0;\n";
//		shader += "float y1 = p_"+name+"_"+str(i)+"_y;\n";
//		shader += "float yac = p_"+name+"_"+str(i)+"_y + d*p_"+name+"_"+str(i)+"_rs;\n";
//		shader += "float ybc = p_"+name+"_"+str(i+1)+"_y - d*p_"+name+"_"+str(i+1)+"_ls;\n";
//		shader += "float y2 = p_"+name+"_"+str(i+1)+"_y;\n";
//		shader += "return y1*omt3 + yac*omt2*t*3.0 + ybc*omt*t2*3.0 + y2*t3;\n";
//		shader += "}\n";
//;
//	shader += "}\n";
//	return shader;

float MMAlgos::curve(const float x, const PoolRealArray &points) {
	if (points.size() % 4 != 0 || points.size() < 8) {
		return 0.0;
	}

	int ps = points.size() / 4;

	for (int i = 0; i < ps - 1; ++i) { //i in range(ps - 1)
		int pi = i * 4;
		int pip1 = (i + 1) * 4;

		if (i < ps - 2) {
			//	if (x <= p_"+name+"_"+str(i+1)+"_x);
			if (x > points[pip1]) {
				continue;
			}
		}

		//float dx = x - p_"+name+"_"+str(i)+"_x;
		float dx = x - points[pi];
		//var d : float = p_"+name+"_"+str(i+1)+"_x - p_"+name+"_"+str(i)+"_x;
		float d = points[pip1] - points[pi];
		float t = dx / d;
		float omt = (1.0 - t);
		float omt2 = omt * omt;
		float omt3 = omt2 * omt;
		float t2 = t * t;
		float t3 = t2 * t;
		d /= 3.0;
		//		var y1 : float = p_"+name+"_"+str(i)+"_y;
		float y1 = points[pi + 1];
		//		var yac : float = p_"+name+"_"+str(i)+"_y + d*p_"+name+"_"+str(i)+"_rs;
		float yac = points[pi + 1] + d * points[pi + 3];
		//		var ybc : float = p_"+name+"_"+str(i+1)+"_y - d*p_"+name+"_"+str(i+1)+"_ls;
		float ybc = points[pip1 + 1] - d * points[pip1 + 2];
		//		var y2 : float = p_"+name+"_"+str(i+1)+"_y;
		float y2 = points[pip1 + 1];
		return y1 * omt3 + yac * omt2 * t * 3.0 + ybc * omt * t2 * 3.0 + y2 * t3;
	}

	return 0.0;
}

// =============  DILATE.GD ==============;
//----------------------;
//dilate.mmg;
//NYI
//----------------------;
//dilate_pass_2.mmg;
//NYI
//----------------------;
//dilate_pass_3.mmg;
//NYI

// ==========  EDGE_DETECT.GD =============;
//----------------------;
//edge_detect.mmg;
//----------------------;
//edge_detect_1.mmg;
//----------------------;
//edge_detect_2.mmg;
//----------------------;
//edge_detect_3.mmg;
//----------------------;
//mul_detect.mmg;

// ================ FILLS.GD ============;

//fill.mmg;
//----------------------;
//----------------------;
//fill_preprocess.mmg;
//----------------------;
//fill_to_color.mmg;
//----------------------;
//fill_to_position.mmg;
//----------------------;
//fill_to_random_color.mmg;
//----------------------;
//fill_to_random_grey.mmg;
//----------------------;
//fill_to_size.mmg;
//----------------------;
//fill_to_uv.mmg;
//----------------------;

//vec4 flood_fill_preprocess(vec2 uv, float c, float s) {;
//	if (c > 0.5) {;
//		return vec4(0.0);
//	} else {;
//		return vec4(floor(uv*s)/s, vec2(1.0/s));
//	};
//};

Color MMAlgos::flood_fill_preprocess(const Vector2 &uv, const float c, const float s) {
	if ((c > 0.5)) {
		return Color(0, 0, 0, 0);
	} else {
		Vector2 uuv;
		uuv = floorv2(uv * s) / s;
		float f = 1.0 / s;
		return Color(uuv.x, uuv.y, f, f);
	}

	//vec3 fill_to_uv_stretch(vec2 coord, vec4 bb, float seed) {;
	//	vec2 uv_islands = fract(coord-bb.xy)/bb.zw;
	//	float random_value = rand(vec2(seed)+bb.xy+bb.zw);
	//	return vec3(uv_islands, random_value);
	//};
}

Vector3 MMAlgos::fill_to_uv_stretch(const Vector2 &coord, const Color &bb, const float pseed) {
	Vector2 uv_islands = fractv2(coord - Vector2(bb.r, bb.g)) / Vector2(bb.b, bb.a);
	float random_value = rand(Vector2(pseed, pseed) + Vector2(bb.r, bb.g) + Vector2(bb.b, bb.a));
	return Vector3(uv_islands.x, uv_islands.y, random_value);
}

//vec3 fill_to_uv_square(vec2 coord, vec4 bb, float seed) {;
//	vec2 uv_islands;
//;
//	if (bb.z > bb.w) {;
//		vec2 adjusted_coord = coord + vec2(0.0, (bb.z - bb.w) / 2.0);
//		uv_islands = fract(adjusted_coord-bb.xy)/bb.zz;
//	} else {;
//		vec2 adjusted_coord = coord + vec2((bb.w - bb.z) / 2.0, 0.0);
//		uv_islands = fract(adjusted_coord-bb.xy)/bb.ww;
//	};
//;
//	float random_value = rand(vec2(seed)+bb.xy+bb.zw);
//	return vec3(uv_islands, random_value);
//};

Vector3 MMAlgos::fill_to_uv_square(const Vector2 &coord, const Color &bb, const float pseed) {
	Vector2 uv_islands = Vector2();

	if ((bb.b > bb.a)) {
		Vector2 adjusted_coord = coord + Vector2(0.0, (bb.b - bb.a) / 2.0);
		uv_islands = fractv2(adjusted_coord - Vector2(bb.r, bb.g)) / Vector2(bb.b, bb.b);
	}

	else {
		Vector2 adjusted_coord = coord + Vector2((bb.a - bb.b) / 2.0, 0.0);
		uv_islands = fractv2(adjusted_coord - Vector2(bb.r, bb.g)) / Vector2(bb.a, bb.a);
	}

	float random_value = rand(Vector2(pseed, pseed) + Vector2(bb.r, bb.g) + Vector2(bb.b, bb.a));
	return Vector3(uv_islands.x, uv_islands.y, random_value);
}

//====== FILTER.GD ======;
//adjust_hsv.mmg;
//brightness_contrast.mmg;
//greyscale.mmg;
//main node methods: adjust_hsv, brightness_contrast;
//----------------------;
//colorize.mmg;
//Remaps a greyscale image to a custom gradient;
//Inputs:;
//input, float, $uv.x - The input greyscale image - (Image input);
//Outputs:;
//output (rgba) $gradient($input($uv)) - Image output;
//Parameters:;
//gradient, Gradient;
//----------------------;
//default_color.mmg;
//----------------------;
//decompose.mmg;
//----------------------;
//auto_tones.mmg;
//----------------------;
//blend.mmg;
//Blends its input, using an optional mask;
//Outputs:;
//Output - (color);
//vec4 $(name_uv)_s1 = $s1($uv);
//vec4 $(name_uv)_s2 = $s2($uv);
//float $(name_uv)_a = $amount*$a($uv);
//vec4(blend_$blend_type($uv, $(name_uv)_s1.rgb, $(name_uv)_s2.rgb, $(name_uv)_a*$(name_uv)_s1.a), MIN(1.0, $(name_uv)_s2.a+$(name_uv)_a*$(name_uv)_s1.a));
//Inputs:;
//in1, color, default vec4($uv.x, 1.0, 1.0, 1.0);
//in2, color, default vec4($uv.x, 1.0, 1.0, 1.0);
//blend_type, enum, default: 0, Normal,Dissolve,Multiply,Screen,Overlay,Hard Light,Soft Light,Burn,Dodge,Lighten,Darken,Difference;
//opacity, float, min: 0, max: 1, default: 0.5, step: 0.01 (input float);
//----------------------;
//combine.mmg;
//----------------------;
//emboss.mmg;
//Creates highlights and shadows from an input heightmap;
//float $(name)_fct(vec2 uv) {;
//	float pixels = MAX(1.0, $width);
//	float e = 1.0/$size;
//	float rv = 0.0;
//;
//	for (float dx = -pixels; dx <= pixels; dx += 1.0) {;
//		for (float dy = -pixels; dy <= pixels; dy += 1.0) {;
//			if (abs(dx) > 0.5 || abs(dy) > 0.5) {;
//				rv += $in(uv+e*vec2(dx, dy))*cos(atan(dy, dx)-$angle*3.14159265359/180.0)/length(vec2(dx, dy));
//			};
//		};
//	};
//;
//	return $amount*rv/pixels+0.5;
//};
//Outputs:;
//Output - (float);
//$(name)_fct($uv);
//Inputs:;
//input, float, default 0;
//size, int (image size);
//angle, float, min: -180, max: 180, default: 0, step: 0.1;
//amount, float, min: 0, max: 10, default: 1, step: 0.1;
//width, float, min: 1, max: 5, default: 1, step: 1;
//----------------------;
//invert.mmg;
//A filter that inverts the R, G, and B channels of its input while keeping the A channel unchanged;
//Outputs:;
//Output - (rgba);
//vec4(vec3(1.0)-$in($uv).rgb, $in($uv).a);
//Inputs:;
//input, rgba, default vec4(1.0, 1.0, 1.0, 1.0);
//----------------------;
//normal_map.mmg;
//----------------------;
//sharpen.mmg;
//----------------------;
//tones.mmg;
//----------------------;
//tones_map.mmg;
//----------------------;
//make_tileable.mmg;
//Creates a tileable version of its input image by moving different parts around to hide seams.;
//vec4 make_tileable_$(name)(vec2 uv, float w) {;
//	vec4 a = $in(uv);
//	vec4 b = $in(fract(uv+vec2(0.5)));
//	float coef_ab = sin(1.57079632679*clamp((length(uv-vec2(0.5))-0.5+w)/w, 0.0, 1.0));
//	vec4 c = $in(fract(uv+vec2(0.25)));
//	float coef_abc = sin(1.57079632679*clamp((min(min(length(uv-vec2(0.0, 0.5)), length(uv-vec2(0.5, 0.0))), MIN(length(uv-vec2(1.0, 0.5)), length(uv-vec2(0.5, 1.0))))-w)/w, 0.0, 1.0));
//	return mix(c, mix(a, b, coef_ab), coef_abc);
//};
//Inputs:;
//input, rgba, default: vec4(1.0);
//width, float, min:0, max: 1: default: 0.1, step: 0.01;
//Outputs:;
//output (rgba);
//make_tileable_$(name)($uv, 0.5*$w);
//----------------------;
//occlusion.mmg;
//----------------------;
//occlusion2.mmg;
//----------------------;
//pixelize.mmg;
//----------------------;
//quantize.mmg;
//----------------------;
//skew.mmg;
//----------------------;
//tonality.mmg;
//----------------------;
//tones_range.mmg;
//----------------------;
//tones_step.mmg;
//----------------------;
//math.mmg;
//----------------------;
//smooth_curvature.mmg;
//----------------------;
//smooth_curvature2.mmg;
//----------------------;
//supersample.mmg;
//----------------------;
//swap_channels.mmg;
//----------------------;
//math_v3.mmg;
//----------------------;

//vec3 rgb_to_hsv(vec3 c) {;
//	vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
//	vec4 p = c.g < c.b ? vec4(c.bg, K.wz) : vec4(c.gb, K.xy);
//	vec4 q = c.r < p.x ? vec4(p.xyw, c.r) : vec4(c.r, p.yzx);
//;
//	float d = q.x - MIN(q.w, q.y);
//	float e = 1.0e-10;
//	return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
//};

Vector3 MMAlgos::rgb_to_hsv(const Vector3 &c) {
	Color K = Color(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
	Color p;

	if (c.y < c.z) {
		p = Color(c.z, c.y, K.a, K.b);
	} else {
		p = Color(c.y, c.z, K.r, K.g);
	}

	Color q;

	if (c.x < p.r) {
		q = Color(p.r, p.g, p.a, c.x);
	} else {
		q = Color(c.x, p.g, p.b, p.r);
	}

	float d = q.r - MIN(q.a, q.g);
	float e = 1.0e-10;
	return Vector3(abs(q.b + (q.a - q.g) / (6.0 * d + e)), d / (q.r + e), q.r);
}

//vec3 hsv_to_rgb(vec3 c) {;
//	vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
//	vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
//	return c.z * mix(K.xxx, CLAMP(p - K.xxx, 0.0, 1.0), c.y);
//};

Vector3 MMAlgos::hsv_to_rgb(const Vector3 &c) {
	Color K = Color(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
	Vector3 p = absv3(fractv3(Vector3(c.x, c.x, c.x) + Vector3(K.r, K.g, K.b)) * 6.0 - Vector3(K.a, K.a, K.a));
	return c.z * Vector3(K.r, K.r, K.r).linear_interpolate(clampv3(p - Vector3(K.r, K.r, K.r), Vector3(), Vector3(1, 1, 1)), c.y);
}

//adjust_hsv.mmg;
//vec4 $(name_uv)_rbga = $in($(uv));
//vec3 $(name_uv)_hsv = rgb_to_hsv($(name_uv)_rbga.rgb);
//$(name_uv)_hsv.x += $(hue);
//$(name_uv)_hsv.y = CLAMP($(name_uv)_hsv.y*$(saturation), 0.0, 1.0);
//$(name_uv)_hsv.z = CLAMP($(name_uv)_hsv.z*$(value), 0.0, 1.0);
//hue, min: -0.5, max: 0.5, step: 0, default: 0;
//saturation, min: 0, max: 2, step: 0, default: 1;
//value, min: 0, max: 2, step: 0, default: 1;
//output: vec4(hsv_to_rgb($(name_uv)_hsv), $(name_uv)_rbga.a);

Color MMAlgos::adjust_hsv(const Color &color, const float hue, const float saturation, const float value) {
	Vector3 hsv = rgb_to_hsv(Vector3(color.r, color.g, color.b));
	hsv.x += hue;
	hsv.y = CLAMP(hsv.y * saturation, 0.0, 1.0);
	hsv.z = CLAMP(hsv.z * value, 0.0, 1.0);
	Vector3 h = hsv_to_rgb(hsv);
	return Color(h.x, h.y, h.z, color.a);
}

//tones.mmg;
//vec4 adjust_levels(vec4 input, vec4 in_min, vec4 in_mid, vec4 in_max, vec4 out_min, vec4 out_max) {
//	input = clamp((input-in_min)/(in_max-in_min), 0.0, 1.0);
//	in_mid = (in_mid-in_min)/(in_max-in_min);
//	vec4 dark = step(in_mid, input);
//	input = 0.5*mix(input/(in_mid), 1.0+(input-in_mid)/(1.0-in_mid), dark);
//	return out_min+input*(out_max-out_min);
//}

Color MMAlgos::adjust_levels(const Color &p_input, const Color &p_in_min, const Color &p_in_mid, const Color &p_in_max, const Color &p_out_min, const Color &p_out_max) {
	Color input = (p_input - p_in_min) / (p_in_max - p_in_min).clamp();
	Color in_mid = (p_in_mid - p_in_min) / (p_in_max - p_in_min);
	Color dark = stepc(in_mid, p_input);
	input = Color(0.5, 0.5, 0.5, 0.5) * mixc(input / (in_mid), Color(1, 1, 1, 1) + (input - in_mid) / (Color(1, 1, 1, 1) - in_mid), dark);
	return p_out_min + input * (p_out_max - p_out_min);
}

//brightness, min: -1, max: 1, step: 0.01, default: 0;
//contrast, min: -1, max: 1, step: 0.01, default: 1;
//input: default: vec4(0.5 ,0.5, 0.5, 1.0) -> img;
//output: vec4(clamp($in($uv).rgb*$contrast+vec3($brightness)+0.5-$contrast*0.5, vec3(0.0), vec3(1.0)), $in($uv).a);

Color MMAlgos::brightness_contrast(const Color &color, const float brightness, const float contrast) {
	//output: vec4(clamp( $in($uv).rgb*$contrast + vec3($brightness) + 0.5 - $contrast*0.5, vec3(0.0), vec3(1.0)), $in($uv).a);
	Vector3 cvv = Vector3(color.r, color.g, color.b) * contrast;
	Vector3 cv = cvv + Vector3(brightness, brightness, brightness) + Vector3(0.5, 0.5, 0.5) - Vector3(contrast, contrast, contrast) * 0.5;
	Vector3 v = clampv3(cv, Vector3(), Vector3(1, 1, 1));
	return Color(v.x, v.y, v.z, color.a);
}

//greyscale;
//input: default: vec3(0.0). (Image);
//output: gs_$mode($in($uv));
//mode: enum: Lightness, Average, Luminosity, Min, Max. default: 4;
//float gs_min(vec3 c) {;
//	return MIN(c.r, MIN(c.g, c.b));
//};

float MMAlgos::grayscale_min(const Vector3 &c) {
	return MIN(c.x, MIN(c.y, c.z));
}

//float gs_max(vec3 c) {;
//	return MAX(c.r, MAX(c.g, c.b));
//};

float MMAlgos::grayscale_max(const Vector3 &c) {
	return MAX(c.x, MAX(c.y, c.z));
}

//float gs_lightness(vec3 c) {;
//	return 0.5*(MAX(c.r, MAX(c.g, c.b)) + MIN(c.r, MIN(c.g, c.b)));
//};

float MMAlgos::grayscale_lightness(const Vector3 &c) {
	return 0.5 * (MAX(c.x, MAX(c.y, c.z)) + MIN(c.x, MIN(c.y, c.z)));
}

//float gs_average(vec3 c) {;
//	return 0.333333333333*(c.r + c.g + c.b);
//};

float MMAlgos::grayscale_average(const Vector3 &c) {
	return 0.333333333333 * (c.x + c.y + c.z);
}

//float gs_luminosity(vec3 c) {;
//	return 0.21 * c.r + 0.72 * c.g + 0.07 * c.b;
//};

float MMAlgos::grayscale_luminosity(const Vector3 &c) {
	return 0.21 * c.x + 0.72 * c.y + 0.07 * c.z;
}

Color MMAlgos::invert(const Color &color) {
	return Color(1.0 - color.r, 1.0 - color.g, 1.0 - color.b, color.a);
}

//vec3 blend_normal(vec2 uv, vec3 c1, vec3 c2, float opacity) {\n\t;
//	return opacity*c1 + (1.0-opacity)*c2;\n;
//};

Vector3 MMAlgos::blend_normal(const Vector2 &uv, const Vector3 &c1, const Vector3 &c2, const float opacity) {
	return opacity * c1 + (1.0 - opacity) * c2;
}

//vec3 blend_dissolve(vec2 uv, vec3 c1, vec3 c2, float opacity) {\n\t;
//	if (rand(uv) < opacity) {\n\t\t;
//		return c1;\n\t;
//	} else {\n\t\t;
//		return c2;\n\t;
//	}\n;
//};

Vector3 MMAlgos::blend_dissolve(const Vector2 &uv, const Vector3 &c1, const Vector3 &c2, const float opacity) {
	if ((rand2(uv) < Vector2(opacity, opacity))) {
		return c1;
	} else {
		return c2;
	}

	//vec3 blend_multiply(vec2 uv, vec3 c1, vec3 c2, float opacity) {\n\t;
	//	return opacity*c1*c2 + (1.0-opacity)*c2;\n;
	//};
}

Vector3 MMAlgos::blend_multiply(const Vector2 &uv, const Vector3 &c1, const Vector3 &c2, const float opacity) {
	return opacity * c1 * c2 + (1.0 - opacity) * c2;
}

//vec3 blend_screen(vec2 uv, vec3 c1, vec3 c2, float opacity) {\n\t;
//	return opacity*(1.0-(1.0-c1)*(1.0-c2)) + (1.0-opacity)*c2;\n;
//};

Vector3 MMAlgos::blend_screen(const Vector2 &uv, const Vector3 &c1, const Vector3 &c2, const float opacity) {
	return opacity * (Vector3(1, 1, 1) - (Vector3(1, 1, 1) - c1) * (Vector3(1, 1, 1) - c2)) + (1.0 - opacity) * c2;
}

//float blend_overlay_f(float c1, float c2) {\n\t;
//	return (c1 < 0.5) ? (2.0*c1*c2) : (1.0-2.0*(1.0-c1)*(1.0-c2));\n;
//};

float MMAlgos::blend_overlay_f(const float c1, const float c2) {
	if ((c1 < 0.5)) {
		return (2.0 * c1 * c2);
	} else {
		return (1.0 - 2.0 * (1.0 - c1) * (1.0 - c2));
	}

	//vec3 blend_overlay(vec2 uv, vec3 c1, vec3 c2, float opacity) {\n\t;
	//	return opacity*vec3(blend_overlay_f(c1.x, c2.x), blend_overlay_f(c1.y, c2.y), blend_overlay_f(c1.z, c2.z)) + (1.0-opacity)*c2;\n;
	//};
}

Vector3 MMAlgos::blend_overlay(const Vector2 &uv, const Vector3 &c1, const Vector3 &c2, const float opacity) {
	return opacity * Vector3(blend_overlay_f(c1.x, c2.x), blend_overlay_f(c1.y, c2.y), blend_overlay_f(c1.z, c2.z)) + (1.0 - opacity) * c2;
}

//vec3 blend_hard_light(vec2 uv, vec3 c1, vec3 c2, float opacity) {\n\t;
//	return opacity*0.5*(c1*c2+blend_overlay(uv, c1, c2, 1.0)) + (1.0-opacity)*c2;\n;
//};

Vector3 MMAlgos::blend_hard_light(const Vector2 &uv, const Vector3 &c1, const Vector3 &c2, const float opacity) {
	return opacity * 0.5 * (c1 * c2 + blend_overlay(uv, c1, c2, 1.0)) + (1.0 - opacity) * c2;
}

//float blend_soft_light_f(float c1, float c2) {\n\t;
//	return (c2 < 0.5) ? (2.0*c1*c2+c1*c1*(1.0-2.0*c2)) : 2.0*c1*(1.0-c2)+sqrt(c1)*(2.0*c2-1.0);\n;
//};

float MMAlgos::blend_soft_light_f(const float c1, const float c2) {
	if ((c2 < 0.5)) {
		return (2.0 * c1 * c2 + c1 * c1 * (1.0 - 2.0 * c2));
	} else {
		return 2.0 * c1 * (1.0 - c2) + sqrt(c1) * (2.0 * c2 - 1.0);
	}

	//vec3 blend_soft_light(vec2 uv, vec3 c1, vec3 c2, float opacity) {\n\t;
	//	return opacity*vec3(blend_soft_light_f(c1.x, c2.x), blend_soft_light_f(c1.y, c2.y), blend_soft_light_f(c1.z, c2.z)) + (1.0-opacity)*c2;\n;
	//};
}

Vector3 MMAlgos::blend_soft_light(const Vector2 &uv, const Vector3 &c1, const Vector3 &c2, const float opacity) {
	return opacity * Vector3(blend_soft_light_f(c1.x, c2.x), blend_soft_light_f(c1.y, c2.y), blend_soft_light_f(c1.z, c2.z)) + (1.0 - opacity) * c2;
}

//float blend_burn_f(float c1, float c2) {\n\t;
//	return (c1==0.0)?c1:MAX((1.0-((1.0-c2)/c1)),0.0);\n;
//};

float MMAlgos::blend_burn_f(const float c1, const float c2) {
	if ((c1 == 0.0)) {
		return c1;
	} else {
		return MAX((1.0 - ((1.0 - c2) / c1)), 0.0);
	}

	//vec3 blend_burn(vec2 uv, vec3 c1, vec3 c2, float opacity) {\n\t;
	//	return opacity*vec3(blend_burn_f(c1.x, c2.x), blend_burn_f(c1.y, c2.y), blend_burn_f(c1.z, c2.z)) + (1.0-opacity)*c2;
	//};
}

Vector3 MMAlgos::blend_burn(const Vector2 &uv, const Vector3 &c1, const Vector3 &c2, const float opacity) {
	return opacity * Vector3(blend_burn_f(c1.x, c2.x), blend_burn_f(c1.y, c2.y), blend_burn_f(c1.z, c2.z)) + (1.0 - opacity) * c2;
}

//float blend_dodge_f(float c1, float c2) {\n\t;
//	return (c1==1.0)?c1:min(c2/(1.0-c1),1.0);\n;
//};

float MMAlgos::blend_dodge_f(const float c1, const float c2) {
	if ((c1 == 1.0)) {
		return c1;
	}

	else {
		return MIN(c2 / (1.0 - c1), 1.0);
	}

	//vec3 blend_dodge(vec2 uv, vec3 c1, vec3 c2, float opacity) {\n\t;
	//	return opacity*vec3(blend_dodge_f(c1.x, c2.x), blend_dodge_f(c1.y, c2.y), blend_dodge_f(c1.z, c2.z)) + (1.0-opacity)*c2;\n;
	//};
}

Vector3 MMAlgos::blend_dodge(const Vector2 &uv, const Vector3 &c1, const Vector3 &c2, const float opacity) {
	return opacity * Vector3(blend_dodge_f(c1.x, c2.x), blend_dodge_f(c1.y, c2.y), blend_dodge_f(c1.z, c2.z)) + (1.0 - opacity) * c2;
}

//vec3 blend_lighten(vec2 uv, vec3 c1, vec3 c2, float opacity) {\n\t;
//	return opacity*MAX(c1, c2) + (1.0-opacity)*c2;\n;
//};

Vector3 MMAlgos::blend_lighten(const Vector2 &uv, const Vector3 &c1, const Vector3 &c2, const float opacity) {
	return opacity * maxv3(c1, c2) + (1.0 - opacity) * c2;
}

//vec3 blend_darken(vec2 uv, vec3 c1, vec3 c2, float opacity) {\n\t;
//	return opacity*min(c1, c2) + (1.0-opacity)*c2;\n;
//};

Vector3 MMAlgos::blend_darken(const Vector2 &uv, const Vector3 &c1, const Vector3 &c2, const float opacity) {
	return opacity * minv3(c1, c2) + (1.0 - opacity) * c2;
}

//vec3 blend_difference(vec2 uv, vec3 c1, vec3 c2, float opacity) {\n\t;
//	return opacity*clamp(c2-c1, vec3(0.0), vec3(1.0)) + (1.0-opacity)*c2;\n;
//};

Vector3 MMAlgos::blend_difference(const Vector2 &uv, const Vector3 &c1, const Vector3 &c2, const float opacity) {
	return opacity * clampv3(c2 - c1, Vector3(), Vector3(1, 1, 1)) + (1.0 - opacity) * c2;
}

// === GRADIENTS.GD =====;
//note: data : PoolRealArray -> pos, r, g, b, a, pos, r, g, b, a ....;
//gradient.mmg;
//float $(name_uv)_r = 0.5+(cos($rotate*0.01745329251)*($uv.x-0.5)+sin($rotate*0.01745329251)*($uv.y-0.5))/(cos(abs(mod($rotate, 90.0)-45.0)*0.01745329251)*1.41421356237);";
//output: $gradient(fract($(name_uv)_r*$repeat));
//repeat: default: 1, min: 1, max : 32, step: 1;
//rotate: default: 0, min: -180, max: 180, step: 0.1;
//default: "interpolation": 1,;
// "points": [{"a": 1,"b": 0,"g": 0,"pos": 0,"r": 0},{"a": 1,"b": 1,"g": 1,"pos": 1,"r": 1} ],;
//radial_gradient.mmg;
//output: $gradient(fract($repeat*1.41421356237*length(fract($uv)-vec2(0.5, 0.5))));
//repeat: default: 1, min: 1, max : 32, step: 1;
//circular_gradient.mmg;
//output: gradient(fract($repeat*0.15915494309*atan($uv.y-0.5, $uv.x-0.5)));
//repeat: default: 1, min: 1, max : 32, step: 1;
//gradient.gd;

Color MMAlgos::radial_gradient_type_1(const Vector2 &uv, const float repeat, const PoolRealArray &data) {
	return gradient_type_1(fractf(repeat * 1.41421356237 * (fractv2(uv) - Vector2(0.5, 0.5)).length()), data);
}

Color MMAlgos::radial_gradient_type_2(const Vector2 &uv, const float repeat, const PoolRealArray &data) {
	return gradient_type_2(fractf(repeat * 1.41421356237 * (fractv2(uv) - Vector2(0.5, 0.5)).length()), data);
}

Color MMAlgos::radial_gradient_type_3(const Vector2 &uv, const float repeat, const PoolRealArray &data) {
	return gradient_type_3(fractf(repeat * 1.41421356237 * (fractv2(uv) - Vector2(0.5, 0.5)).length()), data);
}

Color MMAlgos::radial_gradient_type_4(const Vector2 &uv, const float repeat, const PoolRealArray &data) {
	return gradient_type_4(fractf(repeat * 1.41421356237 * (fractv2(uv) - Vector2(0.5, 0.5)).length()), data);
}

Color MMAlgos::normal_gradient_type_1(const Vector2 &uv, const float repeat, const float rotate, const PoolRealArray &data) {
	float rr = 0.5 + (cos(rotate * 0.01745329251) * (uv.x - 0.5) + sin(rotate * 0.01745329251) * (uv.y - 0.5)) / (cos(abs(modf(rotate, 90.0) - 45.0) * 0.01745329251) * 1.41421356237);
	return gradient_type_1(fractf(rr * repeat), data);
}

Color MMAlgos::normal_gradient_type_2(const Vector2 &uv, const float repeat, const float rotate, const PoolRealArray &data) {
	float rr = 0.5 + (cos(rotate * 0.01745329251) * (uv.x - 0.5) + sin(rotate * 0.01745329251) * (uv.y - 0.5)) / (cos(abs(modf(rotate, 90.0) - 45.0) * 0.01745329251) * 1.41421356237);
	return gradient_type_2(fractf(rr * repeat), data);
}

Color MMAlgos::normal_gradient_type_3(const Vector2 &uv, const float repeat, const float rotate, const PoolRealArray &data) {
	float rr = 0.5 + (cos(rotate * 0.01745329251) * (uv.x - 0.5) + sin(rotate * 0.01745329251) * (uv.y - 0.5)) / (cos(abs(modf(rotate, 90.0) - 45.0) * 0.01745329251) * 1.41421356237);
	return gradient_type_3(fractf(rr * repeat), data);
}

Color MMAlgos::normal_gradient_type_4(const Vector2 &uv, const float repeat, const float rotate, const PoolRealArray &data) {
	float rr = 0.5 + (cos(rotate * 0.01745329251) * (uv.x - 0.5) + sin(rotate * 0.01745329251) * (uv.y - 0.5)) / (cos(abs(modf(rotate, 90.0) - 45.0) * 0.01745329251) * 1.41421356237);
	return gradient_type_4(fractf(rr * repeat), data);
}

Color MMAlgos::circular_gradient_type_1(const Vector2 &uv, const float repeat, const PoolRealArray &data) {
	return gradient_type_1(fractf(repeat * 0.15915494309 * atan2((uv.y - 0.5), uv.x - 0.5)), data);
}

Color MMAlgos::circular_gradient_type_2(const Vector2 &uv, const float repeat, const PoolRealArray &data) {
	return gradient_type_2(fractf(repeat * 0.15915494309 * atan2((uv.y - 0.5), uv.x - 0.5)), data);
}

Color MMAlgos::circular_gradient_type_3(const Vector2 &uv, const float repeat, const PoolRealArray &data) {
	return gradient_type_3(fractf(repeat * 0.15915494309 * atan2((uv.y - 0.5), uv.x - 0.5)), data);
}

Color MMAlgos::circular_gradient_type_4(const Vector2 &uv, const float repeat, const PoolRealArray &data) {
	return gradient_type_4(fractf(repeat * 0.15915494309 * atan2((uv.y - 0.5), uv.x - 0.5)), data);
}

Color MMAlgos::gradient_type_1(const float x, const PoolRealArray &data) {
	if (data.size() % 5 != 0 || data.size() == 0) {
		return Color();
	}

	for (int i = 0; i < data.size() - 5; i += 5) { //i in range(0, data.size() - 5, 5)
		if (x < 0.5 * (data[i] + data[i + 5])) {
			return Color(data[i + 1], data[i + 2], data[i + 3], data[i + 4]);
		}
	}

	int ds = data.size() - 5;
	return Color(data[ds + 1], data[ds + 2], data[ds + 3], data[ds + 4]);
}

Color MMAlgos::gradient_type_2(const float x, const PoolRealArray &data) {
	if (data.size() % 5 != 0 || data.size() == 0) {
		return Color();
	}

	for (int i = 0; i < data.size(); i += 5) { //i in range(0, data.size(), 5)
		if (x < data[i]) {
			if (i == 0) {
				return Color(data[i + 1], data[i + 2], data[i + 3], data[i + 4]);
			}

			Color cprev = Color(data[i - 4], data[i - 3], data[i - 2], data[i - 1]);
			Color ccurr = Color(data[i + 1], data[i + 2], data[i + 3], data[i + 4]);
			return cprev.linear_interpolate(ccurr, (x - data[i - 5]) / (data[i] - data[i - 5]));
		}
	}

	int ds = data.size() - 5;
	return Color(data[ds + 1], data[ds + 2], data[ds + 3], data[ds + 4]);
}

Color MMAlgos::gradient_type_3(const float x, const PoolRealArray &data) {
	if (data.size() % 5 != 0 || data.size() == 0) {
		return Color();
	}

	for (int i = 0; i < data.size(); i += 5) { //i in range(0, data.size(), 5)
		if (x < data[i]) {
			if (i == 0) {
				return Color(data[i + 1], data[i + 2], data[i + 3], data[i + 4]);
			}

			Color cprev = Color(data[i - 4], data[i - 3], data[i - 2], data[i - 1]);
			Color ccurr = Color(data[i + 1], data[i + 2], data[i + 3], data[i + 4]);
			return cprev.linear_interpolate(ccurr, 0.5 - 0.5 * cos(3.14159265359 * ((x - data[i - 5]) / (data[i] - data[i - 5]))));
		}
	}

	int ds = data.size() - 5;
	return Color(data[ds + 1], data[ds + 2], data[ds + 3], data[ds + 4]);
}

Color MMAlgos::get_data_color(const int index, const PoolRealArray &data) {
	int i = index * 5;
	return Color(data[i + 1], data[i + 2], data[i + 3], data[i + 4]);
}

float MMAlgos::get_data_pos(const int index, const PoolRealArray &data) {
	return data[index * 5];
}

Color MMAlgos::gradient_type_4(const float x, const PoolRealArray &data) {
	if (data.size() % 5 != 0 || data.size() == 0) {
		return Color();
	}

	int ds = data.size() / 5;
	int s = ds - 1;

	for (int i = 0; i < s; ++i) { //i in range(0, s)
		if (x < get_data_pos(i, data)) {
			if (i == 0) {
				return get_data_color(i, data);
			}

			//			var dx : String = "(x-%s)/(%s-%s)" % [ pv(name, i), pv(name, i+1), pv(name, i) ];
			float dx = (x - get_data_pos(i, data)) / (get_data_pos(i + 1, data) - get_data_pos(i, data));
			//			var b : String = "mix(%s, %s, %s)" % [ pc(name, i), pc(name, i+1), dx ];
			Color b = get_data_color(i - 1, data).linear_interpolate(get_data_color(i - 1, data), dx);

			if (i == 1) {
				//				var c : String = "mix(%s, %s, (x-%s)/(%s-%s))" % [ pc(name, i+1), pc(name, i+2), pv(name, i+1), pv(name, i+2), pv(name, i+1) ];
				Color c = get_data_color(i + 1, data).linear_interpolate(get_data_color(i + 2, data), (x - get_data_pos(i + 1, data)) / (get_data_pos(i + 2, data) - get_data_pos(i + 1, data)));
				//				shader += "    return mix("+c+", "+b+", 1.0-0.5*"+dx+");\n";
				return c.linear_interpolate(b, 1.0 - 0.5 * dx);
			}

			//			var a : String = "mix(%s, %s, (x-%s)/(%s-%s))" % [ pc(name, i-1), pc(name, i),  pv(name, i-1), pv(name, i), pv(name, i-1) ];
			Color a = get_data_color(i - 1, data).linear_interpolate(get_data_color(i, data), (x - get_data_pos(i - 1, data)) / (get_data_pos(i, data) - get_data_pos(i - 1, data)));
			//			if i < s-1:;

			if (i < s - 1) {
				//				var c : String = "mix(%s, %s, (x-%s)/(%s-%s))" % [ pc(name, i+1), pc(name, i+2), pv(name, i+1), pv(name, i+2), pv(name, i+1) ];
				Color c = get_data_color(i + 1, data).linear_interpolate(get_data_color(i + 2, data), (x - get_data_pos(i + 1, data)) / (get_data_pos(i + 2, data) - get_data_pos(i + 1, data)));
				//				var ac : String = "mix("+a+", "+c+", 0.5-0.5*cos(3.14159265359*"+dx+"))";
				Color ac = a.linear_interpolate(c, 0.5 - 0.5 * cos(3.14159265359 * dx));
				//				shader += "    return 0.5*("+b+" + "+ac+");\n";
				Color dt = b + ac;
				dt.r *= 0.5;
				dt.g *= 0.5;
				dt.b *= 0.5;
				dt.a = CLAMP(0, 1, dt.a);
				return dt;
				//			else;
			} else {
				//				shader += "    return mix("+a+", "+b+", 0.5+0.5*"+dx+");\n";
				return a.linear_interpolate(b, 0.5 + 0.5 * dx);
			}
		}
	}

	return get_data_color(ds - 1, data);
}

//todo make it selectable;

Color MMAlgos::gradient_type_5(const float x, const PoolRealArray &data) {
	if (data.size() % 5 != 0 || data.size() == 0) {
		return Color();
	}

	int ds = data.size() / 5;
	int s = ds - 1;

	for (int i = 0; i < s; ++i) { //i in range(0, s)
		if (x < get_data_pos(i, data)) {
			if (i == 0) {
				return get_data_color(i, data);
			}

			//			var dx : String = "(x-%s)/(%s-%s)" % [ pv(name, i), pv(name, i+1), pv(name, i) ];
			float dx = (x - get_data_pos(i, data)) / (get_data_pos(i + 1, data) - get_data_pos(i, data));
			//			var b : String = "mix(%s, %s, %s)" % [ pc(name, i), pc(name, i+1), dx ];
			Color b = get_data_color(i - 1, data).linear_interpolate(get_data_color(i - 1, data), dx);

			if (i == 1) {
				//				var c : String = "mix(%s, %s, (x-%s)/(%s-%s))" % [ pc(name, i+1), pc(name, i+2), pv(name, i+1), pv(name, i+2), pv(name, i+1) ];
				Color c = get_data_color(i + 1, data).linear_interpolate(get_data_color(i + 2, data), (x - get_data_pos(i + 1, data)) / (get_data_pos(i + 2, data) - get_data_pos(i + 1, data)));
				//				shader += "    return mix("+c+", "+b+", 1.0-0.5*"+dx+");\n";
				return c.linear_interpolate(b, 1.0 - 0.5 * dx);
			}

			//			var a : String = "mix(%s, %s, (x-%s)/(%s-%s))" % [ pc(name, i-1), pc(name, i),  pv(name, i-1), pv(name, i), pv(name, i-1) ];
			Color a = get_data_color(i - 1, data).linear_interpolate(get_data_color(i, data), (x - get_data_pos(i - 1, data)) / (get_data_pos(i, data) - get_data_pos(i - 1, data)));
			//			if i < s-1:;

			if (i < s - 1) {
				//				var c : String = "mix(%s, %s, (x-%s)/(%s-%s))" % [ pc(name, i+1), pc(name, i+2), pv(name, i+1), pv(name, i+2), pv(name, i+1) ];
				Color c = get_data_color(i + 1, data).linear_interpolate(get_data_color(i + 2, data), (x - get_data_pos(i + 1, data)) / (get_data_pos(i + 2, data) - get_data_pos(i + 1, data)));
				//				var ac : String = "mix("+a+", "+c+", 0.5-0.5*cos(3.14159265359*"+dx+"))";
				Color ac = a.linear_interpolate(c, 0.5 - 0.5 * cos(3.14159265359 * dx));
				//				shader += "    return 0.5*("+b+" + "+ac+");\n";
				Color dt = b + ac;
				dt.r *= 0.5;
				dt.g *= 0.5;
				dt.b *= 0.5;
				dt.a = CLAMP(0, 1, dt.a);
				return dt;
				//			else;
			} else {
				//				shader += "    return mix("+a+", "+b+", 0.5+0.5*"+dx+");\n";
				return a.linear_interpolate(b, 0.5 + 0.5 * dx);
			}
		}
	}

	return get_data_color(ds - 1, data);
}

// ==== MWF.GD========;
//----------------------;
//mwf_create_map.mmg;
//----------------------;
//mwf_mix_maps.mmg;
//----------------------;
//mwf_map.mmg;
//----------------------;
//mwf_mix.mmg;
//----------------------;
//mwf_output.mmg;
//----------------------;
//edge_detect.mmg;
//----------------------;
//edge_detect.mmg;

//vec3 matmap_mix(vec3 in1, vec3 in2) {\n\t;
//	float is_in1 = step(in2.x, in1.x);\n\t;
//	//return vec3(MAX(in1.x, in2.x), in1.yz*is_in1+in2.yz*(1.0-is_in1));\n\t;
//	return vec3(MAX(in1.x, in2.x), mix(in2.yz, in1.yz, is_in1));\n;
//};

//vec2 matmap_uv(vec2 uv, float angle, float seed) {\n\t;
//	uv -= vec2(0.5);\n\tvec2 rv;\n\t;
//	rv.x = uv.x*cos(angle)+uv.y*sin(angle);\n\t;
//	rv.y = -uv.x*sin(angle)+uv.y*cos(angle);\n\t;
//	return fract(rv + rand2(vec2(seed)));\n;
//};

//vec3 matmap_rotate_nm(vec3 input, float angle) {\n\t;
//	vec2 uv = input.xy - vec2(0.5);\n\t;
//	vec2 rv;\n\t;
//	rv.x = uv.x*cos(angle)+uv.y*sin(angle);\n\t;
//	rv.y = -uv.x*sin(angle)+uv.y*cos(angle);\n\t;
//	return vec3(rv + vec2(0.5), input.z);\n;
//};

// ======= NOISES.GD ====;
//----------------------;
//color_noise.mmg;
//Outputs:;
//Output - (rgb) - Shows the noise pattern;
//color_dots($(uv), 1.0/$(size), $(seed));
//Inputs:;
//size, float, default: 8, min: 2, max: 12, step: 1;
//----------------------;
//noise.mmg;
//Outputs:;
//float $(name)_f(vec2 uv) {;
//	return dots(uv, 1.0/$(size), $(density), $(seed));
//};
//Output - (float) - Shows the noise pattern;
//$(name)_f($(uv));
//Inputs:;
//grid_size, float, default: 4, min: 2, max: 12, step: 1;
//density, float, default: 0.5, min: 0, max: 1, step: 0.01;
//----------------------;
//noise_anisotropic.mmg;
//Generates x-axis interpolated value noise;
//Output:;
//Output (float) - Shows a greyscale value noise;
//anisotropic($(uv), vec2($(scale_x), $(scale_y)), $(seed), $(smoothness), $(interpolation));
//Input:;
//scale, Vector2, min: 1, 1, max: 32, 1024, step: 1, 1, default 4, 256;
//smoothness, float, min: 0, max: 1, step: 0,01, default: 1;
//Interpolation, float, min: 0, max: 1, step: 0,01, default: 1;
//float dots(vec2 uv, float size, float density, float seed) {;
//	vec2 seed2 = rand2(vec2(seed, 1.0-seed));
//	uv /= size;
//	vec2 point_pos = Math::floor(uv)+vec2(0.5);
//	float color = step(rand(seed2+point_pos), density);
//	return color;
//};

float MMAlgos::dots(const Vector2 &uuv, const float size, const float density, const float pseed) {
	Vector2 uv = uuv;
	Vector2 seed2 = rand2(Vector2(pseed, 1.0 - pseed));
	uv /= size;
	Vector2 point_pos = floorv2(uv) + Vector2(0.5, 0.5);
	float color = step(rand2(seed2 + point_pos).x, density);
	return color;
}

Color MMAlgos::anisotropicc(const Vector2 &uv, const Vector2 &size, const float pseed, const float smoothness, const float interpolation) {
	float v = anisotropic(uv, size, pseed, smoothness, interpolation);
	return Color(v, v, v, 1);
}

//float anisotropic(vec2 uv, vec2 size, float seed, float smoothness, float interpolation) {;
//	vec2 seed2 = rand2(vec2(seed, 1.0-seed));
//	vec2 xy = Math::floor(uv*size);
//	vec2 offset = vec2(rand(seed2 + xy.y), 0.0);
//	vec2 xy_offset = Math::floor(uv * size + offset );
//;
//	float f0 = rand(seed2+mod(xy_offset, size));
//	float f1 = rand(seed2+mod(xy_offset+vec2(1.0, 0.0), size));
//	float mixer = CLAMP( (fract(uv.x*size.x+offset.x) -.5) / smoothness + 0.5, 0.0, 1.0 );
//	float smooth_mix = Math::smoothstep(0.0, 1.0, mixer);
//	float linear = mix(f0, f1, mixer);
//	float smoothed = mix(f0, f1, smooth_mix);
//;
//	return mix(linear, smoothed, interpolation);
//};

float MMAlgos::anisotropic(const Vector2 &uv, const Vector2 &size, const float pseed, const float smoothness, const float interpolation) {
	Vector2 seed2 = rand2(Vector2(pseed, 1.0 - pseed));
	Vector2 xy = floorv2(uv * size);
	Vector2 s2xy = seed2;
	s2xy.x += xy.y;
	s2xy.y += xy.y;
	Vector2 offset = Vector2(rand(s2xy), 0.0);
	Vector2 xy_offset = floorv2(uv * size + offset);
	float f0 = rand(seed2 + modv2(xy_offset, size));
	float f1 = rand(seed2 + modv2(xy_offset + Vector2(1.0, 0.0), size));
	float mixer = CLAMP((fract(uv.x * size.x + offset.x) - 0.5) / smoothness + 0.5, 0.0, 1.0);
	float smooth_mix = Math::smoothstep(0.0F, 1.0F, mixer);
	float linear = Math::lerp(f0, f1, mixer);
	float smoothed = Math::lerp(f0, f1, smooth_mix);
	return Math::lerp(linear, smoothed, interpolation);
}

//vec3 color_dots(vec2 uv, float size, float seed) {;
//	vec2 seed2 = rand2(vec2(seed, 1.0-seed));
//	uv /= size;
//	vec2 point_pos = Math::floor(uv)+vec2(0.5);
//	return rand3(seed2+point_pos);
//};

Vector3 MMAlgos::color_dots(const Vector2 &uuv, const float size, const float pseed) {
	Vector2 uv = uuv;
	Vector2 seed2 = rand2(Vector2(pseed, 1.0 - pseed));
	uv /= size;
	Vector2 point_pos = floorv2(uv) + Vector2(0.5, 0.5);
	return rand3(seed2 + point_pos);
}

Color MMAlgos::noise_color(const Vector2 &uv, const float size, const float pseed) {
	Vector3 v = color_dots(uv, 1.0 / size, pseed);
	return Color(v.x, v.y, v.z, 1);
}

// ====== NOISE_FBM.GD ======;
//fbm2.mmg (and fbm.mmg);
//Output:;
//$(name)_fbm($(uv), vec2($(scale_x), $(scale_y)), int($(folds)), int($(iterations)), $(persistence), float($(seed)));
//Instance:;
//float $(name)_fbm(vec2 coord, vec2 size, int folds, int octaves, float persistence, float seed) {;
//	float normalize_factor = 0.0;
//	float value = 0.0;
//	float scale = 1.0;
//;
//	for (int i = 0; i < octaves; i++) {;
//		float noise = fbm_$noise(coord*size, size, seed);
//;
//		for (int f = 0; f < folds; ++f) {;
//			noise = abs(2.0*noise-1.0);
//		};
//;
//		value += noise * scale;
//		normalize_factor += scale;
//		size *= 2.0;
//		scale *= persistence;
//	};
//;
//	return value / normalize_factor;
//};
//Inputs:;
//noise, enum, default: 2, values: Value, Perlin, Simplex, Cellular, Cellular2, Cellular3, Cellular4, Cellular5, Cellular6;
//scale, vector2, default: 4, min: 1, max: 32, step: 1;
//folds, float, default: 0, min: 0, max: 5, step: 1;
//iterations (octaves), float, default: 3, min: 1, max: 10, step: 1;
//persistence, float, default: 0.5, min: 0, max: 1, step: 0.01;

Color MMAlgos::fbmval(const Vector2 &uv, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed) {
	float f = fbmf(uv, size, folds, octaves, persistence, pseed);
	return Color(f, f, f, 1);
}

Color MMAlgos::perlin(const Vector2 &uv, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed) {
	float f = perlinf(uv, size, folds, octaves, persistence, pseed);
	return Color(f, f, f, 1);
}

Color MMAlgos::perlinabs(const Vector2 &uv, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed) {
	float f = perlinf(uv, size, folds, octaves, persistence, pseed);
	return Color(f, f, f, 1);
}

Color MMAlgos::simplex(const Vector2 &uv, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed) {
	float f = fbm_simplexf(uv, size, folds, octaves, persistence, pseed);
	return Color(f, f, f, 1);
}

Color MMAlgos::cellular(const Vector2 &uv, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed) {
	float f = cellularf(uv, size, folds, octaves, persistence, pseed);
	return Color(f, f, f, 1);
}

Color MMAlgos::cellular2(const Vector2 &uv, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed) {
	float f = cellular2f(uv, size, folds, octaves, persistence, pseed);
	return Color(f, f, f, 1);
}

Color MMAlgos::cellular3(const Vector2 &uv, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed) {
	float f = cellular3f(uv, size, folds, octaves, persistence, pseed);
	return Color(f, f, f, 1);
}

Color MMAlgos::cellular4(const Vector2 &uv, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed) {
	float f = cellular4f(uv, size, folds, octaves, persistence, pseed);
	return Color(f, f, f, 1);
}

Color MMAlgos::cellular5(const Vector2 &uv, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed) {
	float f = cellular5f(uv, size, folds, octaves, persistence, pseed);
	return Color(f, f, f, 1);
}

Color MMAlgos::cellular6(const Vector2 &uv, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed) {
	float f = cellular6f(uv, size, folds, octaves, persistence, pseed);
	return Color(f, f, f, 1);
}

float MMAlgos::fbmf(const Vector2 &coord, const Vector2 &p_size, const int folds, const int octaves, const float persistence, const float pseed) {
	Vector2 size = p_size;
	float normalize_factor = 0.0;
	float value = 0.0;
	float scale = 1.0;
	// (int i = 0; i < octaves; i++) {;

	for (int i = 0; i < octaves; ++i) { //i in range(octaves)
		float noise = fbm_value(coord * size, size, pseed);
		// (int f = 0; f < folds; ++f) {;

		for (int j = 0; j < folds; ++j) { //j in range(folds)
			noise = abs(2.0 * noise - 1.0);
		}

		value += noise * scale;
		normalize_factor += scale;
		size *= Vector2(2, 2);
		scale *= persistence;
	}

	return value / normalize_factor;
}

float MMAlgos::perlinf(const Vector2 &coord, const Vector2 &p_size, const int folds, const int octaves, const float persistence, const float pseed) {
	Vector2 size = p_size;
	float normalize_factor = 0.0;
	float value = 0.0;
	float scale = 1.0;
	// (int i = 0; i < octaves; i++) {;

	for (int i = 0; i < octaves; ++i) { //i in range(octaves)
		float noise = fbm_perlin(coord * size, size, pseed);
		// (int f = 0; f < folds; ++f) {;

		for (int j = 0; j < folds; ++j) { //j in range(folds)
			noise = abs(2.0 * noise - 1.0);
		}

		value += noise * scale;
		normalize_factor += scale;
		size *= Vector2(2, 2);
		scale *= persistence;
	}

	return value / normalize_factor;
}

float MMAlgos::perlinabsf(const Vector2 &coord, const Vector2 &p_size, const int folds, const int octaves, const float persistence, const float pseed) {
	Vector2 size = p_size;
	float normalize_factor = 0.0;
	float value = 0.0;
	float scale = 1.0;
	// (int i = 0; i < octaves; i++) {;

	for (int i = 0; i < octaves; ++i) { //i in range(octaves)
		float noise = fbm_perlinabs(coord * size, size, pseed);
		// (int f = 0; f < folds; ++f) {;

		for (int j = 0; j < folds; ++j) { //j in range(folds)
			noise = abs(2.0 * noise - 1.0);
		}

		value += noise * scale;
		normalize_factor += scale;
		size *= Vector2(2, 2);
		scale *= persistence;
	}

	return value / normalize_factor;
}

float MMAlgos::fbm_simplexf(const Vector2 &coord, const Vector2 &p_size, const int folds, const int octaves, const float persistence, const float pseed) {
	Vector2 size = p_size;
	float normalize_factor = 0.0;
	float value = 0.0;
	float scale = 1.0;
	// (int i = 0; i < octaves; i++) {;

	for (int i = 0; i < octaves; ++i) { //i in range(octaves)
		float noise = fbm_simplex(coord * size, size, pseed);
		// (int f = 0; f < folds; ++f) {;

		for (int j = 0; j < folds; ++j) { //j in range(folds)
			noise = abs(2.0 * noise - 1.0);
		}

		value += noise * scale;
		normalize_factor += scale;
		size *= Vector2(2, 2);
		scale *= persistence;
	}

	return value / normalize_factor;
}

float MMAlgos::cellularf(const Vector2 &coord, const Vector2 &p_size, const int folds, const int octaves, const float persistence, const float pseed) {
	Vector2 size = p_size;
	float normalize_factor = 0.0;
	float value = 0.0;
	float scale = 1.0;
	// (int i = 0; i < octaves; i++) {;

	for (int i = 0; i < octaves; ++i) { //i in range(octaves)
		float noise = fbm_cellular(coord * size, size, pseed);
		// (int f = 0; f < folds; ++f) {;

		for (int j = 0; j < folds; ++j) { //j in range(folds)
			noise = abs(2.0 * noise - 1.0);
		}

		value += noise * scale;
		normalize_factor += scale;
		size *= Vector2(2, 2);
		scale *= persistence;
	}

	return value / normalize_factor;
}

float MMAlgos::cellular2f(const Vector2 &coord, const Vector2 &p_size, const int folds, const int octaves, const float persistence, const float pseed) {
	Vector2 size = p_size;
	float normalize_factor = 0.0;
	float value = 0.0;
	float scale = 1.0;
	// (int i = 0; i < octaves; i++) {;

	for (int i = 0; i < octaves; ++i) { //i in range(octaves)
		float noise = fbm_cellular2(coord * size, size, pseed);
		// (int f = 0; f < folds; ++f) {;

		for (int j = 0; j < folds; ++j) { //j in range(folds)
			noise = abs(2.0 * noise - 1.0);
		}

		value += noise * scale;
		normalize_factor += scale;
		size *= Vector2(2, 2);
		scale *= persistence;
	}

	return value / normalize_factor;
}

float MMAlgos::cellular3f(const Vector2 &coord, const Vector2 &p_size, const int folds, const int octaves, const float persistence, const float pseed) {
	Vector2 size = p_size;
	float normalize_factor = 0.0;
	float value = 0.0;
	float scale = 1.0;
	// (int i = 0; i < octaves; i++) {;

	for (int i = 0; i < octaves; ++i) { //i in range(octaves)
		float noise = fbm_cellular3(coord * size, size, pseed);
		// (int f = 0; f < folds; ++f) {;

		for (int j = 0; j < folds; ++j) { //j in range(folds)
			noise = abs(2.0 * noise - 1.0);
		}

		value += noise * scale;
		normalize_factor += scale;
		size *= Vector2(2, 2);
		scale *= persistence;
	}

	return value / normalize_factor;
}

float MMAlgos::cellular4f(const Vector2 &coord, const Vector2 &p_size, const int folds, const int octaves, const float persistence, const float pseed) {
	Vector2 size = p_size;
	float normalize_factor = 0.0;
	float value = 0.0;
	float scale = 1.0;
	// (int i = 0; i < octaves; i++) {;

	for (int i = 0; i < octaves; ++i) { //i in range(octaves)
		float noise = fbm_cellular4(coord * size, size, pseed);
		// (int f = 0; f < folds; ++f) {;

		for (int j = 0; j < folds; ++j) { //j in range(folds)
			noise = abs(2.0 * noise - 1.0);
		}

		value += noise * scale;
		normalize_factor += scale;
		size *= Vector2(2, 2);
		scale *= persistence;
	}

	return value / normalize_factor;
}

float MMAlgos::cellular5f(const Vector2 &coord, const Vector2 &p_size, const int folds, const int octaves, const float persistence, const float pseed) {
	Vector2 size = p_size;
	float normalize_factor = 0.0;
	float value = 0.0;
	float scale = 1.0;
	// (int i = 0; i < octaves; i++) {;

	for (int i = 0; i < octaves; ++i) { //i in range(octaves)
		float noise = fbm_cellular5(coord * size, size, pseed);
		// (int f = 0; f < folds; ++f) {;

		for (int j = 0; j < folds; ++j) { //j in range(folds)
			noise = abs(2.0 * noise - 1.0);
		}

		value += noise * scale;
		normalize_factor += scale;
		size *= Vector2(2, 2);
		scale *= persistence;
	}

	return value / normalize_factor;
}

float MMAlgos::cellular6f(const Vector2 &coord, const Vector2 &p_size, const int folds, const int octaves, const float persistence, const float pseed) {
	Vector2 size = p_size;
	float normalize_factor = 0.0;
	float value = 0.0;
	float scale = 1.0;
	// (int i = 0; i < octaves; i++) {;

	for (int i = 0; i < octaves; ++i) { //i in range(octaves)
		float noise = fbm_cellular6(coord * size, size, pseed);
		// (int f = 0; f < folds; ++f) {;

		for (int j = 0; j < folds; ++j) { //j in range(folds)
			noise = abs(2.0 * noise - 1.0);
		}

		value += noise * scale;
		normalize_factor += scale;
		size *= Vector2(2, 2);
		scale *= persistence;
	}

	return value / normalize_factor;
}

//float fbm_value(vec2 coord, vec2 size, float seed) {;
//	vec2 o = Math::floor(coord)+rand2(vec2(seed, 1.0-seed))+size;
//	vec2 f = fract(coord);
//;
//	float p00 = rand(mod(o, size));
//	float p01 = rand(mod(o + vec2(0.0, 1.0), size));
//	float p10 = rand(mod(o + vec2(1.0, 0.0), size));
//	float p11 = rand(mod(o + vec2(1.0, 1.0), size));
//;
//	vec2 t = f * f * (3.0 - 2.0 * f);
//;
//	return mix(mix(p00, p10, t.x), mix(p01, p11, t.x), t.y);
//};

float MMAlgos::fbm_value(const Vector2 &coord, const Vector2 &size, const float pseed) {
	Vector2 o = floorv2(coord) + rand2(Vector2(float(pseed), 1.0 - float(pseed))) + size;
	Vector2 f = fractv2(coord);
	float p00 = rand(modv2(o, size));
	float p01 = rand(modv2(o + Vector2(0.0, 1.0), size));
	float p10 = rand(modv2(o + Vector2(1.0, 0.0), size));
	float p11 = rand(modv2(o + Vector2(1.0, 1.0), size));
	Vector2 t = f * f * (Vector2(3, 3) - 2.0 * f);
	return Math::lerp(Math::lerp(p00, p10, t.x), Math::lerp(p01, p11, t.x), t.y);
}

//float fbm_perlin(vec2 coord, vec2 size, float seed) {;
//	tvec2 o = Math::floor(coord)+rand2(vec2(seed, 1.0-seed))+size;
//	vec2 f = fract(coord);
//;
//	float a00 = rand(mod(o, size)) * 6.28318530718;
//	float a01 = rand(mod(o + vec2(0.0, 1.0), size)) * 6.28318530718;
//	float a10 = rand(mod(o + vec2(1.0, 0.0), size)) * 6.28318530718;
//	float a11 = rand(mod(o + vec2(1.0, 1.0), size)) * 6.28318530718;
//;
//	vec2 v00 = vec2(cos(a00), sin(a00));
//	vec2 v01 = vec2(cos(a01), sin(a01));
//	vec2 v10 = vec2(cos(a10), sin(a10));
//	vec2 v11 = vec2(cos(a11), sin(a11));
//;
//	float p00 = dot(v00, f);
//	float p01 = dot(v01, f - vec2(0.0, 1.0));
//	float p10 = dot(v10, f - vec2(1.0, 0.0));
//	float p11 = dot(v11, f - vec2(1.0, 1.0));
//;
//	vec2 t = f * f * (3.0 - 2.0 * f);
//;
//	return 0.5 + mix(mix(p00, p10, t.x), mix(p01, p11, t.x), t.y);
//};

float MMAlgos::fbm_perlin(const Vector2 &coord, const Vector2 &size, const float pseed) {
	Vector2 o = floorv2(coord) + rand2(Vector2(float(pseed), 1.0 - float(pseed))) + size;
	Vector2 f = fractv2(coord);
	float a00 = rand(modv2(o, size)) * 6.28318530718;
	float a01 = rand(modv2(o + Vector2(0.0, 1.0), size)) * 6.28318530718;
	float a10 = rand(modv2(o + Vector2(1.0, 0.0), size)) * 6.28318530718;
	float a11 = rand(modv2(o + Vector2(1.0, 1.0), size)) * 6.28318530718;
	Vector2 v00 = Vector2(cos(a00), sin(a00));
	Vector2 v01 = Vector2(cos(a01), sin(a01));
	Vector2 v10 = Vector2(cos(a10), sin(a10));
	Vector2 v11 = Vector2(cos(a11), sin(a11));
	float p00 = v00.dot(f);
	float p01 = v01.dot(f - Vector2(0.0, 1.0));
	float p10 = v10.dot(f - Vector2(1.0, 0.0));
	float p11 = v11.dot(f - Vector2(1.0, 1.0));
	Vector2 t = f * f * (Vector2(3, 3) - 2.0 * f);
	return 0.5 + Math::lerp(Math::lerp(p00, p10, t.x), Math::lerp(p01, p11, t.x), t.y);
}

//float fbm_perlinabs(vec2 coord, vec2 size, float seed) {;
//	return abs(2.0*fbm_perlin(coord, size, seed)-1.0);
//};

float MMAlgos::fbm_perlinabs(const Vector2 &coord, const Vector2 &size, const float pseed) {
	return abs(2.0 * fbm_perlin(coord, size, pseed) - 1.0);
}

//vec2 rgrad2(vec2 p, float rot, float seed) {;
//	float u = rand(p + vec2(seed, 1.0-seed));
//	u = fract(u) * 6.28318530718; // 2*pi;
//	return vec2(cos(u), sin(u));
//};

Vector2 MMAlgos::rgrad2(const Vector2 &p, const float rot, const float pseed) {
	float u = rand(p + Vector2(pseed, 1.0 - pseed));
	// 2*pi;
	u = fract(u) * 6.28318530718;
	return Vector2(cos(u), sin(u));
}

//float fbm_simplex(vec2 coord, vec2 size, float seed) {;
//	coord *= 2.0; // needed for it to tile;
//	coord += rand2(vec2(seed, 1.0-seed)) + size;
//	size *= 2.0; // needed for it to tile;
//	coord.y += 0.001;
//;
//	vec2 uv = vec2(coord.x + coord.y*0.5, coord.y);
//	vec2 i0 = Math::floor(uv);    vec2 f0 = fract(uv);
//	vec2 i1 = (f0.x > f0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
//	vec2 p0 = vec2(i0.x - i0.y * 0.5, i0.y);
//	vec2 p1 = vec2(p0.x + i1.x - i1.y * 0.5, p0.y + i1.y);
//	vec2 p2 = vec2(p0.x + 0.5, p0.y + 1.0);
//;
//	i1 = i0 + i1;
//;
//	vec2 i2 = i0 + vec2(1.0, 1.0);
//	vec2 d0 = coord - p0;
//	vec2 d1 = coord - p1;
//	vec2 d2 = coord - p2;
//;
//	vec3 xw = mod(vec3(p0.x, p1.x, p2.x), size.x);
//	vec3 yw = mod(vec3(p0.y, p1.y, p2.y), size.y);
//;
//	vec3 iuw = xw + 0.5 * yw;
//	vec3 ivw = yw;
//;
//	vec2 g0 = rgrad2(vec2(iuw.x, ivw.x), 0.0, seed);
//	vec2 g1 = rgrad2(vec2(iuw.y, ivw.y), 0.0, seed);
//	vec2 g2 = rgrad2(vec2(iuw.z, ivw.z), 0.0, seed);
//;
//	vec3 w = vec3(dot(g0, d0), dot(g1, d1), dot(g2, d2));
//	vec3 t = 0.8 - vec3(dot(d0, d0), dot(d1, d1), dot(d2, d2));
//;
//	t = MAX(t, vec3(0.0));
//	vec3 t2 = t * t;
//	vec3 t4 = t2 * t2;
//	float n = dot(t4, w);
//;
//	return 0.5 + 5.5 * n;
//};

float MMAlgos::fbm_simplex(const Vector2 &p_coord, const Vector2 &p_size, const float pseed) {
	Vector2 coord = p_coord;
	Vector2 size = p_size;
	// needed for it to tile;
	coord *= Vector2(2, 2);
	coord += rand2(Vector2(pseed, 1.0 - pseed)) + size;
	// needed for it to tile;
	size *= Vector2(2, 2);
	coord.y += 0.001;
	Vector2 uv = Vector2(coord.x + coord.y * 0.5, coord.y);
	Vector2 i0 = floorv2(uv);
	Vector2 f0 = fractv2(uv);
	Vector2 i1;

	if ((f0.x > f0.y)) {
		i1 = Vector2(1.0, 0.0);
	}

	else {
		i1 = Vector2(0.0, 1.0);
	}

	Vector2 p0 = Vector2(i0.x - i0.y * 0.5, i0.y);
	Vector2 p1 = Vector2(p0.x + i1.x - i1.y * 0.5, p0.y + i1.y);
	Vector2 p2 = Vector2(p0.x + 0.5, p0.y + 1.0);
	i1 = i0 + i1;
	//Vector2 i2 = i0 + Vector2(1.0, 1.0);
	Vector2 d0 = coord - p0;
	Vector2 d1 = coord - p1;
	Vector2 d2 = coord - p2;
	Vector3 xw = modv3(Vector3(p0.x, p1.x, p2.x), Vector3(size.x, size.x, size.x));
	Vector3 yw = modv3(Vector3(p0.y, p1.y, p2.y), Vector3(size.y, size.y, size.y));
	Vector3 iuw = xw + 0.5 * yw;
	Vector3 ivw = yw;
	Vector2 g0 = rgrad2(Vector2(iuw.x, ivw.x), 0.0, pseed);
	Vector2 g1 = rgrad2(Vector2(iuw.y, ivw.y), 0.0, pseed);
	Vector2 g2 = rgrad2(Vector2(iuw.z, ivw.z), 0.0, pseed);
	Vector3 w = Vector3(g0.dot(d0), g1.dot(d1), g2.dot(d2));
	Vector3 t = Vector3(0.8, 0.8, 0.8) - Vector3(d0.dot(d0), d1.dot(d1), d2.dot(d2));
	t = maxv3(t, Vector3());
	Vector3 t2 = t * t;
	Vector3 t4 = t2 * t2;
	float n = t4.dot(w);
	return 0.5 + 5.5 * n;
}

//float fbm_cellular(vec2 coord, vec2 size, float seed) {;
//	vec2 o = Math::floor(coord)+rand2(vec2(seed, 1.0-seed))+size;
//	vec2 f = fract(coord);
//	float min_dist = 2.0;
//;
//	for(float x = -1.0; x <= 1.0; x++) {;
//		for(float y = -1.0; y <= 1.0; y++) {;
//			vec2 node = rand2(mod(o + vec2(x, y), size)) + vec2(x, y);
//			float dist = sqrt((f - node).x * (f - node).x + (f - node).y * (f - node).y);
//			min_dist = MIN(min_dist, dist);
//		};
//	};
//;
//	return min_dist;
//};

float MMAlgos::fbm_cellular(const Vector2 &coord, const Vector2 &size, const float pseed) {
	Vector2 o = floorv2(coord) + rand2(Vector2(float(pseed), 1.0 - float(pseed))) + size;
	Vector2 f = fractv2(coord);
	float min_dist = 2.0;
	//(float x = -1.0; x <= 1.0; x++) {;

	for (int xx = -1; xx < 2; ++xx) { //xx in range(-1, 2)
		float x = xx;
		//(float y = -1.0; y <= 1.0; y++) {;

		for (int yy = -1; yy < 2; ++yy) { //yy in range(-1, 2)
			float y = yy;
			Vector2 node = rand2(modv2(o + Vector2(x, y), size)) + Vector2(x, y);
			float dist = sqrt((f - node).x * (f - node).x + (f - node).y * (f - node).y);
			min_dist = MIN(min_dist, dist);
		}
	}

	return min_dist;
}

//float fbm_cellular2(vec2 coord, vec2 size, float seed) {;
//	vec2 o = Math::floor(coord)+rand2(vec2(seed, 1.0-seed))+size;
//	vec2 f = fract(coord);
//	float min_dist1 = 2.0;
//	float min_dist2 = 2.0;
//;
//	for(float x = -1.0; x <= 1.0; x++) {;
//		for(float y = -1.0; y <= 1.0; y++) {;
//			vec2 node = rand2(mod(o + vec2(x, y), size)) + vec2(x, y);
//			float dist = sqrt((f - node).x * (f - node).x + (f - node).y * (f - node).y);
//			if (min_dist1 > dist) {;
//				min_dist2 = min_dist1;
//				min_dist1 = dist;
//			} else if (min_dist2 > dist) {;
//				min_dist2 = dist;
//			};
//		};
//	};
//;
//	return min_dist2-min_dist1;
//};

float MMAlgos::fbm_cellular2(const Vector2 &coord, const Vector2 &size, const float pseed) {
	Vector2 o = floorv2(coord) + rand2(Vector2(float(pseed), 1.0 - float(pseed))) + size;
	Vector2 f = fractv2(coord);
	float min_dist1 = 2.0;
	float min_dist2 = 2.0;
	//(float x = -1.0; x <= 1.0; x++) {;

	for (int xx = -1; xx < 2; ++xx) { //xx in range(-1, 2)
		float x = xx;
		//(float y = -1.0; y <= 1.0; y++) {;

		for (int yy = -1; yy < 2; ++yy) { //yy in range(-1, 2)
			float y = yy;
			Vector2 node = rand2(modv2(o + Vector2(x, y), size)) + Vector2(x, y);
			float dist = sqrt((f - node).x * (f - node).x + (f - node).y * (f - node).y);

			if ((min_dist1 > dist)) {
				min_dist2 = min_dist1;
				min_dist1 = dist;
			} else if ((min_dist2 > dist)) {
				min_dist2 = dist;
			}
		}
	}

	return min_dist2 - min_dist1;
}

//float fbm_cellular3(vec2 coord, vec2 size, float seed) {;
//	vec2 o = Math::floor(coord)+rand2(vec2(seed, 1.0-seed))+size;
//	vec2 f = fract(coord);
//	float min_dist = 2.0;
//;
//	for(float x = -1.0; x <= 1.0; x++) {;
//		for(float y = -1.0; y <= 1.0; y++) {;
//			vec2 node = rand2(mod(o + vec2(x, y), size))*0.5 + vec2(x, y);
//			float dist = abs((f - node).x) + abs((f - node).y);
//			min_dist = MIN(min_dist, dist);
//		};
//	};
//;
//	return min_dist;
//};

float MMAlgos::fbm_cellular3(const Vector2 &coord, const Vector2 &size, const float pseed) {
	Vector2 o = floorv2(coord) + rand2(Vector2(float(pseed), 1.0 - float(pseed))) + size;
	Vector2 f = fractv2(coord);
	float min_dist = 2.0;
	//(float x = -1.0; x <= 1.0; x++) {;

	for (int xx = -1; xx < 2; ++xx) { //xx in range(-1, 2)
		float x = xx;
		//(float y = -1.0; y <= 1.0; y++) {;

		for (int yy = -1; yy < 2; ++yy) { //yy in range(-1, 2)
			float y = yy;
			Vector2 node = rand2(modv2(o + Vector2(x, y), size)) * 0.5 + Vector2(x, y);
			float dist = abs((f - node).x) + abs((f - node).y);
			min_dist = MIN(min_dist, dist);
		}
	}

	return min_dist;
}

//float fbm_cellular4(vec2 coord, vec2 size, float seed) {;
//	vec2 o = Math::floor(coord)+rand2(vec2(seed, 1.0-seed))+size;
//	vec2 f = fract(coord);
//	float min_dist1 = 2.0;
//	float min_dist2 = 2.0;
//;
//	for(float x = -1.0; x <= 1.0; x++) {;
//		for(float y = -1.0; y <= 1.0; y++) {;
//			vec2 node = rand2(mod(o + vec2(x, y), size))*0.5 + vec2(x, y);
//			float dist = abs((f - node).x) + abs((f - node).y);
//;
//			if (min_dist1 > dist) {;
//				min_dist2 = min_dist1;
//				min_dist1 = dist;
//			} else if (min_dist2 > dist) {;
//				min_dist2 = dist;
//			};
//		};
//	};
//;
//	return min_dist2-min_dist1;
//};

float MMAlgos::fbm_cellular4(const Vector2 &coord, const Vector2 &size, const float pseed) {
	Vector2 o = floorv2(coord) + rand2(Vector2(float(pseed), 1.0 - float(pseed))) + size;
	Vector2 f = fractv2(coord);
	float min_dist1 = 2.0;
	float min_dist2 = 2.0;
	//(float x = -1.0; x <= 1.0; x++) {;

	for (int xx = -1; xx < 2; ++xx) { //xx in range(-1, 2)
		float x = xx;
		//(float y = -1.0; y <= 1.0; y++) {;

		for (int yy = -1; yy < 2; ++yy) { //yy in range(-1, 2)
			float y = yy;
			Vector2 node = rand2(modv2(o + Vector2(x, y), size)) * 0.5 + Vector2(x, y);
			float dist = abs((f - node).x) + abs((f - node).y);

			if ((min_dist1 > dist)) {
				min_dist2 = min_dist1;
				min_dist1 = dist;
			} else if ((min_dist2 > dist)) {
				min_dist2 = dist;
			}
		}
	}

	return min_dist2 - min_dist1;
}

//float fbm_cellular5(vec2 coord, vec2 size, float seed) {;
//	vec2 o = Math::floor(coord)+rand2(vec2(seed, 1.0-seed))+size;
//	vec2 f = fract(coord);
//	float min_dist = 2.0;
//;
//	for(float x = -1.0; x <= 1.0; x++) {;
//		for(float y = -1.0; y <= 1.0; y++) {;
//			vec2 node = rand2(mod(o + vec2(x, y), size)) + vec2(x, y);
//			float dist = MAX(abs((f - node).x), abs((f - node).y));
//			min_dist = MIN(min_dist, dist);
//		};
//	};
//;
//	return min_dist;
//};

float MMAlgos::fbm_cellular5(const Vector2 &coord, const Vector2 &size, const float pseed) {
	Vector2 o = floorv2(coord) + rand2(Vector2(float(pseed), 1.0 - float(pseed))) + size;
	Vector2 f = fractv2(coord);
	float min_dist = 2.0;
	//(float x = -1.0; x <= 1.0; x++) {;

	for (int xx = -1; xx < 2; ++xx) { //xx in range(-1, 2)
		float x = xx;
		//(float y = -1.0; y <= 1.0; y++) {;

		for (int yy = -1; yy < 2; ++yy) { //yy in range(-1, 2)
			float y = yy;
			Vector2 node = rand2(modv2(o + Vector2(x, y), size)) + Vector2(x, y);
			float dist = MAX(abs((f - node).x), abs((f - node).y));
			min_dist = MIN(min_dist, dist);
		}
	}

	return min_dist;
}

//float fbm_cellular6(vec2 coord, vec2 size, float seed) {;
//	vec2 o = Math::floor(coord)+rand2(vec2(seed, 1.0-seed))+size;
//	vec2 f = fract(coord);
//	float min_dist1 = 2.0;
//	float min_dist2 = 2.0;
//;
//	for(float x = -1.0; x <= 1.0; x++) {;
//		for(float y = -1.0; y <= 1.0; y++) {;
//			vec2 node = rand2(mod(o + vec2(x, y), size)) + vec2(x, y);
//			float dist = MAX(abs((f - node).x), abs((f - node).y));
//;
//			if (min_dist1 > dist) {;
//				min_dist2 = min_dist1;
//				min_dist1 = dist;
//			} else if (min_dist2 > dist) {;
//				min_dist2 = dist;
//			};
//		};
//	};
//;
//	return min_dist2-min_dist1;
//};

float MMAlgos::fbm_cellular6(const Vector2 &coord, const Vector2 &size, const float pseed) {
	Vector2 o = floorv2(coord) + rand2(Vector2(float(pseed), 1.0 - float(pseed))) + size;
	Vector2 f = fractv2(coord);
	float min_dist1 = 2.0;
	float min_dist2 = 2.0;
	//(float x = -1.0; x <= 1.0; x++) {;

	for (int xx = -1; xx < 2; ++xx) { //xx in range(-1, 2)
		float x = xx;
		//(float y = -1.0; y <= 1.0; y++) {;

		for (int yy = -1; yy < 2; ++yy) { //yy in range(-1, 2)
			float y = yy;
			Vector2 node = rand2(modv2(o + Vector2(x, y), size)) + Vector2(x, y);
			float dist = MAX(abs((f - node).x), abs((f - node).y));

			if ((min_dist1 > dist)) {
				min_dist2 = min_dist1;
				min_dist1 = dist;
			} else if ((min_dist2 > dist)) {
				min_dist2 = dist;
			}
		}
	}

	return min_dist2 - min_dist1;
}

// ===== NOISE_PERLIN.GD ========;
//----------------------;
//perlin.mmg;
//Outputs:;
//Output - (float) - Shows a greyscale value noise;
//perlin($(uv), vec2($(scale_x), $(scale_y)), int($(iterations)), $(persistence), $(seed));
//Inputs:;
//scale, vector2, default: 4, min: 1, max: 32, step: 1;
//iterations, float, min: 0, max: 10, default: 3, step:1;
//persistence, float, min: 0, max: 1, default: 0.5, step:0.05;
//----------------------;
//perlin_color.mmg;
//Outputs:;
//Output - (rgb) - Shows a color value noise;
//perlin_color($(uv), vec2($(scale_x), $(scale_y)), int($(iterations)), $(persistence), $(seed));
//Inputs:;
//scale, vector2, default: 4, min: 1, max: 32, step: 1;
//iterations, float, min: 0, max: 10, default: 3, step:1;
//persistence, float, min: 0, max: 1, default: 0.5, step:0.05;

Color MMAlgos::perlinc(const Vector2 &uv, const Vector2 &size, const int iterations, const float persistence, const int pseed) {
	float f = perlin2c(uv, size, iterations, persistence, pseed);
	return Color(f, f, f, 1);
}

//float perlin(vec2 uv, vec2 size, int iterations, float persistence, float seed) {;
//	vec2 seed2 = rand2(vec2(seed, 1.0-seed));
//	float rv = 0.0;
//	float coef = 1.0;
//	float acc = 0.0;
//;
//	for (int i = 0; i < iterations; ++i) {;
//		vec2 step = vec2(1.0)/size;
//		vec2 xy = Math::floor(uv*size);
//;
//		float f0 = rand(seed2+mod(xy, size));
//		float f1 = rand(seed2+mod(xy+vec2(1.0, 0.0), size));
//		float f2 = rand(seed2+mod(xy+vec2(0.0, 1.0), size));
//		float f3 = rand(seed2+mod(xy+vec2(1.0, 1.0), size));
//;
//		vec2 mixval = Math::smoothstep(0.0, 1.0, fract(uv*size));
//		rv += coef * mix(mix(f0, f1, mixval.x), mix(f2, f3, mixval.x), mixval.y);
//		acc += coef;
//		size *= 2.0;
//		coef *= persistence;
//	};
//;
//	return rv / acc;
//};

float MMAlgos::perlin2c(const Vector2 &uv, const Vector2 &p_size, const int iterations, const float persistence, const int pseed) {
	Vector2 size = p_size;
	Vector2 seed2 = rand2(Vector2(float(pseed), 1.0 - float(pseed)));
	float rv = 0.0;
	float coef = 1.0;
	float acc = 0.0;

	for (int i = 0; i < iterations; ++i) { //i in range(iterations)
		//Vector2 step = Vector2(1, 1) / size;
		Vector2 xy = floorv2(uv * size);
		float f0 = rand(seed2 + modv2(xy, size));
		float f1 = rand(seed2 + modv2(xy + Vector2(1.0, 0.0), size));
		float f2 = rand(seed2 + modv2(xy + Vector2(0.0, 1.0), size));
		float f3 = rand(seed2 + modv2(xy + Vector2(1.0, 1.0), size));
		Vector2 mixval = smoothstepv2(0.0, 1.0, fractv2(uv * size));
		rv += coef * Math::lerp(Math::lerp(f0, f1, mixval.x), Math::lerp(f2, f3, mixval.x), mixval.y);
		acc += coef;
		size *= Vector2(2, 2);
		coef *= persistence;
	}

	return rv / acc;
}

//vec3 perlin_color(vec2 uv, vec2 size, int iterations, float persistence, float seed) {;
//	vec2 seed2 = rand2(vec2(seed, 1.0-seed));
//	vec3 rv = vec3(0.0);
//	float coef = 1.0;
//	float acc = 0.0;
//;
//	for (int i = 0; i < iterations; ++i) {;
//		vec2 step = vec2(1.0)/size;
//		vec2 xy = Math::floor(uv*size);
//		vec3 f0 = rand3(seed2+mod(xy, size));
//		vec3 f1 = rand3(seed2+mod(xy+vec2(1.0, 0.0), size));
//		vec3 f2 = rand3(seed2+mod(xy+vec2(0.0, 1.0), size));
//		vec3 f3 = rand3(seed2+mod(xy+vec2(1.0, 1.0), size));
//		vec2 mixval = Math::smoothstep(0.0, 1.0, fract(uv*size));
//;
//		rv += coef * mix(mix(f0, f1, mixval.x), mix(f2, f3, mixval.x), mixval.y);
//		acc += coef;
//		size *= 2.0;
//		coef *= persistence;
//	};
//;
//	return rv / acc;
//};

Vector3 MMAlgos::perlin_color(const Vector2 &uv, const Vector2 &p_size, const int iterations, const float persistence, const int pseed) {
	Vector2 size = p_size;
	Vector2 seed2 = rand2(Vector2(float(pseed), 1.0 - float(pseed)));
	Vector3 rv = Vector3();
	float coef = 1.0;
	float acc = 0.0;

	for (int i = 0; i < iterations; ++i) { //i in range(iterations)
		//Vector2 step = Vector2(1, 1) / size;
		Vector2 xy = floorv2(uv * size);
		Vector3 f0 = rand3(seed2 + modv2(xy, size));
		Vector3 f1 = rand3(seed2 + modv2(xy + Vector2(1.0, 0.0), size));
		Vector3 f2 = rand3(seed2 + modv2(xy + Vector2(0.0, 1.0), size));
		Vector3 f3 = rand3(seed2 + modv2(xy + Vector2(1.0, 1.0), size));
		Vector2 mixval = smoothstepv2(0.0, 1.0, fractv2(uv * size));
		rv += coef * f0.linear_interpolate(f1, mixval.x).linear_interpolate(f2.linear_interpolate(f3, mixval.x), mixval.y);
		acc += coef;
		size *= Vector2(2, 2);
		coef *= persistence;
	}

	return rv / acc;
}

Color MMAlgos::perlin_colorc(const Vector2 &uv, const Vector2 &size, const int iterations, const float persistence, const int pseed) {
	Vector3 f = perlin_color(uv, size, iterations, persistence, pseed);
	return Color(f.x, f.y, f.z, 1);
}

Color MMAlgos::perlin_warp_1(const Vector2 &uv, const Vector2 &size, const int iterations, const float persistence, const int pseed, const Vector2 &translate, const float rotate, const Vector2 &size2) {
	float f = perlin2c(uv, size2, iterations, persistence, pseed);
	Vector2 vt = transform(uv, Vector2(translate.x * (2.0 * f - 1.0), translate.y * (2.0 * f - 1.0)), rotate * 0.01745329251 * (2.0 * 1.0 - 1.0), Vector2(size.x * (2.0 * 1.0 - 1.0), size.y * (2.0 * 1.0 - 1.0)), true);
	float ff = perlin2c(vt, size2, iterations, persistence, pseed);
	return Color(ff, ff, ff, 1);
}

Color MMAlgos::perlin_warp_2(const Vector2 &uv, const Vector2 &size, const int iterations, const float persistence, const int pseed, const Vector2 &translate, const float rotate, const Vector2 &size2) {
	float f = perlin2c(uv, size2, iterations, persistence, pseed);
	Vector2 vt = transform(uv, Vector2(translate.x * (2.0 * f - 1.0), translate.y * (2.0 * f - 1.0)), rotate * 0.01745329251 * (2.0 * 1.0 - 1.0), Vector2(size.x * (2.0 * 1.0 - 1.0), size.y * (2.0 * 1.0 - 1.0)), true);
	float ff = perlin2c(vt, size2, iterations, persistence, pseed);
	Vector3 rgba = Vector3(ff, ff, ff);
	Vector2 tf = transform(uv, Vector2(translate.x * (2.0 * (rgba.dot(Vector3(1, 1, 1) / 3.0) - 1.0)), translate.y * (2.0 * (rgba.dot(Vector3(1, 1, 1) / 3.0) - 1.0))), rotate * 0.01745329251 * (2.0 * 1.0 - 1.0), Vector2(size.x * (2.0 * 1.0 - 1.0), size.y * (2.0 * 1.0 - 1.0)), true);
	float fff = perlin2c(tf, size2, iterations, persistence, pseed);
	return Color(fff, fff, fff, 1);
}

// ========= NOIDE_VORONOI.GD =========;
//voronoi.mmg;
//voronoi_1, 2, 3, 4 -> different outputs;
//Outputs:;
//vec4 $(name_uv)_xyzw = voronoi($uv, vec2($scale_x, $scale_y), vec2($stretch_y, $stretch_x), $intensity, $randomness, $seed);
//Nodes - float - A greyscale pattern based on the distance to cell centers;
//$(name_uv)_xyzw.z;
//Borders - float - A greyscale pattern based on the distance to borders;
//$(name_uv)_xyzw.w;
//Random color - rgb - A color pattern that assigns a random color to each cell;
//rand3(fract(floor($(name_uv)_xyzw.xy)/vec2($scale_x, $scale_y)));
//Fill - rgba - An output that should be plugged into a Fill companion node;
//vec4(fract(($(name_uv)_xyzw.xy-1.0)/vec2($scale_x, $scale_y)), vec2(2.0)/vec2($scale_x, $scale_y));
//Inputs:;
//scale, min: 1, max: 32, step: 1, default: 4;
//stretch, min: 0.01, max: 1, step: 0.01, default: 1;
//intensity, min: 0, max: 1, step: 0.01, default: 0.75;
//randomness, min: 0, max: 1, step: 0.01, default: 1;
//vec4 $(name_uv)_xyzw = voronoi($uv, vec2($scale_x, $scale_y), vec2($stretch_y, $stretch_x), $intensity, $randomness, $seed);
//note this is newer than what I have TODO;
//// Based on https://www.shadertoy.com/view/ldl3W8;
//// The MIT License;
//// Copyright © 2013 Inigo Quilez;
//vec3 iq_voronoi(vec2 x, vec2 size, vec2 stretch, float randomness, vec2 seed) {;
//	vec2 n = Math::floor(x);
//	vec2 f = fract(x);
//	vec2 mg, mr, mc;
//	float md = 8.0;
//;
//	for (int j=-1; j<=1; j++);
//		for (int i=-1; i<=1; i++) {;
//			vec2 g = vec2(float(i),float(j));
//			vec2 o = randomness*rand2(seed + mod(n + g + size, size));
//			vec2 c = g + o;
//			vec2 r = c - f;
//			vec2 rr = r*stretch;
//			float d = dot(rr,rr);
//			if (d<md) {;
//				mc = c;
//				md = d;
//				mr = r;
//				mg = g;
//			};
//		};
//;
//	md = 8.0;
//;
//	for (int j=-2; j<=2; j++);
//		for (int i=-2; i<=2; i++) {;
//			vec2 g = mg + vec2(float(i),float(j));
//			vec2 o = randomness*rand2(seed + mod(n + g + size, size));
//			vec2 r = g + o - f;
//			vec2 rr = (mr-r)*stretch;
//;
//			if (dot(rr,rr)>0.00001);
//				md = MIN(md, dot(0.5*(mr+r)*stretch, normalize((r-mr)*stretch)));
//		};
//;
//	return vec3(md, mc+n);
//};
//;
//vec4 voronoi(vec2 uv, vec2 size, vec2 stretch, float intensity, float randomness, float seed) {;
//	uv *= size;
//	vec3 v = iq_voronoi(uv, size, stretch, randomness, rand2(vec2(seed, 1.0-seed)));
//	return vec4(v.yz, intensity*length((uv-v.yz)*stretch), v.x);
//};

Color MMAlgos::voronoi(const Vector2 &uuv, const Vector2 &size, const Vector2 &stretch, const float intensity, const float randomness, const int pseed) {
	Vector2 uv = uuv;
	Vector2 seed2 = rand2(Vector2(float(pseed), 1.0 - float(pseed)));
	uv *= size;
	float best_distance0 = 1.0;
	float best_distance1 = 1.0;
	Vector2 point0;
	Vector2 point1;
	Vector2 p0 = floorv2(uv);
	// (int dx = -1; dx < 2; ++dx) {;

	for (int dx = -1; dx < 2; ++dx) { //dx in range(-1, 2)
		// (int dy = -1; dy < 2; ++dy) {;

		for (int dy = -1; dy < 2; ++dy) { //dy in range(-1, 2)
			Vector2 d = Vector2(float(dx), float(dy));
			Vector2 p = p0 + d;
			p += randomness * rand2(seed2 + modv2(p, size));
			float distance = (stretch * (uv - p) / size).length();

			if ((best_distance0 > distance)) {
				best_distance1 = best_distance0;
				best_distance0 = distance;
				point1 = point0;
				point0 = p;
			} else if ((best_distance1 > distance)) {
				best_distance1 = distance;
				point1 = p;
			}
		}
	}

	float edge_distance = (uv - 0.5 * (point0 + point1)).dot((point0 - point1).normalized());
	return Color(point0.x, point0.y, best_distance0 * (size).length() * intensity, edge_distance);
}

//$(name_uv)_xyzw.z;

Color MMAlgos::voronoi_1(const Vector2 &uv, const Vector2 &size, const Vector2 &stretch, const float intensity, const float randomness, const int pseed) {
	Color c = voronoi(uv, size, stretch, intensity, randomness, pseed);
	return Color(c.b, c.b, c.b, 1);
}

//$(name_uv)_xyzw.w;

Color MMAlgos::voronoi_2(const Vector2 &uv, const Vector2 &size, const Vector2 &stretch, const float intensity, const float randomness, const int pseed) {
	Color c = voronoi(uv, size, stretch, intensity, randomness, pseed);
	return Color(c.a, c.a, c.a, 1);
}

//rand3(fract(floor($(name_uv)_xyzw.xy)/vec2($scale_x, $scale_y)));

Color MMAlgos::voronoi_3(const Vector2 &uv, const Vector2 &size, const Vector2 &stretch, const float intensity, const float randomness, const int pseed) {
	Color c = voronoi(uv, size, stretch, intensity, randomness, pseed);
	Vector2 vv = Vector2(c.r, c.g);
	Vector3 v = rand3(fractv2(vv));
	return Color(v.x, v.y, v.z, 1);
}

//vec4(fract(($(name_uv)_xyzw.xy-1.0)/vec2($scale_x, $scale_y)), vec2(2.0)/vec2($scale_x, $scale_y));
//voronoi_4 todo;
// =================  PATTERNS.GD =======================;
//----------------------;
//beehive.mmg;
//Outputs: (beehive_1c, beehive_2c, beehive_3c  TODO make common code parameters);
//Common;
//vec2 $(name_uv)_uv = $uv*vec2($sx, $sy*1.73205080757);
//vec4 $(name_uv)_center = beehive_center($(name_uv)_uv);
//Output (float) - Shows the greyscale pattern;
//1.0-2.0*beehive_dist($(name_uv)_center.xy);
//Random color (rgb) - Shows a random color for each hexagonal tile;
//rand3(fract($(name_uv)_center.zw/vec2($sx, $sy))+vec2(float($seed)));
//UV map (rgb) - Shows an UV map to be connected to the UV map port of the Custom UV node;
//vec3(vec2(0.5)+$(name_uv)_center.xy, rand(fract($(name_uv)_center.zw/vec2($sx, $sy))+vec2(float($seed))));
//Inputs:;
//size, vector2, default: 2, min: 1, max: 64, step: 1;
//----------------------;
//pattern.mmg;
//Outputs: $(name)_fct($(uv));
//Combiner, enum, default: 0, values (COMBINER_TYPE_: Multiply, Add, Max, Min, Xor, Pow;
//Pattern_x_type, enum, default: 5, values (COMBINER_AXIS_TYPE_: Sine, Triangle, Square, Sawtooth, Constant, Bounce;
//Pattern_y_type, enum, default: 5, values (COMBINER_AXIS_TYPE_: Sine, Triangle, Square, Sawtooth, Constant, Bounce;
//Pattern_Repeat, vector2, min: 0, max: 32, default:4, step: 1;
//----------------------;
//bricks.mmg;
//Outputs:;
//Common;
//vec4 $(name_uv)_rect = bricks_$pattern($uv, vec2($columns, $rows), $repeat, $row_offset);
//vec4 $(name_uv) = brick($uv, $(name_uv)_rect.xy, $(name_uv)_rect.zw, $mortar*$mortar_map($uv), $round*$round_map($uv), MAX(0.001, $bevel*$bevel_map($uv)));
//Bricks pattern (float) - A greyscale image that shows the bricks pattern;
//$(name_uv).x;
//Random color (rgb) - A random color for each brick;
//brick_random_color($(name_uv)_rect.xy, $(name_uv)_rect.zw, float($seed));
//Position.x (float) - The position of each brick along the X axis",;
//$(name_uv).y;
//Position.y (float) - The position of each brick along the Y axis;
//$(name_uv).z;
//Brick UV (rgb) - An UV map output for each brick, to be connected to the Map input of a CustomUV node;
//brick_uv($uv, $(name_uv)_rect.xy, $(name_uv)_rect.zw, float($seed));
//Corner UV (rgb) - An UV map output for each brick corner, to be connected to the Map input of a CustomUV node;
//brick_corner_uv($uv, $(name_uv)_rect.xy, $(name_uv)_rect.zw, $mortar*$mortar_map($uv), $corner, float($seed));
//Direction (float) - The direction of each brick (white: horizontal, black: vertical);
//0.5*(sign($(name_uv)_rect.z-$(name_uv)_rect.x-$(name_uv)_rect.w+$(name_uv)_rect.y)+1.0);
//Inputs:;
//type / Pattern, enum, default: 0, values: Running Bond,Running Bond (2),HerringBone,Basket Weave,Spanish Bond;
//repeat, int, min: 1, max: 8, default: 1, step:1;
//rows, int, min: 1, max: 64, default: 6, step:1;
//columns, int, min: 1, max: 64, default: 6, step:1;
//offset, float, min: 0, max: 1, default: 0.5, step:0.01;
//mortar, float, min: 0, max: 0.5, default: 0.1, step:0.01 (universal input);
//bevel, float, min: 0, max: 0.5, default: 0.1, step:0.01 (universal input);
//round, float, min: 0, max: 0.5, default: 0.1, step:0.01 (universal input);
//corner, float, min: 0, max: 0.5, default: 0.1, step:0.01;
//----------------------;
//bricks_uneven.mmg;
//Outputs:;
//Common;
//vec4 $(name_uv)_rect = bricks_uneven($uv, int($iterations), $min_size, $randomness, float($seed));
//vec4 $(name_uv) = brick2($uv, $(name_uv)_rect.xy, $(name_uv)_rect.zw, $mortar*$mortar_map($uv), $round*$round_map($uv), MAX(0.00001, $bevel*$bevel_map($uv)));
//Bricks pattern (float) - A greyscale image that shows the bricks pattern;
//$(name_uv).x;
//Random color (rgb) - A random color for each brick;
//rand3(fract($(name_uv)_rect.xy)+rand2(vec2(float($seed))));
//Position.x (float) - The position of each brick along the X axis",;
//$(name_uv).y;
//Position.y (float) - The position of each brick along the Y axis;
//$(name_uv).z;
//Brick UV (rgb) - An UV map output for each brick, to be connected to the Map input of a CustomUV node;
//brick_uv($uv, $(name_uv)_rect.xy, $(name_uv)_rect.zw, float($seed));
//Corner UV (rgb) - An UV map output for each brick corner, to be connected to the Map input of a CustomUV node;
//brick_corner_uv($uv, $(name_uv)_rect.xy, $(name_uv)_rect.zw, $mortar*$mortar_map($uv), $corner, float($seed));
//Direction (float) - The direction of each brick (white: horizontal, black: vertical);
//0.5*(sign($(name_uv)_rect.z-$(name_uv)_rect.x-$(name_uv)_rect.w+$(name_uv)_rect.y)+1.0);
//Inputs:;
//iterations, int, min: 1, max: 16, default:8, step:1;
//min_size, float, min: 0, max: 0.5, default: 0.3, step:0.01;
//randomness, float, min: 0, max: 1, default: 0.5, step:0.01;
//mortar, float, min: 0, max: 0.5, default: 0.1, step:0.01 (universal input);
//bevel, float, min: 0, max: 0.5, default: 0.1, step:0.01 (universal input);
//round, float, min: 0, max: 0.5, default: 0.1, step:0.01 (universal input);
//corner, float, min: 0, max: 0.5, default: 0.1, step:0.01;
//----------------------;
//runes.mmg (includes sdline.mmg);
//Generates a grid filled with random runes;
//Outputs:;
//Output (float) - A greyscale image showing random runes.;
//Rune(vec2($columns, $rows)*$uv, float($seed));
//Inputs:;
//size, vector2, default: 4, min: 2, max: 32, step: 1;
//----------------------;
//scratches.mmg;
//Draws white scratches on a black background;
//Outputs:;
//Output (float) - Shows white scratches on a black background;
//scratches($uv, int($layers), vec2($length, $width), $waviness, $angle, $randomness, vec2(float($seed), 0.0));
//Inputs:;
//scratch_size (l, w), vector2, min: 0.1, max: 1, default: (0.25, 0.5), step:0.01;
//layers, float, min: 1, max: 10, default: 4, step:1;
//waviness, float, min: 0, max: 1, default: 0.5, step:0.01;
//angle, float, min: -180, max: 180, default: 0, step:1;
//randomness, float, min: 0, max: 1, default: 0.5, step:0.01;
//----------------------;
//iching.mmg;
//This node generates a grid of random I Ching hexagrams.;
//Outputs:;
//Output (float) - A greyscale image showing random I Ching hexagrams.;
//IChing(vec2($columns, $rows)*$uv, float($seed));
//Inputs:;
//size, vector2, default: 2, min: 2, max: 32, step: 1;
//----------------------;
//weave.mmg;
//Outputs:;
//Output (float) - Shows the generated greyscale weave pattern.;
//weave($uv, vec2($columns, $rows), $width*$width_map($uv));
//Inputs:;
//size, vector2, default: 4, min: 2, max: 32, step: 1;
//width, float, min: 0, max: 1, default: 0.8, step:0.05 (universal input);
//----------------------;
//weave2.mmg;
//code;
//vec3 $(name_uv) = weave2($uv, vec2($columns, $rows), $stitch, $width_x*$width_map($uv), $width_y*$width_map($uv));
//Outputs:;
//Output (float) - Shows the generated greyscale weave pattern.;
//$(name_uv).x;
//Horizontal mask (float) - Horizontal mask;
//$(name_uv).y;
//Vertical mask (float) - Mask for vertical stripes;
//$(name_uv).z;
//Inputs:;
//size, vector2, default: 4, min: 2, max: 32, step: 1;
//width, vector2, default: 0.8, min: 0, max: 1, step: 0.05;
//stitch, float, min: 0, max: 10, default: 1, step:1;
//width_map, float, default: 1, (does not need input val (label)) (universal input);
//----------------------;
//truchet.mmg;
//Outputs:;
//line: $shape = 1;
//circle: $shape = 2;
//Output (float) - Shows a greyscale image of the truchet pattern.;
//truchet$shape($uv*$size, vec2(float($seed)));
//Inputs:;
//shape, enum, default: 0, values: line, circle;
//size, float, default: 4, min: 2, max: 64, step: 1;
//----------------------;
//truchet_generic.mmg;
//Outputs:;
//Output (color);
//$in(truchet_generic_uv($uv*$size, vec2(float($seed))));
//Inputs:;
//in, color, default: color(1.0);
//size, float, default: 4, min: 2, max: 64, step: 1;
//----------------------;
//arc_pavement.mmg;
//----------------------;
//sine_wave.mmg;
//Draws a greyscale sine wave pattern;
//Outputs:;
//Output, float, Shows a greyscale image of a sine wave;
//1.0-abs(2.0*($uv.y-0.5)-$amplitude*sin(($frequency*$uv.x+$phase)*6.28318530718));
//Inputs:;
//amplitude, float, min: 0, max: 1, step: 0.01, default: 0.5;
//frequency, float, min: 0, max: 16, default: 1;
//phase, float, min: 0, max: 1, step: 0.01, default: 0.5;
//"Sine,Triangle,Square,Sawtooth,Constant,Bounce";
//"Multiply,Add,Max,Min,Xor,Pow";
//float $(name)_fct(vec2 uv) {;
//	return mix_$(mix)(wave_$(x_wave)($(x_scale)*uv.x), wave_$(y_wave)($(y_scale)*uv.y));
//};

float MMAlgos::pattern(const Vector2 &uv, const float x_scale, const float y_scale, const int ct, const int catx, const int caty) {
	float x = 0;
	float y = 0;

	if (catx == COMBINER_AXIS_TYPE_SINE) {
		x = wave_sine(x_scale * uv.x);
	} else if (catx == COMBINER_AXIS_TYPE_TRIANGLE) {
		x = wave_triangle(x_scale * uv.x);
	} else if (catx == COMBINER_AXIS_TYPE_SQUARE) {
		x = wave_square(x_scale * uv.x);
	} else if (catx == COMBINER_AXIS_TYPE_SAWTOOTH) {
		x = wave_sawtooth(x_scale * uv.x);
	} else if (catx == COMBINER_AXIS_TYPE_CONSTANT) {
		x = wave_constant(x_scale * uv.x);
	} else if (catx == COMBINER_AXIS_TYPE_BOUNCE) {
		x = wave_bounce(x_scale * uv.x);
	}

	if (caty == COMBINER_AXIS_TYPE_SINE) {
		y = wave_sine(y_scale * uv.y);
	} else if (caty == COMBINER_AXIS_TYPE_TRIANGLE) {
		y = wave_triangle(y_scale * uv.y);
	} else if (caty == COMBINER_AXIS_TYPE_SQUARE) {
		y = wave_square(y_scale * uv.y);
	} else if (caty == COMBINER_AXIS_TYPE_SAWTOOTH) {
		y = wave_sawtooth(y_scale * uv.y);
	} else if (caty == COMBINER_AXIS_TYPE_CONSTANT) {
		y = wave_constant(y_scale * uv.y);
	} else if (caty == COMBINER_AXIS_TYPE_BOUNCE) {
		y = wave_bounce(y_scale * uv.y);
	}

	if (ct == COMBINER_TYPE_MULTIPLY) {
		return mix_mul(x, y);
	} else if (ct == COMBINER_TYPE_ADD) {
		return mix_add(x, y);
	} else if (ct == COMBINER_TYPE_MAX) {
		return mix_max(x, y);
	} else if (ct == COMBINER_TYPE_MIN) {
		return mix_min(x, y);
	} else if (ct == COMBINER_TYPE_XOR) {
		return mix_xor(x, y);
	} else if (ct == COMBINER_TYPE_POW) {
		return mix_pow(x, y);
	}

	return 0.0;
}

//"Line,Circle";

Color MMAlgos::truchet1c(const Vector2 &uv, const float size, const float pseed) {
	float f = truchet1(uv * size, Vector2(pseed, pseed));
	return Color(f, f, f, 1);
}

//float truchet1(vec2 uv, vec2 seed) {;
//	vec2 i = Math::floor(uv);
//	vec2 f = fract(uv)-vec2(0.5);
//;
//	return 1.0-abs(abs((2.0*step(rand(i+seed), 0.5)-1.0)*f.x+f.y)-0.5);
//};

float MMAlgos::truchet1(const Vector2 &uv, const Vector2 &pseed) {
	Vector2 i = floorv2(uv);
	Vector2 f = fractv2(uv) - Vector2(0.5, 0.5);
	return 1.0 - abs(abs((2.0 * step(rand(i + pseed), 0.5) - 1.0) * f.x + f.y) - 0.5);
}

Color MMAlgos::truchet2c(const Vector2 &uv, const float size, const float pseed) {
	float f = truchet2(uv * size, Vector2(pseed, pseed));
	return Color(f, f, f, 1);
}

//float truchet2(vec2 uv, vec2 seed) {;
//	vec2 i = Math::floor(uv);
//	vec2 f = fract(uv);
//	float random = step(rand(i+seed), 0.5);
//;
//	f.x *= 2.0*random-1.0;
//	f.x += 1.0-random;
//;
//	return 1.0-min(abs(length(f)-0.5), abs(length(1.0-f)-0.5));
//};

float MMAlgos::truchet2(const Vector2 &uv, const Vector2 &pseed) {
	Vector2 i = floorv2(uv);
	Vector2 f = fractv2(uv);
	float random = step(rand(i + pseed), 0.5);
	f.x *= 2.0 * random - 1.0;
	f.x += 1.0 - random;
	return 1.0 - MIN(abs(f.length() - 0.5), abs((Vector2(1, 1) - f).length() - 0.5));
}

Color MMAlgos::weavec(const Vector2 &uv, const Vector2 &count, const float width) {
	float f = weave(uv, count, width);
	return Color(f, f, f, 1);
}

//float weave(vec2 uv, vec2 count, float width) {;
//	uv *= count;
//	float c = (sin(3.1415926*(uv.x+floor(uv.y)))*0.5+0.5)*step(abs(fract(uv.y)-0.5), width*0.5);
//	c = MAX(c, (sin(3.1415926*(1.0+uv.y+floor(uv.x)))*0.5+0.5)*step(abs(fract(uv.x)-0.5), width*0.5));
//	return c;
//};

float MMAlgos::weave(const Vector2 &uuv, const Vector2 &count, const float width) {
	Vector2 uv = uuv;
	uv *= count;
	float c = (sin(3.1415926 * (uv.x + Math::floor(uv.y))) * 0.5 + 0.5) * step(abs(fract(uv.y) - 0.5), width * 0.5);
	c = MAX(c, (sin(3.1415926 * (1.0 + uv.y + floor(uv.x))) * 0.5 + 0.5) * step(abs(fract(uv.x) - 0.5), width * 0.5));
	return c;
}

//vec3 weave2(vec2 uv, vec2 count, float stitch, float width_x, float width_y) {;
//	uv *= stitch;
//	uv *= count;
//	float c1 = (sin(3.1415926 / stitch * (uv.x + Math::floor(uv.y) - (stitch - 1.0))) * 0.25 + 0.75 ) *step(abs(fract(uv.y)-0.5), width_x*0.5);
//	float c2 = (sin(3.1415926 / stitch * (1.0+uv.y+floor(uv.x) ))* 0.25 + 0.75 )*step(abs(fract(uv.x)-0.5), width_y*0.5);
//	return vec3(MAX(c1, c2), 1.0-step(c1, c2), 1.0-step(c2, c1));
//};

Vector3 MMAlgos::weave2(const Vector2 &uuv, const Vector2 &count, const float stitch, const float width_x, const float width_y) {
	Vector2 uv = uuv;
	uv.x *= stitch;
	uv.y *= stitch;
	uv *= count;
	float c1 = (sin(3.1415926 / stitch * (uv.x + Math::floor(uv.y) - (stitch - 1.0))) * 0.25 + 0.75) * step(abs(fract(uv.y) - 0.5), width_x * 0.5);
	float c2 = (sin(3.1415926 / stitch * (1.0 + uv.y + Math::floor(uv.x))) * 0.25 + 0.75) * step(abs(fract(uv.x) - 0.5), width_y * 0.5);
	return Vector3(MAX(c1, c2), 1.0 - step(c1, c2), 1.0 - step(c2, c1));
}

Color MMAlgos::sinewavec(const Vector2 &uv, const float amplitude, const float frequency, const float phase) {
	float f = 1.0 - abs(2.0 * (uv.y - 0.5) - amplitude * sin((frequency * uv.x + phase) * 6.28318530718));
	return Color(f, f, f, 1);
}

float MMAlgos::sinewavef(const Vector2 &uv, const float amplitude, const float frequency, const float phase) {
	return 1.0 - abs(2.0 * (uv.y - 0.5) - amplitude * sin((frequency * uv.x + phase) * 6.28318530718));
}

//float scratch(vec2 uv, vec2 size, float waviness, float angle, float randomness, vec2 seed) {;
//	float subdivide = Math::floor(1.0/size.x);
//	float cut = size.x*subdivide;
//;
//	uv *= subdivide;
//;
//	vec2 r1 = rand2(floor(uv)+seed);
//	vec2 r2 = rand2(r1);
//;
//	uv = fract(uv);
//	vec2 border = 10.0*min(fract(uv), 1.0-fract(uv));
//	uv = 2.0*uv-vec2(1.0);
//;
//	float a = 6.28318530718*(angle+(r1.x-0.5)*randomness);
//	float c = cos(a);
//	float s = sin(a);
//;
//	uv = vec2(c*uv.x+s*uv.y, s*uv.x-c*uv.y);
//	uv.y += 2.0*r1.y-1.0;
//	uv.y += 0.5*waviness*cos(2.0*uv.x+6.28318530718*r2.y);
//	uv.x /= cut;
//	uv.y /= subdivide*size.y;
//;
//	return MIN(border.x, border.y)*(1.0-uv.x*uv.x)*MAX(0.0, 1.0-1000.0*uv.y*uv.y);
//};

float MMAlgos::scratch(const Vector2 &uuv, const Vector2 &size, const float waviness, const float angle, const float randomness, const Vector2 &pseed) {
	Vector2 uv = uuv;
	float subdivide = Math::floor(1.0 / size.x);
	float cut = size.x * subdivide;
	uv *= subdivide;
	Vector2 r1 = rand2(floorv2(uv) + pseed);
	Vector2 r2 = rand2(r1);
	uv = fractv2(uv);
	uv = 2.0 * uv - Vector2(1, 1);
	float a = 6.28 * (angle + (r1.x - 0.5) * randomness);
	float c = cos(a);
	float s = sin(a);
	uv = Vector2(c * uv.x + s * uv.y, s * uv.x - c * uv.y);
	uv.y += 2.0 * r1.y - 1.0;
	uv.y += 0.5 * waviness * cos(2.0 * uv.x + 6.28 * r2.y);
	uv.x /= cut;
	uv.y /= subdivide * size.y;
	return (1.0 - uv.x * uv.x) * MAX(0.0, 1.0 - 1000.0 * uv.y * uv.y);
}

//float scratches(vec2 uv, int layers, vec2 size, float waviness, float angle, float randomness, vec2 seed) {;
//	float v = 0.0;
//;
//	for (int i = 0; i < layers; ++i) {;
//		seed = rand2(seed);
//		v = MAX(v, scratch(fract(uv+seed), size, waviness, angle/360.0, randomness, seed));
//	};
//;
//	return v;
//};

float MMAlgos::scratches(const Vector2 &uv, const int layers, const Vector2 &size, const float waviness, const float angle, const float randomness, const Vector2 &p_pseed) {
	Vector2 pseed = p_pseed;
	float v = 0.0;
	// (int i = 0; i < layers; ++i) {;

	for (int i = 0; i < layers; ++i) { //i in range(layers)
		pseed = rand2(pseed);
		v = MAX(v, scratch(fractv2(uv + pseed), size, waviness, angle / 360.0, randomness, pseed));
	}

	return v;
}

Color MMAlgos::scratchesc(const Vector2 &uv, const int layers, const Vector2 &size, const float waviness, const float angle, const float randomness, const Vector2 &pseed) {
	float f = scratches(uv, layers, size, waviness, angle, randomness, pseed);
	return Color(f, f, f, 1);
}

Color MMAlgos::runesc(const Vector2 &uv, const Vector2 &col_row, const float pseed) {
	float f = rune(col_row * uv, pseed);
	return Color(f, f, f, 1);
}

float MMAlgos::runesf(const Vector2 &uv, const Vector2 &col_row, const float pseed) {
	return rune(col_row * uv, pseed);
}

//sdline.mmg;
//vec2 sdLine(vec2 p, vec2 a, vec2 b) {;
//	vec2 pa = p-a, ba = b-a;
//	float h = CLAMP(dot(pa,ba)/dot(ba,ba), 0.0, 1.0);
//;
//	return vec2(length(pa-ba*h), h);
//};
//float ThickLine(vec2 uv, vec2 posA, vec2 posB, float radiusInv){;
//	return CLAMP(1.1-20.0*sdLine(uv, posA, posB).x, 0.0, 1.0);
//};
//// makes a rune in the 0..1 uv space. Seed is which rune to draw.;
//// passes back gray in x and derivates for lighting in yz;
//float Rune(vec2 uv, float s) {;
//	float finalLine = 0.0;
//	vec2 seed = Math::floor(uv)-rand2(vec2(s));
//	uv = fract(uv);
//;
//	for (int i = 0; i < 4; i++)	// number of strokes;
//	{;
//		vec2 posA = rand2(floor(seed+0.5));
//		vec2 posB = rand2(floor(seed+1.5));
//		seed += 2.0;
//		// expand the range and mod it to get a nicely distributed random number - hopefully. :);
//		posA = fract(posA * 128.0);
//		posB = fract(posB * 128.0);
//		// each rune touches the edge of its box on all 4 sides;
//;
//		if (i == 0) posA.y = 0.0;
//		if (i == 1) posA.x = 0.999;
//		if (i == 2) posA.x = 0.0;
//		if (i == 3) posA.y = 0.999;
//;
//		// snap the random line endpoints to a grid 2x3;
//		vec2 snaps = vec2(2.0, 3.0);
//		posA = (floor(posA * snaps) + 0.5) / snaps; // + 0.5 to center it in a grid cell;
//		posB = (floor(posB * snaps) + 0.5) / snaps;
//;
//		//if (distance(posA, posB) < 0.0001) continue;
//		// eliminate dots.;
//		// Dots (degenerate lines) are not cross-GPU safe without adding 0.001 - divide by 0 error.;
//		finalLine = MAX(finalLine, ThickLine(uv, posA, posB + 0.001, 20.0));
//	};
//;
//	return finalLine;
//};
// makes a rune in the 0..1 uv space. Seed is which rune to draw.;
// passes back gray in x and derivates for lighting in yz;

float MMAlgos::rune(const Vector2 &uuv, const float pseed) {
	Vector2 uv = uuv;
	float finalLine = 0.0;
	Vector2 sseed = floorv2(uv) - Vector2(pseed, pseed);
	uv = fractv2(uv);
	// (int i = 0; i < 4; i++):  //	// number of strokes;

	for (int i = 0; i < 4; ++i) { //i in range(4)
		Vector2 posA = rand2(floorv2(sseed + Vector2(0.5, 0.5)));
		Vector2 posB = rand2(floorv2(sseed + Vector2(1.5, 1.5)));
		sseed.x += 2.0;
		sseed.y += 2.0;
		// expand the range and mod it to get a nicely distributed random number - hopefully. :);
		posA = fractv2(posA * 128.0);
		posB = fractv2(posB * 128.0);
		// each rune touches the edge of its box on all 4 sides;

		if (i == 0) {
			posA.y = 0.0;
		}

		if (i == 1) {
			posA.x = 0.999;
		}

		if (i == 2) {
			posA.x = 0.0;
		}

		if (i == 3) {
			posA.y = 0.999;
		}

		// snap the random line endpoints to a grid 2x3;
		Vector2 snaps = Vector2(2.0, 3.0);
		// + 0.5 to center it in a grid cell;
		posA = (floorv2(posA * snaps) + Vector2(0.5, 0.5)) / snaps;
		posB = (floorv2(posB * snaps) + Vector2(0.5, 0.5)) / snaps;
		//if (distance(posA, posB) < 0.0001) continue;	// eliminate dots.;
		// Dots (degenerate lines) are not cross-GPU safe without adding 0.001 - divide by 0 error.;
		finalLine = MAX(finalLine, ThickLine(uv, posA, posB + Vector2(0.001, 0.001), 20.0));
	}

	return finalLine;
}

Color MMAlgos::IChingc(const Vector2 &uv, const Vector2 &row_col, const int pseed) {
	float f = IChing(row_col * uv, float(pseed));
	return Color(f, f, f, 1);
}

//float IChing(vec2 uv, float seed) {;
//	int value = int(32.0*rand(floor(uv)+vec2(seed)));
//	float base = step(0.5, fract(fract(uv.y)*6.5))*step(0.04, fract(uv.y+0.02))*step(0.2, fract(uv.x+0.1));
//	int bit = int(fract(uv.y)*6.5);
//;
//	return base*step(0.1*step(float(bit & value), 0.5), fract(uv.x+0.55));
//};

float MMAlgos::IChing(const Vector2 &uv, const float pseed) {
	int value = int(32.0 * rand(floorv2(uv) + Vector2(pseed, pseed)));
	float base = step(0.5, fract(fract(uv.y) * 6.5)) * step(0.04, fract(uv.y + 0.02)) * step(0.2, fract(uv.x + 0.1));
	int bit = int(fract(uv.y) * 6.5);
	return base * step(0.1 * step(float(bit & value), 0.5), fract(uv.x + 0.55));
}

//Beehive output 1;
//Shows the greyscale pattern;
//vec2 $(name_uv)_uv = $uv*vec2($sx, $sy*1.73205080757);
//vec4 $(name_uv)_center = beehive_center($(name_uv)_uv);
//1.0-2.0*beehive_dist($(name_uv)_center.xy);

Color MMAlgos::beehive_1c(const Vector2 &uv, const Vector2 &size, const int pseed) {
	Vector2 o80035_0_uv = uv * Vector2(size.x, size.y * 1.73205080757);
	Color center = beehive_center(o80035_0_uv);
	float f = 1.0 - 2.0 * beehive_dist(Vector2(center.r, center.g));
	return Color(f, f, f, 1);
}

//Beehive output 2;
//Shows a random color for each hexagonal tile;
//vec2 $(name_uv)_uv = $uv*vec2($sx, $sy*1.73205080757);
//vec4 $(name_uv)_center = beehive_center($(name_uv)_uv);
//rand3(fract($(name_uv)_center.zw/vec2($sx, $sy))+vec2(float($seed)));

Color MMAlgos::beehive_2c(const Vector2 &uv, const Vector2 &size, const int pseed) {
	Vector2 o80035_0_uv = uv * Vector2(size.x, size.y * 1.73205080757);
	Color center = beehive_center(o80035_0_uv);
	//	float f = 1.0 - 2.0 * beehive_dist(Vector2(center.r, center.g));
	Vector3 v = rand3(fractv2(Vector2(center.b, center.a) / Vector2(size.x, size.y)) + Vector2(float(pseed), float(pseed)));
	return Color(v.x, v.y, v.z, 1);
}

//Beehive output 3;
//Shows an UV map to be connected to the UV map port of the Custom UV node;
//vec3(vec2(0.5)+$(name_uv)_center.xy, rand(fract($(name_uv)_center.zw/vec2($sx, $sy))+vec2(float($seed))));
//vec2 $(name_uv)_uv = $uv*vec2($sx, $sy*1.73205080757);
//vec4 $(name_uv)_center = beehive_center($(name_uv)_uv);

Color MMAlgos::beehive_3c(const Vector2 &uv, const Vector2 &size, const int pseed) {
	Vector2 o80035_0_uv = uv * Vector2(size.x, size.y * 1.73205080757);
	Color center = beehive_center(o80035_0_uv);
	//var f : float = 1.0 - 2.0 * beehive_dist(Vector2(center.r, center.g));
	Vector2 v1 = Vector2(0.5, 0.5) + Vector2(center.r, center.g);
	float ff = rand(fractv2(Vector2(center.b, center.a) / Vector2(size.x, size.y)) + Vector2(float(pseed), float(pseed)));
	Color c = Color(v1.x, v1.y, ff, ff);
	return c;
}

//float beehive_dist(vec2 p){;
//	ec2 s = vec2(1.0, 1.73205080757);
//	p = abs(p);
//	return MAX(dot(p, s*.5), p.x);
//};

float MMAlgos::beehive_dist(const Vector2 &p_p) {
	Vector2 p = p_p;
	Vector2 s = Vector2(1.0, 1.73205080757);
	p = absv2(p);
	return MAX(p.dot(s * .5), p.x);
}

//vec4 beehive_center(vec2 p) {;
//	vec2 s = vec2(1.0, 1.73205080757);
//	vec4 hC = Math::floor(vec4(p, p - vec2(.5, 1)) / vec4(s,s)) + .5;
//	vec4 h = vec4(p - hC.xy*s, p - (hC.zw + .5)*s);
//	return dot(h.xy, h.xy)<dot(h.zw, h.zw) ? vec4(h.xy, hC.xy) : vec4(h.zw, hC.zw + 9.73);
//};

Color MMAlgos::beehive_center(const Vector2 &p) {
	Vector2 s = Vector2(1.0, 1.73205080757);
	Color hC = Color(p.x, p.y, p.x - 0.5, p.y - 1) / Color(s.x, s.y, s.x, s.y);
	hC = floorc(Color(p.x, p.y, p.x - 0.5, p.y - 1) / Color(s.x, s.y, s.x, s.y)) + Color(0.5, 0.5, 0.5, 0.5);
	Vector2 v1 = Vector2(p.x - hC.r * s.x, p.y - hC.g * s.y);
	Vector2 v2 = Vector2(p.x - (hC.b + 0.5) * s.x, p.y - (hC.a + 0.5) * s.y);
	Color h = Color(v1.x, v1.y, v2.x, v2.y);

	if (Vector2(h.r, h.g).dot(Vector2(h.r, h.g)) < Vector2(h.b, h.a).dot(Vector2(h.b, h.a))) {
		return Color(h.r, h.g, hC.r, hC.g);
	} else {
		return Color(h.b, h.a, hC.b + 9.73, hC.a + 9.73);
	}
}

//return dot(h.xy, h.xy) < dot(h.zw, h.zw) ? Color(h.xy, hC.xy) : Color(h.zw, hC.zw + 9.73);
//vec3 brick_corner_uv(vec2 uv, vec2 bmin, vec2 bmax, float mortar, float corner, float seed) {;
//	vec2 center = 0.5*(bmin + bmax);
//	vec2 size = bmax - bmin;
//;
//	float max_size = MAX(size.x, size.y);
//	float min_size = MIN(size.x, size.y);
//;
//	mortar *= min_size;
//	corner *= min_size;
//;
//	return vec3(clamp((0.5*size-vec2(mortar)-abs(uv-center))/corner, vec2(0.0), vec2(1.0)), rand(fract(center)+vec2(seed)+ceil(vec2(uv-center))));
//};

Vector3 MMAlgos::brick_corner_uv(const Vector2 &uv, const Vector2 &bmin, const Vector2 &bmax, const float p_mortar, const float p_corner, const float pseed) {
	float mortar = p_mortar;
	float corner = p_corner;
	Vector2 center = 0.5 * (bmin + bmax);
	Vector2 size = bmax - bmin;
	//	float max_size = MAX(size.x, size.y);
	float min_size = MIN(size.x, size.y);
	mortar *= min_size;
	corner *= min_size;
	Vector3 r = Vector3();
	r.x = CLAMP(((0.5 * size.x - mortar) - abs(uv.x - center.x)) / corner, 0, 1);
	r.y = CLAMP(((0.5 * size.y - mortar) - abs(uv.y - center.y)) / corner, 0, 1);
	r.z = rand(fractv2(center) + Vector2(pseed, pseed));
	return r;
}

//	return vec3(clamp((0.5*size-vec2(mortar)-abs(uv-center))/corner, vec2(0.0), vec2(1.0)), rand(fract(center)+vec2(seed)));
//vec4 brick(vec2 uv, vec2 bmin, vec2 bmax, float mortar, float round, float bevel) {;
//	float color;
//	vec2 size = bmax - bmin;
//	float min_size = MIN(size.x, size.y);
//;
//	mortar *= min_size;
//	bevel *= min_size;
//	round *= min_size;
//	vec2 center = 0.5*(bmin+bmax);
//	vec2 d = abs(uv-center)-0.5*(size)+vec2(round+mortar);
//;
//	color = length(MAX(d,vec2(0))) + MIN(MAX(d.x,d.y),0.0)-round;
//	color = CLAMP(-color/bevel, 0.0, 1.0);
//	vec2 tiled_brick_pos = mod(bmin, vec2(1.0, 1.0));
//;
//	return vec4(color, center, tiled_brick_pos.x+7.0*tiled_brick_pos.y);
//};

Color MMAlgos::brick(const Vector2 &uv, const Vector2 &bmin, const Vector2 &bmax, const float p_mortar, const float p_pround, const float p_bevel) {
	float mortar = p_mortar;
	float pround = p_pround;
	float bevel = p_bevel;

	float color;
	Vector2 size = bmax - bmin;
	float min_size = MIN(size.x, size.y);
	mortar *= min_size;
	bevel *= min_size;
	pround *= min_size;
	Vector2 center = 0.5 * (bmin + bmax);
	Vector2 d = Vector2();
	d.x = abs(uv.x - center.x) - 0.5 * (size.x) + (pround + mortar);
	d.y = abs(uv.y - center.y) - 0.5 * (size.y) + (pround + mortar);
	color = Vector2(MAX(d.x, 0), MAX(d.y, 0)).length() + MIN(MAX(d.x, d.y), 0.0) - pround;
	color = CLAMP(-color / bevel, 0.0, 1.0);
	//	var tiled_brick_pos : Vector2 = Vector2(bmin.x - 1.0 * Math::floor(bmin.x / 1.0), bmin.y - 1.0 * Math::floor(bmin.y / 1.0));
	float tiled_brick_pos_x = bmin.x - 1.0 * Math::floor(bmin.x / 1.0);
	float tiled_brick_pos_y = bmin.y - 1.0 * Math::floor(bmin.y / 1.0);
	//vec2 tiled_brick_pos = mod(bmin, vec2(1.0, 1.0));
	return Color(color, center.x, center.y, tiled_brick_pos_x + 7.0 * tiled_brick_pos_y);
}

//vec3 brick_random_color(vec2 bmin, vec2 bmax, float seed) {;
//	vec2 center = 0.5*(bmin + bmax);
//	return rand3(fract(center + vec2(seed)));
//};

Vector3 MMAlgos::brick_random_color(const Vector2 &bmin, const Vector2 &bmax, const float pseed) {
	Vector2 center = (bmin + bmax);
	center.x *= 0.5;
	center.y *= 0.5;
	return rand3(fractv2(center + Vector2(pseed, pseed)));
}

//vec3 brick_uv(vec2 uv, vec2 bmin, vec2 bmax, float seed) {;
//	vec2 center = 0.5*(bmin + bmax);
//	vec2 size = bmax - bmin;
//;
//	float max_size = MAX(size.x, size.y);
//;
//	return vec3(0.5+(uv-center)/max_size, rand(fract(center)+vec2(seed)));
//};

Vector3 MMAlgos::brick_uv(const Vector2 &uv, const Vector2 &bmin, const Vector2 &bmax, const float pseed) {
	Vector2 center = 0.5 * (bmin + bmax);
	Vector2 size = bmax - bmin;
	float max_size = MAX(size.x, size.y);
	float x = 0.5 + (uv.x - center.x) / max_size;
	float y = 0.5 + (uv.y - center.y) / max_size;
	return Vector3(x, y, rand(fractv2(center) + Vector2(pseed, pseed)));
}

//vec4 bricks_rb(vec2 uv, vec2 count, float repeat, float offset) {;
//	count *= repeat;float x_offset = offset*step(0.5, fract(uv.y*count.y*0.5));
//;
//	vec2 bmin = Math::floor(vec2(uv.x*count.x-x_offset, uv.y*count.y));
//;
//	bmin.x += x_offset;
//	bmin /= count;
//;
//	return vec4(bmin, bmin+vec2(1.0)/count);
//};

Color MMAlgos::bricks_rb(const Vector2 &uv, const Vector2 &p_count, const float repeat, const float offset) {
	Vector2 count = p_count;

	count *= repeat;
	float x_offset = offset * step(0.5, fractf(uv.y * count.y * 0.5));
	Vector2 bmin;
	bmin.x = Math::floor(uv.x * count.x - x_offset);
	bmin.y = Math::floor(uv.y * count.y);
	bmin.x += x_offset;
	bmin /= count;
	Vector2 bmc = bmin + Vector2(1.0, 1.0) / count;
	return Color(bmin.x, bmin.y, bmc.x, bmc.y);
}

//vec4 bricks_rb2(vec2 uv, vec2 count, float repeat, float offset) {;
//	count *= repeat;
//;
//	float x_offset = offset*step(0.5, fract(uv.y*count.y*0.5));
//	count.x = count.x*(1.0+step(0.5, fract(uv.y*count.y*0.5)));
//;
//	vec2 bmin = Math::floor(vec2(uv.x*count.x-x_offset, uv.y*count.y));
//;
//	bmin.x += x_offset;
//	bmin /= count;
//;
//	return vec4(bmin, bmin+vec2(1.0)/count);
//};

Color MMAlgos::bricks_rb2(const Vector2 &uv, const Vector2 &p_count, const float repeat, const float offset) {
	Vector2 count = p_count;

	count *= repeat;
	float x_offset = offset * step(0.5, fractf(uv.y * count.y * 0.5));
	count.x = count.x * (1.0 + step(0.5, fractf(uv.y * count.y * 0.5)));
	Vector2 bmin = Vector2();
	bmin.x = Math::floor(uv.x * count.x - x_offset);
	bmin.y = Math::floor(uv.y * count.y);
	bmin.x += x_offset;
	bmin /= count;
	Vector2 b = bmin + Vector2(1, 1) / count;
	return Color(bmin.x, bmin.y, b.x, b.y);
}

//vec4 bricks_hb(vec2 uv, vec2 count, float repeat, float offset) {;
//	float pc = count.x+count.y;
//	float c = pc*repeat;
//	vec2 corner = Math::floor(uv*c);
//	float cdiff = mod(corner.x-corner.y, pc);
//;
//	if (cdiff < count.x) {;
//		return vec4((corner-vec2(cdiff, 0.0))/c, (corner-vec2(cdiff, 0.0)+vec2(count.x, 1.0))/c);
//	} else {;
//		return vec4((corner-vec2(0.0, pc-cdiff-1.0))/c, (corner-vec2(0.0, pc-cdiff-1.0)+vec2(1.0, count.y))/c);
//	};
//};

Color MMAlgos::bricks_hb(const Vector2 &uv, const Vector2 &count, const float repeat, const float offset) {
	float pc = count.x + count.y;
	float c = pc * repeat;
	Vector2 corner = Vector2(floor(uv.x * c), Math::floor(uv.y * c));
	float cdiff = modf(corner.x - corner.y, pc);

	if ((cdiff < count.x)) {
		Color col = Color();
		col.r = (corner.x - cdiff) / c;
		col.g = corner.y / c;
		col.b = (corner.x - cdiff + count.x) / c;
		col.a = (corner.y + 1.0) / c;
		return col;
	} else {
		Color col = Color();
		col.r = corner.x / c;
		col.g = (corner.y - (pc - cdiff - 1.0)) / c;
		col.b = (corner.x + 1.0) / c;
		col.a = (corner.y - (pc - cdiff - 1.0) + count.y) / c;
		return col;
	}

	//vec4 bricks_bw(vec2 uv, vec2 count, float repeat, float offset) {;
	//	vec2 c = 2.0*count*repeat;
	//	float mc = MAX(c.x, c.y);
	//	vec2 corner1 = Math::floor(uv*c);
	//	vec2 corner2 = count*floor(repeat*2.0*uv);
	//	float cdiff = mod(dot(floor(repeat*2.0*uv), vec2(1.0)), 2.0);
	//	vec2 corner;
	//	vec2 size;
	//;
	//	if (cdiff == 0.0) {;
	//		orner = vec2(corner1.x, corner2.y);
	//		size = vec2(1.0, count.y);
	//	} else {;
	//		corner = vec2(corner2.x, corner1.y);
	//		size = vec2(count.x, 1.0);
	//	};
	//;
	//	return vec4(corner/c, (corner+size)/c);
	//};
}

Color MMAlgos::bricks_bw(const Vector2 &uv, const Vector2 &count, const float repeat, const float offset) {
	Vector2 c = 2.0 * count * repeat;
	//float mc = MAX(c.x, c.y);
	Vector2 corner1 = Vector2(floor(uv.x * c.x), Math::floor(uv.y * c.y));
	Vector2 corner2 = Vector2(count.x * Math::floor(repeat * 2.0 * uv.x), count.y * Math::floor(repeat * 2.0 * uv.y));
	Vector2 tmp = Vector2(floor(repeat * 2.0 * uv.x), Math::floor(repeat * 2.0 * uv.y));
	float cdiff = modf(tmp.dot(Vector2(1, 1)), 2.0);
	Vector2 corner;
	Vector2 size;

	if (cdiff == 0) {
		corner = Vector2(corner1.x, corner2.y);
		size = Vector2(1.0, count.y);
	} else {
		corner = Vector2(corner2.x, corner1.y);
		size = Vector2(count.x, 1.0);
	}

	return Color(corner.x / c.x, corner.y / c.y, (corner.x + size.x) / c.x, (corner.y + size.y) / c.y);
}

//vec4 bricks_sb(vec2 uv, vec2 count, float repeat, float offset) {;
//	vec2 c = (count+vec2(1.0))*repeat;
//	float mc = MAX(c.x, c.y);
//	vec2 corner1 = Math::floor(uv*c);
//	vec2 corner2 = (count+vec2(1.0))*floor(repeat*uv);
//	vec2 rcorner = corner1 - corner2;
//	vec2 corner;
//	vec2 size;
//;
//	if (rcorner.x == 0.0 && rcorner.y < count.y) {;
//		corner = corner2;
//		size = vec2(1.0, count.y);
//	} else if (rcorner.y == 0.0) {;
//		corner = corner2+vec2(1.0, 0.0);
//		size = vec2(count.x, 1.0);
//	} else if (rcorner.x == count.x) {;
//		corner = corner2+vec2(count.x, 1.0);
//		size = vec2(1.0, count.y);
//	} else if (rcorner.y == count.y) {;
//		corner = corner2+vec2(0.0, count.y);
//		size = vec2(count.x, 1.0);
//	} else {;
//		corner = corner2+vec2(1.0);
//		size = vec2(count.x-1.0, count.y-1.0);
//	};
//;
//	return vec4(corner/c, (corner+size)/c);
//};

Color MMAlgos::bricks_sb(const Vector2 &uv, const Vector2 &count, const float repeat, const float offset) {
	Vector2 c = (count + Vector2(1, 1)) * repeat;
	//	float mc = MAX(c.x, c.y);
	Vector2 corner1 = Vector2(floor(uv.x * c.x), Math::floor(uv.y * c.y));
	Vector2 corner2 = (count + Vector2(1, 1)) * Vector2(floor(repeat * uv.x), Math::floor(repeat * uv.y));
	Vector2 rcorner = corner1 - corner2;
	Vector2 corner;
	Vector2 size;

	if ((rcorner.x == 0.0 && rcorner.y < count.y)) {
		corner = corner2;
		size = Vector2(1.0, count.y);
	} else if (rcorner.y == 0.0) {
		corner = corner2 + Vector2(1.0, 0.0);
		size = Vector2(count.x, 1.0);
	} else if (rcorner.x == count.x) {
		corner = corner2 + Vector2(count.x, 1.0);
		size = Vector2(1.0, count.y);
	} else if (rcorner.y == count.y) {
		corner = corner2 + Vector2(0.0, count.y);
		size = Vector2(count.x, 1.0);
	} else {
		corner = corner2 + Vector2(1, 1);
		size = Vector2(count.x - 1.0, count.y - 1.0);
	}

	return Color(corner.x / c.x, corner.y / c.y, (corner.x + size.x) / c.x, (corner.y + size.y) / c.y);
}

//vec4 brick2(vec2 uv, vec2 bmin, vec2 bmax, float mortar, float round, float bevel) {;
//	float color;
//	vec2 size = bmax - bmin;
//	vec2 center = 0.5*(bmin+bmax);
//	vec2 d = abs(uv-center)-0.5*(size)+vec2(round+mortar);
//;
//	color = length(MAX(d,vec2(0))) + MIN(MAX(d.x,d.y),0.0)-round;
//	color = CLAMP(-color/bevel, 0.0, 1.0);
//;
//	vec2 tiled_brick_pos = mod(bmin, vec2(1.0, 1.0));
//;
//	return vec4(color, center, tiled_brick_pos.x+7.0*tiled_brick_pos.y);
//};

Color MMAlgos::brick2(const Vector2 &uv, const Vector2 &bmin, const Vector2 &bmax, const float mortar, const float pround, const float bevel) {
	return Color();
}

//vec4 bricks_uneven(vec2 uv, int iterations, float min_size, float randomness, float seed) {;
//	vec2 a = vec2(0.0);
//	vec2 b = vec2(1.0);
//	for (int i = 0; i < iterations; ++i) {;
//		vec2 size = b-a;
//		if (MAX(size.x, size.y) < min_size) {;
//			break;
//		};
//;
//		float x = rand(rand2(vec2(rand(a+b), seed)))*randomness+(1.0-randomness)*0.5;
//;
//		if (size.x > size.y) {;
//			x *= size.x;
//;
//			if (uv.x > a.x+x) {;
//				a.x += x;
//			} else {;
//				b.x = a.x + x;
//			};
//		} else {;
//			x *= size.y;
//;
//			if (uv.y > a.y+x) {;
//				a.y += x;
//			} else {;
//				b.y = a.y + x;
//			};
//		};
//	};
//;
//	return vec4(a, b);
//};

Color MMAlgos::bricks_uneven(const Vector2 &uv, const int iterations, const float min_size, const float randomness, const float pseed) {
	return Color();
}

//vec2 truchet_generic_uv(vec2 uv, vec2 seed) {;
//	vec2 i = Math::floor(uv);
//	vec2 f = fract(uv);
//	vec2 invert = step(rand2(seed+i), vec2(0.5));
//;
//	return f*(vec2(1.0)-invert)+(vec2(1.0)-f)*invert;
//};

Vector2 MMAlgos::truchet_generic_uv(const Vector2 &uv, const float pseed) {
	return Vector2();
}

//float pavement(vec2 uv, float bevel, float mortar) {\n\t;
//	uv = abs(uv-vec2(0.5));\n\t;
//;
//	return CLAMP((0.5*(1.0-mortar)-MAX(uv.x, uv.y))/MAX(0.0001, bevel), 0.0, 1.0);
//};
//vec4 arc_pavement(vec2 uv, float acount, float lcount, out vec2 seed) {\n\t;
//	float PI = 3.141592654;\n\t;
//	float radius = (0.5/sqrt(2.0));\n;
//	float uvx = uv.x;\n;
//	uv.x = 0.5*fract(uv.x+0.5)+0.25;\n;
//	float center = (uv.x-0.5)/radius;\n;
//	center *= center;\n;
//	center = Math::floor(acount*(uv.y-radius*sqrt(1.0-center))+0.5)/acount;\n;
//;
//	vec2 v = uv-vec2(0.5, center);\n;
//	float cornerangle = 0.85/acount+0.25*PI;\n;
//	float acountangle = (PI-2.0*cornerangle)/(lcount+floor(mod(center*acount, 2.0)));\n;
//	float angle = mod(atan(v.y, v.x), 2.0*PI);\n\t;
//;
//	float base_angle;\n\t;
//	float local_uvy = 0.5+acount*(length(v)-radius)*(1.54-0.71*cos(1.44*(angle-PI*0.5)));\n\t;
//	vec2 local_uv;\n;
//;
//	if (angle < cornerangle) {\n;
//		base_angle = 0.25*PI;\n\t\t;
//		local_uv = vec2((angle-0.25*PI)/cornerangle*0.38*acount+0.5, 1.0-local_uvy);\n\t\t;
//		seed = vec2(fract(center), 0.0);\n;
//	} else if (angle > PI-cornerangle) {\n;
//		base_angle = 0.75*PI;\n\t\t;
//		local_uv = vec2(local_uvy, 0.5-(0.75*PI-angle)/cornerangle*0.38*acount);\n\t\t;
//		seed = vec2(fract(center), 0.0);\n;
//	} else {\n;
//		base_angle = cornerangle+(floor((angle-cornerangle)/acountangle)+0.5)*acountangle;\n\t\t;
//		local_uv = vec2((angle-base_angle)/acountangle+0.5, 1.0-local_uvy);\n\t\t;
//		seed = vec2(fract(center), base_angle);\n;
//	}\n;
//;
//	vec2 brick_center = vec2(0.5, center)+radius*vec2(cos(base_angle), sin(base_angle));\n;
//;
//	return vec4(brick_center.x+uvx-uv.x, brick_center.y, local_uv);\n;
//};
// ====================== SDF2D.GD ========================;
//----------------------;
//sdarc.mmg;
//----------------------;
//sdboolean.mmg;
//----------------------;
//sdbox.mmg;
//----------------------;
//sdcircle.mmg;
//----------------------;
//sdcirclerepeat.mmg;
//Repeats its input shape around a circle;
//Output:;
//Out, sdf2d (float) (property);
//$in(circle_repeat_transform_2d($uv-vec2(0.5), $c)+vec2(0.5));
//Input:;
//in, float (sdf2d), default : 0;
//count, int, min: 1, max: 32, default: 6;
//----------------------;
//sdelongation.mmg;
//----------------------;
//sdline.mmg;
//A line or a capsule shape described as a signed distance function;
//vec2 $(name_uv)_sdl = sdLine($uv, vec2($ax+0.5, $ay+0.5), vec2($bx+0.5, $by+0.5));
//Outputs;
//output, sdf2d (float), (output property);
//$(name_uv)_sdl.x-$r*$profile($(name_uv)_sdl.y);
//Inputs;
//A, Vector2, min: -1, max: 1, step: 0.01, default: (-0.3, -0.3);
//B, Vector2, min: -1, max: 1, step: 0.01, default: (0.3, 0.3);
//width, float, min: 0, max: 1, step: 0.01, default: 0.1;
//points (curve), default: 0, 0, 0, 1,  0, 0, 1, 1;
//----------------------;
//sdmorph.mmg;
//----------------------;
//sdngon.mmg (inc sdrotate.mmg, sdcirclerepeat.mmg);
//----------------------;
//sdpolygon.mmg;
//A polygon as a signed distance function;
//Output:;
//Out, sdf2d (float) (property);
//sdPolygon_$(name)($uv);
//Input:;
//polygon points, default: 0.2, 0.2,  0.4, 0.7,  0.7, 0.4;
//----------------------;
//sdrepeat.mmg;
//Repeats its input shape on a grid.This node does not support overlapping between instances.;
//Output:;
//Out, sdf2d (float) (property);
//$in(repeat_2d($uv, vec2(1.0/$rx, 1.0/$ry), float($seed), $r));
//Input:;
//in, float (sdf2d), default : 0;
//x, int, min: 1, max: 32, default: 4;
//y, int, min: 1, max: 32, default: 4;
//random_rotation, min: 0, max: 1, step:0.01, default: 0.5;
//----------------------;
//sdrhombus.mmg;
//----------------------;
//sdrotate.mmg;
//----------------------;
//sdroundedshape.mmg;
//Dilates an input shape into a rounded shape;
//Output:;
//Out, sdf2d (float) (property);
//$in($uv)-$r;
//Input:;
//in, float (sdf2d), default : 0;
//radius, min: 0, max: 1, step:0.01, default: 0;
//----------------------;
//sdscale.mmg;
//----------------------;
//sdshow.mmg;
//Creates a greyscale image from a shape described as a 2D Signed Distance Function;
//Output;
//Output float (color) - Shows the shape as a greyscale image;
//clamp($base-$in($uv)/MAX($bevel, 0.00001), 0.0, 1.0);
//Input:;
//Input (sdf - shape), default: 0 - sdf2d - universal input;
//bevel, float, min 0, max 1, step 0.01, default 0;
//base, float, min 0, max 1, step 0.01, default 0;
//----------------------;
//sdsmoothboolean.mmg;
//----------------------;
//sdtranslate.mmg;
//----------------------;
//curve.mmg;
//----------------------;
//sdannularshape.mmg;
//Creates an annular shape from a shape described as a signed distance function;
//Output;
//Output float (color) - Shows the shape as a greyscale image;
//sdRipples($in($uv), $r, int($ripples));
//Input:;
//Input (sdf - shape), default: 0 - sdf2d - universal input;
//width, float, min 0, max 1, step 0.01, default 0;
//rippples, int, min 1, max 16, default 1;
//----------------------;
//sd_mask_to_sdf.mmg;
//----------------------;

Color MMAlgos::sdf_show(const float val, const float bevel) {
	float f = CLAMP(-val / MAX(bevel, 0.00001), 0.0, 1.0);
	return Color(f, f, f, 1);
}

float MMAlgos::sdf_circle(const Vector2 &uv, const Vector2 &p_c, const float r) {
	Vector2 c = p_c;

	c.x += 0.5;
	c.y += 0.5;
	return (uv - c).length() - r;
}

float MMAlgos::sdf_box(const Vector2 &uv, const Vector2 &p_c, const Vector2 &wh) {
	Vector2 c = p_c;

	c.x += 0.5;
	c.y += 0.5;
	Vector2 d = absv2(uv - c) - wh;
	return maxv2(d, Vector2(0, 0)).length() + MIN(MAX(d.x, d.y), 0.0);
}

//vec2 $(name_uv)_sdl = sdLine($uv, vec2($ax+0.5, $ay+0.5), vec2($bx+0.5, $by+0.5));

Vector2 MMAlgos::sdf_line(const Vector2 &uv, const Vector2 &p_a, const Vector2 &p_b, const float r) {
	Vector2 a = p_a;
	Vector2 b = p_b;

	a.x += 0.5;
	a.y += 0.5;
	b.x += 0.5;
	b.y += 0.5;
	return sdLine(uv, a, b);
}

float MMAlgos::sdf_rhombus(const Vector2 &uv, const Vector2 &p_c, const Vector2 &wh) {
	Vector2 c = p_c;

	c.x += 0.5;
	c.y += 0.5;
	return sdRhombus(uv - c, wh);
}

float MMAlgos::sdf_arc(const Vector2 &uv, const Vector2 &a, const Vector2 &r) {
	return sdArc(uv - Vector2(0.5, 0.5), modf(a.x, 360.0) * 0.01745329251, modf(a.y, 360.0) * 0.01745329251, r.x, r.y);
}

//float sdr_ndot(vec2 a, vec2 b) {;
//	return a.x*b.x - a.y*b.y;
//};

float MMAlgos::sdr_ndot(const Vector2 &a, const Vector2 &b) {
	return a.x * b.x - a.y * b.y;
}

//float sdRhombus(in vec2 p, in vec2 b) {;
//	vec2 q = abs(p);
//	float h = CLAMP((-2.0*sdr_ndot(q,b)+sdr_ndot(b,b))/dot(b,b),-1.0,1.0);
//	float d = length( q - 0.5*b*vec2(1.0-h,1.0+h) );
//	return d * SGN( q.x*b.y + q.y*b.x - b.x*b.y );
//};

float MMAlgos::sdRhombus(const Vector2 &p, const Vector2 &b) {
	Vector2 q = absv2(p);
	float h = CLAMP((-2.0 * sdr_ndot(q, b) + sdr_ndot(b, b)) / b.dot(b), -1.0, 1.0);
	float d = (q - 0.5 * b * Vector2(1.0 - h, 1.0 + h)).length();
	return d * SGN(q.x * b.y + q.y * b.x - b.x * b.y);
}

//float sdArc(vec2 p, float a1, float a2, float ra, float rb) {;
//	float amid = 0.5*(a1+a2)+1.6+3.14*step(a1, a2);
//	float alength = 0.5*(a1-a2)-1.6+3.14*step(a1, a2);
//;
//	vec2 sca = vec2(cos(amid), sin(amid));
//	vec2 scb = vec2(cos(alength), sin(alength));
//	p *= mat2(vec2(sca.x,sca.y),vec2(-sca.y,sca.x));
//	p.x = abs(p.x);
//;
//	float k = (scb.y*p.x>scb.x*p.y) ? dot(p.xy,scb) : length(p.xy);
//	return sqrt( dot(p,p) + ra*ra - 2.0*ra*k ) - rb;
//};

float MMAlgos::sdArc(const Vector2 &p_p, const float a1, const float a2, const float ra, const float rb) {
	Vector2 p = p_p;

	float amid = 0.5 * (a1 + a2) + 1.6 + 3.14 * step(a1, a2);
	float alength = 0.5 * (a1 - a2) - 1.6 + 3.14 * step(a1, a2);
	Vector2 sca = Vector2(cos(amid), sin(amid));
	Vector2 scb = Vector2(cos(alength), sin(alength));
	//p *= Matrix(Vector2(sca.x , sca.y), Vector2(-sca.y, sca.x));
	Vector2 pt = p;
	p.x = pt.x * sca.x + pt.y * sca.y;
	p.y = pt.x * -sca.y + pt.y * sca.x;
	p.x = abs(p.x);
	float k;

	if ((scb.y * p.x > scb.x * p.y)) {
		k = p.dot(scb);
	} else {
		k = p.length();
	}

	return sqrt(p.dot(p) + ra * ra - 2.0 * ra * k) - rb;
}

float MMAlgos::sdf_boolean_union(const float a, const float b) {
	return MIN(a, b);
}

float MMAlgos::sdf_boolean_substraction(const float a, const float b) {
	return MAX(-a, b);
}

float MMAlgos::sdf_boolean_intersection(const float a, const float b) {
	return MAX(a, b);
}

float MMAlgos::sdf_smooth_boolean_union(const float d1, const float d2, const float k) {
	float h = CLAMP(0.5 + 0.5 * (d2 - d1) / k, 0.0, 1.0);
	return Math::lerp(d2, d1, h) - k * h * (1.0 - h);
}

float MMAlgos::sdf_smooth_boolean_substraction(const float d1, const float d2, const float k) {
	float h = CLAMP(0.5 - 0.5 * (d2 + d1) / k, 0.0, 1.0);
	return Math::lerp(d2, -d1, h) + k * h * (1.0 - h);
}

float MMAlgos::sdf_smooth_boolean_intersection(const float d1, const float d2, const float k) {
	float h = CLAMP(0.5 - 0.5 * (d2 - d1) / k, 0.0, 1.0);
	return Math::lerp(d2, d1, h) + k * h * (1.0 - h);
}

float MMAlgos::sdf_rounded_shape(const float a, const float r) {
	return a - r;
}

float MMAlgos::sdf_annular_shape(const float a, const float r) {
	return abs(a) - r;
}

float MMAlgos::sdf_morph(const float a, const float b, const float amount) {
	return Math::lerp(a, b, amount);
}

//vec2 sdLine(vec2 p, vec2 a, vec2 b) {;
//	vec2 pa = p-a, ba = b-a;
//	float h = CLAMP(dot(pa,ba)/dot(ba,ba), 0.0, 1.0);
//	return vec2(length(pa-ba*h), h);
//};

Vector2 MMAlgos::sdLine(const Vector2 &p, const Vector2 &a, const Vector2 &b) {
	Vector2 pa = p - a;
	Vector2 ba = b - a;
	float h = CLAMP(pa.dot(ba) / ba.dot(ba), 0.0, 1.0);
	return Vector2((pa - (ba * h)).length(), h);
}

//Needs thought;
//func sdf_translate(a : float, x : float, y : float) -> float:;
//	return Math::lerp(a, b, amount);
//vec2 sdf2d_rotate(vec2 uv, float a) {;
//	vec2 rv;
//	float c = cos(a);
//	float s = sin(a);
//	uv -= vec2(0.5);
//	rv.x = uv.x*c+uv.y*s;
//	rv.y = -uv.x*s+uv.y*c;
//	return rv+vec2(0.5);
//};

Vector2 MMAlgos::sdf2d_rotate(const Vector2 &uuv, const float a) {
	Vector2 uv = uuv;
	Vector2 rv;

	float c = cos(a);
	float s = sin(a);
	uv -= Vector2(0.5, 0.5);
	rv.x = uv.x * c + uv.y * s;
	rv.y = -uv.x * s + uv.y * c;
	return rv + Vector2(0.5, 0.5);
}

//float cross2( in vec2 a, in vec2 b ) {;
//	return a.x*b.y - a.y*b.x;
//};
//// signed distance to a quadratic bezier\n;
//vec2 sdBezier( in vec2 pos, in vec2 A, in vec2 B, in vec2 C ) {    \n;
//	vec2 a = B - A;\n;
//	vec2 b = A - 2.0*B + C;\n;
//	vec2 c = a * 2.0;\n;
//	vec2 d = A - pos;\n\n;
//	float kk = 1.0/dot(b,b);\n;
//	float kx = kk * dot(a,b);\n;
//	float ky = kk * (2.0*dot(a,a)+dot(d,b))/3.0;\n;
//	float kz = kk * dot(d,a);      \n\n;
//	float res = 0.0;\n;
//	float sgn = 0.0;\n\n;
//	float p = ky - kx*kx;\n;
//	float p3 = p*p*p;\n;
//	float q = kx*(2.0*kx*kx - 3.0*ky) + kz;\n;
//	float h = q*q + 4.0*p3;\n\t;
//	float rvx;\n\n;
//;
//	if( h>=0.0 ) {;
//		// 1 root\n;
//		h = sqrt(h);\n;
//		vec2 x = (vec2(h,-h)-q)/2.0;\n;
//		vec2 uv = SGN(x)*pow(abs(x), vec2(1.0/3.0));
//		rvx = uv.x+uv.y-kx;\n;
//		float t = CLAMP(rvx, 0.0, 1.0);\n;
//		vec2 q2 = d+(c+b*t)*t;\n;
//		res = dot(q2, q2);\n    \t;
//		sgn = cross2(c+2.0*b*t, q2);\n;
//	} else {;
//		// 3 roots\n;
//		float z = sqrt(-p);\n;
//		float v = acos(q/(p*z*2.0))/3.0;\n;
//		float m = cos(v);\n;
//		float n = sin(v)*1.732050808;\n;
//		vec3  t = CLAMP(vec3(m+m,-n-m,n-m)*z-kx, 0.0, 1.0);\n;
//		vec2  qx=d+(c+b*t.x)*t.x;
//		float dx=dot(qx, qx), sx = cross2(c+2.0*b*t.x,qx);\n;
//		vec2  qy=d+(c+b*t.y)*t.y;
//		float dy=dot(qy, qy), sy = cross2(c+2.0*b*t.y,qy);\n;
//;
//		if( dx<dy ) {;
//			res=dx; sgn=sx; rvx = t.x;
//		} else {;
//			res=dy; sgn=sy; rvx = t.y;
//		}\n;
//	}\n    \n;
//;
//	return vec2(rvx, sqrt(res)*sign(sgn));\n;
//};
// signed distance to a quadratic bezier;

Vector2 MMAlgos::sdBezier(const Vector2 &pos, const Vector2 &A, const Vector2 &B, const Vector2 &C) {
	Vector2 a = B - A;
	Vector2 b = A - 2.0 * B + C;
	Vector2 c = a * 2.0;
	Vector2 d = A - pos;
	float kk = 1.0 / b.dot(b);
	float kx = kk * a.dot(b);
	float ky = kk * (2.0 * a.dot(a) + d.dot(b)) / 3.0;
	float kz = kk * d.dot(a);
	float res = 0.0;
	float sgn = 0.0;
	float p = ky - kx * kx;
	float p3 = p * p * p;
	float q = kx * (2.0 * kx * kx - 3.0 * ky) + kz;
	float h = q * q + 4.0 * p3;
	float rvx = 0;

	if (h >= 0.0) {
		// // 1 root;
		h = sqrt(h);
		Vector2 x = (Vector2(h, -h) - Vector2(q, q)) / 2.0;
		Vector2 uv = signv2(x) * powv2(absv2(x), Vector2(1.0 / 3.0, 1.0 / 3.0));
		rvx = uv.x + uv.y - kx;
		float t = CLAMP(rvx, 0.0, 1.0);
		Vector2 q2 = d + (c + b * t) * t;
		res = q2.dot(q2);
		sgn = (c + Vector2(2, 2) * b * t).cross(q2);
	} else {
		//  // 3 roots;
		float z = sqrt(-p);
		float v = acos(q / (p * z * 2.0)) / 3.0;
		float m = cos(v);
		float n = sin(v) * 1.732050808;
		Vector3 t = clampv3(Vector3(m + m, -n - m, n - m) * z - Vector3(kx, kx, kx), Vector3(), Vector3(1, 1, 1));
		Vector2 qx = d + (c + b * t.x) * t.x;
		float dx = qx.dot(qx);
		float sx = (c + Vector2(2, 2) * b * t.x).cross(qx);
		Vector2 qy = d + (c + b * t.y) * t.y;
		float dy = qy.dot(qy);
		float sy = (c + Vector2(2, 2) * b * t.y).cross(qy);

		if (dx < dy) {
			res = dx;
			sgn = sx;
			rvx = t.x;
		} else {
			res = dy;
			sgn = sy;
			rvx = t.y;
		}
	}

	return Vector2(rvx, sqrt(res) * SGN(sgn));
}

//vec2 circle_repeat_transform_2d(vec2 p, float count) {;
//	float r = 6.28/count;
//	float pa = atan(p.x, p.y);
//	float a = mod(pa+0.5*r, r)-0.5*r;
//;
//	vec2 rv;
//;
//	float c = cos(a-pa);
//	float s = sin(a-pa);
//;
//	rv.x = p.x*c+p.y*s;
//	rv.y = -p.x*s+p.y*c;
//;
//	return rv;
//};

Vector2 MMAlgos::circle_repeat_transform_2d(const Vector2 &p, const float count) {
	float r = 6.28 / count;
	float pa = atan2(p.x, p.y);
	float a = modf(pa + 0.5 * r, r) - 0.5 * r;
	Vector2 rv = Vector2();
	float c = cos(a - pa);
	float s = sin(a - pa);
	rv.x = p.x * c + p.y * s;
	rv.y = -p.x * s + p.y * c;
	return rv;
}

//float sdNgon(vec2 p, float r, float n) {;
//	float PI = 3.1415926535;
//	p = circle_repeat_transform_2d(p, n);
//	vec2 d = abs(p)-vec2(r*tan(3.14159265359/n), r);
//	return p.y < r ? p.y-r : length(MAX(d,vec2(0)))+min(MAX(d.x,d.y),0.0);
//};

Vector2 MMAlgos::sdNgon(const Vector2 &pos, const float r, const float n) {
	return Vector2();
}

//vec2 repeat_2d(vec2 p, vec2 r, float seed, float randomness) {;
//	p -= vec2(0.5);
//	float a = (rand(floor(mod((p.xy+0.5*r.xy)/r.xy, 1.0/r.xy)+vec2(seed)))-0.5)*6.28*randomness;
//	p = mod(p+0.5*r,r)-0.5*r;
//	vec2 rv;
//	float c = cos(a);
//	float s = sin(a);
//	rv.x = p.x*c+p.y*s;
//	rv.y = -p.x*s+p.y*c;
//	return rv+vec2(0.5);
//};

Vector2 MMAlgos::repeat_2d(const Vector2 &p_p, const Vector2 &r, const float pseed, const float randomness) {
	Vector2 p = p_p;

	p -= Vector2(0.5, 0.5);
	Vector2 v = Vector2(p.x, p.y) + Vector2(0.5, 0.5) + Vector2(r.x, r.y);
	float a = ((rand2(floorv2(modv2(v / Vector2(r.x, r.y), Vector2(1.0, 1.0) / Vector2(r.x, r.y)) + Vector2(pseed, pseed))) - Vector2(0.5, 0.5)) * 6.28 * randomness).x;
	p = modv2(p + Vector2(0.5, 0.5) * r, r) - Vector2(0.5, 0.5) * r;
	Vector2 rv = Vector2();
	float c = cos(a);
	float s = sin(a);
	rv.x = p.x * c + p.y * s;
	rv.y = -p.x * s + p.y * c;
	return rv + Vector2(0.5, 0.5);
}

//float sdSmoothUnion( float d1, float d2, float k ) {;
//	float h = CLAMP( 0.5 + 0.5*(d2-d1)/k, 0.0, 1.0 );
//	return mix( d2, d1, h ) - k*h*(1.0-h);
//};

float MMAlgos::sdSmoothUnion(const float d1, const float d2, const float k) {
	return 0.0;
}

//float sdSmoothSubtraction( float d1, float d2, float k ) {;
//	float h = CLAMP( 0.5 - 0.5*(d2+d1)/k, 0.0, 1.0 );
//	return mix( d2, -d1, h ) + k*h*(1.0-h);
//};

float MMAlgos::sdSmoothSubtraction(const float d1, const float d2, const float k) {
	return 0.0;
}

//float sdSmoothIntersection( float d1, float d2, float k ) {;
//	float h = CLAMP( 0.5 - 0.5*(d2-d1)/k, 0.0, 1.0 );
//	return mix( d2, d1, h ) + k*h*(1.0-h);
//};

float MMAlgos::sdSmoothIntersection(const float d1, const float d2, const float k) {
	return 0.0;
}

//float sdRipples(float d, float w, int r) {\n;
//	for (int i = 0; i < r; ++i) {;
//		d = abs(d)-w;
//	};
//;
//	return d;
//};

float MMAlgos::sdRipples(const float p_d, const float w, const int r) {
	float d = p_d;

	for (int i = 0; i < r; ++i) { //i in range(r)
		d = abs(d) - w;
	}

	return d;
}

//$polygon = { p1(vec2), p2(vec2), p3(vec2) ... };
//float sdPolygon_$(name)(vec2 p) {;
//	vec2 v[] = $polygon;
//	int l = v.length();
//	float d = dot(p-v[0],p-v[0]);
//	float s = 1.0;
//	int j = l-1;
//;
//	for(int i=0; i<l; i++) {;
//		vec2 e = v[j] - v[i];
//		vec2 w = p - v[i];
//		vec2 b = w - e*clamp( dot(w,e)/dot(e,e), 0.0, 1.0 );
//		d = MIN( d, dot(b,b) );
//		bvec3 c = bvec3(p.y>=v[i].y,p.y<v[j].y,e.x*w.y>e.y*w.x);
//;
//		if(all(c) || all(not(c))) {;
//			s *= -1.0;
//		};
//;
//		j = i;
//	};
//;
//	return s*sqrt(d);
//};

float MMAlgos::sdPolygon(const Vector2 &p, const PoolVector2Array &v) {
	int l = v.size();
	Vector2 pmv0 = p - v[0];
	float d = pmv0.dot(pmv0);
	float s = 1.0;
	int j = l - 1;
	//for(int i=0; i<l; i++);

	for (int i = 0; i < l; ++i) { //i in range(l)
		Vector2 e = v[j] - v[i];
		Vector2 w = p - v[i];
		Vector2 b = w - e * CLAMP(w.dot(e) / e.dot(e), 0.0, 1.0);
		d = MIN(d, b.dot(b));
		bool b1 = p.y >= v[i].y;
		bool b2 = p.y < v[j].y;
		bool b3 = e.x * w.y > e.y * w.x;

		if ((b1 && b2 && b3) || (!b1 && !b2 && !b3)) {
			s *= -1.0;
		}

		j = i;
	}

	return s * sqrt(d);
}

// =================== SDF3D.GD ===========================;
//----------------------;
//sdf3d_box.mmg;
//Generates a rounded box as a signed distance function;
//Outputs:;
//Common;
//vec3 $(name_uv)_q = abs($uv) - vec3($sx, $sy, $sz);
//Output - (sdf3d) - Shows the rounded box;
//length(MAX($(name_uv)_q,0.0))+min(MAX($(name_uv)_q.x,MAX($(name_uv)_q.y,$(name_uv)_q.z)),0.0)-$r;
//Inputs:;
//size, vector3, min: 0, max: 1, default:0.5, step:0.01;
//size, float, min: 0, max: 1, default:0.5, step:0.01;
//----------------------;
//sdf3d_sphere.mmg;
//Generates a sphere as a signed distance function;
//Outputs:;
//Output - (sdf3d) - Shows the sphere;
//length($uv)-$r;
//Inputs:;
//radius, vector3, min: 0, max: 1, default:0.5, step:0.01;
//----------------------;
//sdf3d_capsule.mmg;
//Generates a capsule as a signed distance function;
//Outputs:;
//Common;
//vec3 $(name_uv)_p = $uv;
//$(name_uv)_p.$axis -= CLAMP($(name_uv)_p.$axis, -$l, $l);
//Output - (sdf3d) - Shows the capsule;
//length($(name_uv)_p)-$r*$profile(clamp(0.5+0.5*($uv).$axis/$l, 0.0, 1.0));
//Inputs:;
//axis, enum, default: 1, values: x, y, z;
//length, float, min: 0, max: 1, default:0.25, step:0.01;
//radius, float, min: 0, max: 1, default:0.2, step:0.01;
//profile, curve, default: (ls, rs, x, z) 0, 0, 0, 1,  0, 0, 1, 1;
//----------------------;
//sdf3d_cone.mmg;
//Outputs:;
//+X: $axis = length($uv.yz),-$uv.x;
//-X: $axis = length($uv.yz),$uv.x;
//+Y: $axis = length($uv.xz),$uv.y;
//-Y: $axis = length($uv.xz),-$uv.y;
//+Z: $axis = length($uv.xy),-$uv.z;
//-Z: $axis = length($uv.xy),$uv.z;
//Output - (sdf3d);
//dot(vec2(cos($a*0.01745329251),sin($a*0.01745329251)),vec2($axis));
//Inputs:;
//axis, enum, default:5, values: +X, -X, +Y, -Y, +Z, -Z;
//angle, float, min: 0, max: 90, default:30, step:1;
//----------------------;
//sdf3d_repeat.mmg;
//Outputs:;
//Output (sdf3d);
//Output - (sdf3dc) - The shape generated by the repeat operation;
//$in(repeat($uv, vec3(1.0/$rx, 1.0/$ry, 0.0), float($seed), $r));
//Inputs:;
//in, vec2, default:vec2(100, 0.0), (sdf3d input);
//X, int, min: 1, max: 32, default:4, step:1;
//Y, int, min: 1, max: 32, default:4, step:1;
//R, float, min: 0, max: 1, default:0.5, step:0.01;
//----------------------;
//sdf3d_rotate.mmg;
//Outputs:;
//Output - (sdf3dc) - The rotated object;
//$in(rotate3d($uv, -vec3($ax, $ay, $az)*0.01745329251));
//Inputs:;
//in, vec2, default:vec2(100, 0.0), (sdf3d input);
//rotation, vector3, min: -180, max: 180, default:0, step:1;
//----------------------;
//sdf3d_cylinder.mmg;
//Outputs:;
//Output - (sdf3dc) - Shows the cylinder;
//min(MAX($(name_uv)_d.x,$(name_uv)_d.y),0.0) + length(MAX($(name_uv)_d,0.0));
//Inputs:;
//axis, enum, default: 1, values: X, Y, Z;
//length, float, min: 0, max: 1, default:0.5, step:0.01;
//radius, float, min: 0, max: 1, default:0.2, step:0.01;
//----------------------;
//sdf3d_plane.mmg;
//Generates a plane that can be used to cut other shapes;
//Outputs:;
//X: $axis = x;
//Y: $axis = y;
//Z: $axis = z;
//Output - (sdf3dc) - Shows the plane;
//$uv.$axis;
//Inputs:;
//axis, enum, default: 1, values: X, Y, Z;
//----------------------;
//sdf3d_torus.mmg;
//Generates a torus as a signed distance function;
//Outputs:;
//X: $axis = length($uv.yz)-$R,$uv.x;
//Y: $axis = length($uv.zx)-$R,$uv.y;
//Z: $axis = length($uv.xy)-$R,$uv.z;
//vec2 $(name_uv)_q = vec2($axis);
//Output - (sdf3dc) - Shows the torus;
//length($(name_uv)_q)-$r;
//Inputs:;
//axis, enum, default: 1, values: X, Y, Z;
//R, float, min: 0, max: 1, default:0.5, step:0.01;
//r, float, min: 0, max: 1, default:0.1, step:0.01;
//----------------------;
//sdf3d_boolean.mmg;
//Outputs:;
//Union: $op = sdf3dc_union;
//Subtraction $op = sdf3dc_sub;
//Intersection $op = sdf3dc_inter;
//Output - (sdf3dc) - The shape generated by the boolean operation;
//$op($in1($uv), $in2($uv));
//Inputs:;
//axis, enum, default: 2, values: Union, Subtraction, Intersection;
//in1, vec2, default:vec2(100, 0.0), (sdf3d input);
//in2, vec2, default:vec2(100, 0.0), (sdf3d input);
//----------------------;
//sdf3d_circle_repeat.mmg;
//Outputs:;
//Output (sdf3dc) - The shape generated by the boolean operation;
//$in(circle_repeat_transform($uv, $c));
//Inputs:;
//count, float, min: 1, max: 32, default:5, step:1;
//----------------------;
//sdf3d_angle.mmg (includes sdf3d_rotate.mmg);
//Outputs:;
//Shows the angleThe shape generated by the boolean operation;
//$(name_uv)_d;
//X: $axis = xyz;
//Y: $axis = yzx;
//Z: $axis = zxy;
//vec3 $(name_uv)_uv = $uv.$axis;
//float $(name_uv)_rotated = rotate3d($(name_uv)_uv, vec3(($angle-180.0)*0.01745329251, 0.0, 0.0)).y;
//float $(name_uv)_d1 = MAX($(name_uv)_uv.y, $(name_uv)_rotated);
//float $(name_uv)_d2 = MIN($(name_uv)_uv.y, $(name_uv)_rotated);
//float $(name_uv)_d = (mod($angle, 360.0) < 180.0) ? $(name_uv)_d1 : $(name_uv)_d2;
//Inputs:;
//axis, enum, default: 0, values: X, Y, Z;
//angle, float, min: 0, max: 360, default:180, step:0.1;
//----------------------;
//sdf3d_color.mmg;
//Outputs:;
//Output - sdf3dc - The colored 3D object;
//vec2($in($uv), $c);
//Inputs:;
//color_index, float, min: 0, max: 1, default:0, step:0.01;
//in, vec2, default:vec2(100, 0.0), (sdf3d input);
//----------------------;
//sdf3d_translate.mmg;
//Outputs:;
//Output - sdf3dc;
//$in($uv-vec3($x, $y, $z));
//Inputs:;
//translation, vector3, min: -1, max: 1, default:0, step:0.01;
//in, vec2, default:vec2(100, 0.0), (sdf3dc input);
//----------------------;
//sdf3d_scale.mmg;
//Outputs:;
//vec2 $(name_uv)_in = $in(($uv)/$s);
//Output - sdf3dc;
//vec2($(name_uv)_in.x*$s, $(name_uv)_in.y);
//Inputs:;
//scale_factor, float, min: 0, max: 5, default:1, step:0.01;
//in, vec2, default:vec2(100, 0.0), (sdf3dc input);
//----------------------;
//sdf3d_rounded.mmg;
//Outputs:;
//vec2 $(name_uv)_v = $in($uv);
//Output - sdf3dc;
//vec2($(name_uv)_v.x-$r, $(name_uv)_v.y);
//Inputs:;
//radius, float, min: 0, max: 1, default:0, step:0.01;
//in, vec2, default:vec2(100, 0.0), (sdf3dc input);
//----------------------;
//sdf3d_revolution.mmg;
//Outputs:;
//vec2 $(name_uv)_q = vec2(length($uv.xy)-$d+0.5, $uv.z+0.5);
//Output - sdf3dc;
//$in($(name_uv)_q);
//Inputs:;
//d, float, min: 0, max: 1, default:0.25, step:0.01;
//input, float, default:10.0, (sdf2d input);
//----------------------;
//sdf3d_smoothboolean.mmg;
//Performs a smooth boolean operation (union, intersection or difference) between two shapes;
//Outputs:;
//Union: $op = union;
//Subtraction: $op = subtraction;
//Intersection: $op = intersection;
//Output - sdf3dc;
//sdf3d_smooth_$op($in1($uv), $in2($uv), $k);
//Inputs:;
//in1, vec2, default:vec2(100, 0.0), (sdf3d input);
//in2, vec2, default:vec2(100, 0.0), (sdf3d input);
//operation, enum, default: 1, values: Union, Subtraction, Intersection;
//smoothness, float, min: 0, max: 1, default:0, step:0.01;
//----------------------;
//sdf3d_elongation.mmg;
//Outputs:;
//Output - sdf3dc;
//$in($uv-clamp($uv, -abs(vec3($x, $y, $z)), abs(vec3($x, $y, $z))));
//Inputs:;
//in, vec2, default:vec2(100, 0.0), (sdf3dc input);
//elongation, vector3, min: 0, max: 1, default:0, step:0.01;
//----------------------;
//sdf3d_extrusion.mmg;
//Outputs:;
//vec2 $(name_uv)_w = vec2($in($uv.xz+vec2(0.5)),abs($uv.y)-$d);
//Output - sdf3dc;
//min(MAX($(name_uv)_w.x,$(name_uv)_w.y),0.0)+length(MAX($(name_uv)_w,0.0));
//Inputs:;
//in, sdf2d, default:100, (input);
//length, float, min: 0, max: 1, default:0.25, step:0.01;
//----------------------;
//sdf3d_morph.mmg;
//Outputs:;
//Output - sdf3d;
//mix($in1($uv), $in2($uv), $amount);
//Inputs:;
//in1, vec2, default:vec2(100, 0.0), (sdf3d input);
//in2, vec2, default:vec2(100, 0.0), (sdf3d input);
//amount, float, min: 0, max: 1, default:0.5, step:0.01;
//----------------------;
//raymarching.mmg (raymarching_preview.mmg);
//Raymarches a 3D object (described as signed distance function with optional color index);
//to render a heightmap, a normal map and a color index map.;
//raymarch_$name = sdf3d_raymarch;
//vec2 $(name_uv)_d = raymarch_$name($uv);
//Outputs:;
//HeightMap - float - The generated height map;
//1.0-$(name_uv)_d.x;
//NormalMap - rgb - The generated normal map;
//vec3(0.5)+0.5*normal_$name(vec3($uv-vec2(0.5), 1.0-$(name_uv)_d.x));
//ColorMap - float - The generated color index map;
//$(name_uv)_d.y;
//Inputs:;
//input, vec2, default:vec2(100, 0.0), (sdf3dc input);
//----------------------;
//raymarching_preview.mmg;
//Outputs:;
//Output (rgb);
//render_$name($uv-vec2(0.5));
//Inputs:;
//input, vec2, default:vec2(100, 0.0), (sdf3dc input);

Color MMAlgos::raymarch(const Vector2 &uv) {
	Vector2 d = sdf3d_raymarch(uv);
	float f = 1.0 - d.x;
	return Color(f, f, f, 1);
}

Color MMAlgos::raymarch2(const Vector2 &uv) {
	Vector2 d = sdf3d_raymarch(uv);
	Vector3 v = Vector3(0.5, 0.5, 0.5) + 0.5 * sdf3d_normal(Vector3(uv.x - 0.5, uv.y - 0.5, 1.0 - d.x));
	return Color(v.x, v.y, v.z, 1);
}

Color MMAlgos::raymarch3(const Vector2 &uv) {
	Vector2 v = sdf3d_raymarch(uv);
	return Color(v.y, v.y, v.y, 1);
}

//length($uv)-$r;

Vector2 MMAlgos::sdf3d_sphere(const Vector3 &p, const float r) {
	float s = p.length() - r;
	return Vector2(s, 0.0);
}

//vec3 $(name_uv)_q = abs($uv) - vec3($sx, $sy, $sz);
//length(MAX($(name_uv)_q,0.0))+min(MAX($(name_uv)_q.x,MAX($(name_uv)_q.y,$(name_uv)_q.z)),0.0)-$r;

Vector2 MMAlgos::sdf3d_box(const Vector3 &p, const float sx, const float sy, const float sz, const float r) {
	Vector3 v = absv3((p)) - Vector3(sx, sy, sz);
	float f = (maxv3(v, Vector3())).length() + MIN(MAX(v.x, MAX(v.y, v.z)), 0.0) - r;
	return Vector2(f, 0.0);
}

//Y: $axis = length($uv.xz),$uv.y;
//vec2 $(name_uv)_d = abs(vec2($axis)) - vec2($r,$l);

Vector2 MMAlgos::sdf3d_cylinder_y(const Vector3 &p, const float r, const float l) {
	Vector2 v = absv2(Vector2(Vector2(p.x, p.z).length(), (p).y)) - Vector2(r, l);
	float f = MIN(MAX(v.x, v.y), 0.0) + maxv2(v, Vector2()).length();
	return Vector2(f, 0.0);
}

//X: $axis = length($uv.yz),$uv.x;
//vec2 $(name_uv)_d = abs(vec2($axis)) - vec2($r,$l);

Vector2 MMAlgos::sdf3d_cylinder_x(const Vector3 &p, const float r, const float l) {
	Vector2 v = absv2(Vector2(Vector2(p.y, p.z).length(), (p).x)) - Vector2(r, l);
	float f = MIN(MAX(v.x, v.y), 0.0) + maxv2(v, Vector2()).length();
	return Vector2(f, 0.0);
}

//Z: $axis = length($uv.xy),$uv.z;
//vec2 $(name_uv)_d = abs(vec2($axis)) - vec2($r,$l);

Vector2 MMAlgos::sdf3d_cylinder_z(const Vector3 &p, const float r, const float l) {
	Vector2 v = absv2(Vector2(Vector2(p.x, p.y).length(), (p).z)) - Vector2(r, l);
	float f = MIN(MAX(v.x, v.y), 0.0) + maxv2(v, Vector2()).length();
	return Vector2(f, 0.0);
}

//vec3 $(name_uv)_p = $uv;
//$(name_uv)_p.$axis -= CLAMP($(name_uv)_p.$axis, -$l, $l);
//return length($(name_uv)_p)-$r*$profile(clamp(0.5+0.5*($uv).$axis/$l, 0.0, 1.0));

Vector2 MMAlgos::sdf3d_capsule_y(const Vector3 &p, const float r, const float l) {
	Vector3 v = p;
	v.y -= CLAMP(v.y, -l, l);
	float f = v.length() - r;
	return Vector2(f, 0.0);
}

Vector2 MMAlgos::sdf3d_capsule_x(const Vector3 &p, const float r, const float l) {
	Vector3 v = p;
	v.x -= CLAMP(v.x, -l, l);
	float f = v.length() - r;
	return Vector2(f, 0.0);
}

Vector2 MMAlgos::sdf3d_capsule_z(const Vector3 &p, const float r, const float l) {
	Vector3 v = p;
	v.z -= CLAMP(v.z, -l, l);
	float f = v.length() - r;
	return Vector2(f, 0.0);
}

//+X: $axis = length($uv.yz),-$uv.x;
//dot(vec2(cos($a*0.01745329251),sin($a*0.01745329251)),vec2($axis));

Vector2 MMAlgos::sdf3d_cone_px(const Vector3 &p, const float a) {
	float f = Vector2(cos(a * 0.01745329251), sin(a * 0.01745329251)).dot(Vector2(Vector2(p.y, p.z).length(), -(p).x));
	return Vector2(f, 0.0);
}

//-X: $axis = length($uv.yz),$uv.x;
//dot(vec2(cos($a*0.01745329251),sin($a*0.01745329251)),vec2($axis));

Vector2 MMAlgos::sdf3d_cone_nx(const Vector3 &p, const float a) {
	float f = Vector2(cos(a * 0.01745329251), sin(a * 0.01745329251)).dot(Vector2(Vector2(p.y, p.z).length(), (p).x));
	return Vector2(f, 0.0);
}

//+Y: $axis = length($uv.xz),$uv.y;
//dot(vec2(cos($a*0.01745329251),sin($a*0.01745329251)),vec2($axis));

Vector2 MMAlgos::sdf3d_cone_py(const Vector3 &p, const float a) {
	float f = Vector2(cos(a * 0.01745329251), sin(a * 0.01745329251)).dot(Vector2(Vector2(p.x, p.z).length(), (p).y));
	return Vector2(f, 0.0);
}

//-Y: $axis = length($uv.xz),-$uv.y;
//dot(vec2(cos($a*0.01745329251),sin($a*0.01745329251)),vec2($axis));

Vector2 MMAlgos::sdf3d_cone_ny(const Vector3 &p, const float a) {
	float f = Vector2(cos(a * 0.01745329251), sin(a * 0.01745329251)).dot(Vector2(Vector2(p.x, p.z).length(), -(p).y));
	return Vector2(f, 0.0);
}

//+Z: $axis = length($uv.xy),-$uv.z;
//dot(vec2(cos($a*0.01745329251),sin($a*0.01745329251)),vec2($axis));

Vector2 MMAlgos::sdf3d_cone_pz(const Vector3 &p, const float a) {
	float f = Vector2(cos(a * 0.01745329251), sin(a * 0.01745329251)).dot(Vector2(Vector2(p.x, p.y).length(), -(p).z));
	return Vector2(f, 0.0);
}

//-Z: $axis = length($uv.xy),$uv.z;
//dot(vec2(cos($a*0.01745329251),sin($a*0.01745329251)),vec2($axis));

Vector2 MMAlgos::sdf3d_cone_nz(const Vector3 &p, const float a) {
	float f = Vector2(cos(a * 0.01745329251), sin(a * 0.01745329251)).dot(Vector2(Vector2(p.x, p.y).length(), (p).z));
	return Vector2(f, 0.0);
}

Vector2 MMAlgos::sdf3d_torus_x(const Vector3 &p, const float R, const float r) {
	Vector2 q = Vector2(Vector2(p.y, p.z).length() - R, (p).x);
	float f = q.length() - r;
	return Vector2(f, 0.0);
}

Vector2 MMAlgos::sdf3d_torus_y(const Vector3 &p, const float R, const float r) {
	Vector2 q = Vector2(Vector2(p.z, p.x).length() - R, (p).y);
	float f = q.length() - r;
	return Vector2(f, 0.0);
}

Vector2 MMAlgos::sdf3d_torus_z(const Vector3 &p, const float R, const float r) {
	Vector2 q = Vector2(Vector2(p.x, p.y).length() - R, (p).z);
	float f = q.length() - r;
	return Vector2(f, 0.0);
}

//vec2 raymarch_$name(vec2 uv) {;
//	vec3 ro = vec3(uv-vec2(0.5), 1.0);
//	vec3 rd = vec3(0.0, 0.0, -1.0);
//	float dO = 0.0;
//	float c = 0.0;
//;
//	for (int i=0; i < 100; i++) {;
//		vec3 p = ro + rd*dO;
//		vec2 dS = $sdf(p);
//		dO += dS.x;
//;
//		if (dO >= 1.0) {;
//			break;
//		} else if (dS.x < 0.0001) {;
//			c = dS.y;
//			break;
//		};
//	};
//;
//	return vec2(dO, c);
//};

Vector2 MMAlgos::sdf3d_raymarch(const Vector2 &uv) {
	Vector3 ro = Vector3(uv.x - 0.5, uv.y - 0.5, 1.0);
	Vector3 rd = Vector3(0.0, 0.0, -1.0);
	float dO = 0.0;
	float c = 0.0;

	for (int i = 0; i < 100; ++i) { //i in range(100)
		Vector3 p = ro + rd * dO;
		Vector2 dS = sdf3d_input(p);
		dO += dS.x;

		if ((dO >= 1.0)) {
			break;
		} else if ((dS.x < 0.0001)) {
			c = dS.y;
			break;
		}
	}

	return Vector2(dO, c);
}

//vec3 normal_$name(vec3 p) {;
//	if (p.z <= 0.0) {;
//		return vec3(0.0, 0.0, 1.0);
//	};
//;
//	float d = $sdf(p).x;
//	float e = .001;
//	vec3 n = d - vec3(;
//		$sdf(p-vec3(e, 0.0, 0.0)).x,;
//		$sdf(p-vec3(0.0, e, 0.0)).x,;
//		$sdf(p-vec3(0.0, 0.0, e)).x);
//;
//	return vec3(-1.0, -1.0, -1.0)*normalize(n);
//};

Vector3 MMAlgos::sdf3d_normal(const Vector3 &p) {
	if ((p.z <= 0.0)) {
		return Vector3(0.0, 0.0, 1.0);
	}

	float d = sdf3d_input(p).x;
	float e = .001;
	Vector3 n = Vector3(d - sdf3d_input(p - Vector3(e, 0.0, 0.0)).x, d - sdf3d_input(p - Vector3(0.0, e, 0.0)).x, d - sdf3d_input(p - Vector3(0.0, 0.0, e)).x);
	return Vector3(-1.0, -1.0, -1.0) * n.normalized();
}

//vec2 sdf3dc_union(vec2 a, vec2 b) {;
//	return vec2(min(a.x, b.x), mix(b.y, a.y, step(a.x, b.x)));
//};

Vector2 MMAlgos::sdf3dc_union(const Vector2 &a, const Vector2 &b) {
	return Vector2(MIN(a.x, b.x), Math::lerp(b.y, a.y, step(a.x, b.x)));
}

//vec2 sdf3dc_sub(vec2 a, vec2 b) {;
//	return vec2(MAX(-a.x, b.x), a.y);
//};

Vector2 MMAlgos::sdf3dc_sub(const Vector2 &a, const Vector2 &b) {
	return Vector2(MAX(-a.x, b.x), a.y);
}

//vec2 sdf3dc_inter(vec2 a, vec2 b) {;
//	return vec2(MAX(a.x, b.x), mix(a.y, b.y, step(a.x, b.x)));
//};

Vector2 MMAlgos::sdf3dc_inter(const Vector2 &a, const Vector2 &b) {
	return Vector2(MAX(a.x, b.x), Math::lerp(a.y, b.y, step(a.x, b.x)));
}

//vec2 sdf3d_smooth_union(vec2 d1, vec2 d2, float k) {;
//	float h = CLAMP(0.5+0.5*(d2.x-d1.x)/k, 0.0, 1.0);
//	return vec2(mix(d2.x, d1.x, h)-k*h*(1.0-h), mix(d2.y, d1.y, step(d1.x, d2.x)));
//};

Vector2 MMAlgos::sdf3d_smooth_union(const Vector2 &d1, const Vector2 &d2, const float k) {
	float h = CLAMP(0.5 + 0.5 * (d2.x - d1.x) / k, 0.0, 1.0);
	return Vector2(Math::lerp(d2.x, d1.x, h) - k * h * (1.0 - h), Math::lerp(d2.y, d1.y, step(d1.x, d2.x)));
}

//vec2 sdf3d_smooth_subtraction(vec2 d1, vec2 d2, float k ) {;
//	float h = CLAMP(0.5-0.5*(d2.x+d1.x)/k, 0.0, 1.0);
//	return vec2(mix(d2.x, -d1.x, h )+k*h*(1.0-h), d2.y);
//};

Vector2 MMAlgos::sdf3d_smooth_subtraction(const Vector2 &d1, const Vector2 &d2, const float k) {
	float h = CLAMP(0.5 - 0.5 * (d2.x + d1.x) / k, 0.0, 1.0);
	return Vector2(Math::lerp(d2.x, -d1.x, h) + k * h * (1.0 - h), d2.y);
}

//vec2 sdf3d_smooth_intersection(vec2 d1, vec2 d2, float k ) {;
//	float h = CLAMP(0.5-0.5*(d2.x-d1.x)/k, 0.0, 1.0);
//	return vec2(mix(d2.x, d1.x, h)+k*h*(1.0-h), mix(d1.y, d2.y, step(d1.x, d2.x)));
//};

Vector2 MMAlgos::sdf3d_smooth_intersection(const Vector2 &d1, const Vector2 &d2, const float k) {
	float h = CLAMP(0.5 - 0.5 * (d2.x - d1.x) / k, 0.0, 1.0);
	return Vector2(Math::lerp(d2.x, d1.x, h) + k * h * (1.0 - h), Math::lerp(d1.y, d2.y, step(d1.x, d2.x)));
}

Vector2 MMAlgos::sdf3d_rounded(const Vector2 &v, const float r) {
	return Vector2(v.x - r, v.y);
}

Vector3 MMAlgos::sdf3d_elongation(const Vector3 &p, const Vector3 &v) {
	return ((p)-clampv3((p), -absv3(v), absv3(v)));
}

Vector3 MMAlgos::sdf3d_repeat(const Vector3 &p, const Vector2 &r, const float randomness, const int pseed) {
	//$in(repeat($uv, vec3(1.0/$rx, 1.0/$ry, 0.0), float($seed), $r));
	return repeat(p, Vector3(1.0 / r.x, 1.0 / r.y, 0.00001), float(pseed), randomness);
}

//vec3 repeat(vec3 p, vec3 r, float seed, float randomness) {;
//	vec3 a = (rand3(floor(mod((p.xy+0.5*r.xy)/r.xy, 1.0/r.xy)+vec2(seed)))-0.5)*6.28*randomness;
//	p = mod(p+0.5*r,r)-0.5*r;
//;
//	vec3 rv;
//	float c;
//	float s;
//;
//	c = cos(a.x);
//	s = sin(a.x);
//;
//	rv.x = p.x;
//	rv.y = p.y*c+p.z*s;rv.z = -p.y*s+p.z*c;
//;
//	c = cos(a.y);
//	s = sin(a.y);
//;
//	p.x = rv.x*c+rv.z*s;
//	p.y = rv.y;
//	p.z = -rv.x*s+rv.z*c;
//;
//	c = cos(a.z);
//	s = sin(a.z);
//;
//	rv.x = p.x*c+p.y*s;
//	rv.y = -p.x*s+p.y*c;
//	rv.z = p.z;
//;
//	return rv;
//};

Vector3 MMAlgos::repeat(const Vector3 &p_p, const Vector3 &r, const float pseed, const float randomness) {
	Vector3 p = p_p;

	Vector3 a = (rand3(floorv2(modv2((Vector2(p.x, p.y) + Vector2(0.5, 0.5) * Vector2(r.x, r.y)) / Vector2(r.x, r.y), Vector2(1, 1) / Vector2(r.x, r.y)) + Vector2(pseed, pseed))) - Vector3(0.5, 0.5, 0.5)) * 6.28 * randomness;
	p = modv3(p + Vector3(0.5, 0.5, 0.5) * r, r) - Vector3(0.5, 0.5, 0.5) * r;
	Vector3 rv = Vector3();
	float c = 0;
	float s = 0;
	c = cos(a.x);
	s = sin(a.x);
	rv.x = p.x;
	rv.y = p.y * c + p.z * s;
	rv.z = -p.y * s + p.z * c;
	c = cos(a.y);
	s = sin(a.y);
	p.x = rv.x * c + rv.z * s;
	p.y = rv.y;
	p.z = -rv.x * s + rv.z * c;
	c = cos(a.z);
	s = sin(a.z);
	rv.x = p.x * c + p.y * s;
	rv.y = -p.x * s + p.y * c;
	rv.z = p.z;
	return rv;
}

//vec3 rotate3d(vec3 p, vec3 a) {;
//	vec3 rv;
//	float c;
//	float s;
//	c = cos(a.x);
//	s = sin(a.x);
//	rv.x = p.x;
//	rv.y = p.y*c+p.z*s;
//	rv.z = -p.y*s+p.z*c;
//	c = cos(a.y);
//	s = sin(a.y);
//	p.x = rv.x*c+rv.z*s;
//	p.y = rv.y;
//	p.z = -rv.x*s+rv.z*c;
//	c = cos(a.z);
//	s = sin(a.z);
//	rv.x = p.x*c+p.y*s;
//	rv.y = -p.x*s+p.y*c;
//	rv.z = p.z;
//	return rv;
//};

Vector3 MMAlgos::rotate3d(const Vector3 &p_p, const Vector3 &a) {
	Vector3 p = p_p;

	Vector3 rv = Vector3();
	float c = 0;
	float s = 0;
	c = cos(a.x);
	s = sin(a.x);
	rv.x = p.x;
	rv.y = p.y * c + p.z * s;
	rv.z = -p.y * s + p.z * c;
	c = cos(a.y);
	s = sin(a.y);
	p.x = rv.x * c + rv.z * s;
	p.y = rv.y;
	p.z = -rv.x * s + rv.z * c;
	c = cos(a.z);
	s = sin(a.z);
	rv.x = p.x * c + p.y * s;
	rv.y = -p.x * s + p.y * c;
	rv.z = p.z;
	return rv;
}

//vec3 circle_repeat_transform(vec3 p, float count) {;
//	float r = 6.28/count;
//	float pa = atan(p.x, p.y);
//	float a = mod(pa+0.5*r, r)-0.5*r;
//	vec3 rv;
//	float c = cos(a-pa);
//	float s = sin(a-pa);
//	rv.x = p.x*c+p.y*s;
//	rv.y = -p.x*s+p.y*c;
//	rv.z = p.z;
//	return rv;
//};

Vector3 MMAlgos::circle_repeat_transform(const Vector3 &p, const float count) {
	float r = 6.28 / count;
	float pa = atan2(p.x, p.y);
	float a = modf(pa + 0.5 * r, r) - 0.5 * r;
	Vector3 rv = Vector3();
	float c = cos(a - pa);
	float s = sin(a - pa);
	rv.x = p.x * c + p.y * s;
	rv.y = -p.x * s + p.y * c;
	rv.z = p.z;
	return rv;
}

//todo this needs to be solved;

Vector2 MMAlgos::sdf3d_input(const Vector3 &p) {
	return sdf3d_sphere(p, 0.5);
}

//raymarching_preview.mmg;
//vec3 render_$name(vec2 uv) {;
//	vec3 p = vec3(uv, 2.0-raymarch_$name(vec3(uv, 2.0), vec3(0.0, 0.0, -1.0)));
//	vec3 n = normal_$name(p);
//	vec3 l = vec3(5.0, 5.0, 10.0);
//	vec3 ld = normalize(l-p);
//;
//	float o = step(p.z, 0.001);
//	float shadow = 1.0-0.75*step(raymarch_$name(l, -ld), length(l-p)-0.01);
//	float light = 0.3+0.7*dot(n, ld)*shadow;
//;
//	return vec3(0.8+0.2*o, 0.8+0.2*o, 1.0)*light;
//};
//static func sdf3d_render(p : Vector2) -> Vector3:;
//	return Vector3();
// ========================= SHAPES.GD =========================;
//----------------------;
//sphere.mmg;
//Outputs:;
//Output - (float) - A heightmap of the specified sphere;
//sphere($uv, vec2($cx, $cy), $r);
//Inputs:;
//center, vector2, default: 0.5, min: 0, max: 1, step: 0.01;
//radius, float, min: 0, max: 1, default: 0.5, step:0.01;
//----------------------;
//shape.mmg;
//Outputs:;
//Output - (float) - Shows a white shape on a black background;
//shape_$(shape)($(uv), $(sides), $(radius)*$radius_map($uv), $(edge)*$edge_map($uv));
//Inputs:;
//shape, enum, default: 0, values: circle, ploygon, star, curved_star, rays;
//sides, int, min: 2, max: 32, default: 3, step:1;
//radius, float, min: 0, max: 1, default: 1, step:0.01 (universal input);
//edge, float, min: 0, max: 1, default: 0.2, step:0.01 (universal input);
//----------------------;
//box.mmg;

//float sphere(vec2 uv, vec2 c, float r) {;
//	uv -= c;
//	uv /= r;
//	return 2.0*r*sqrt(MAX(0.0, 1.0-dot(uv, uv)));
//};

float MMAlgos::sphere(const Vector2 &uv, const Vector2 &c, const float r) {
	return 0.0;
}

//float shape_circle(vec2 uv, float sides, float size, float edge) {;
//	uv = 2.0*uv-1.0;
//	edge = MAX(edge, 1.0e-8);
//	float distance = length(uv);
//	return CLAMP((1.0-distance/size)/edge, 0.0, 1.0);
//};

float MMAlgos::shape_circle(const Vector2 &uuv, const float sides, const float size, const float p_edge) {
	float edge = p_edge;

	Vector2 uv = uuv;
	uv.x = 2.0 * uv.x - 1.0;
	uv.y = 2.0 * uv.y - 1.0;
	edge = MAX(edge, 1.0e-8);
	float distance = uv.length();
	return CLAMP((1.0 - distance / size) / edge, 0.0, 1.0);
}

//float shape_polygon(vec2 uv, float sides, float size, float edge) {;
//	uv = 2.0*uv-1.0;
//	edge = MAX(edge, 1.0e-8);
//	float angle = atan(uv.x, uv.y)+3.14159265359;
//	float slice = 6.28318530718/sides;
//	return CLAMP((1.0-(cos(floor(0.5+angle/slice)*slice-angle)*length(uv))/size)/edge, 0.0, 1.0);
//};

float MMAlgos::shape_polygon(const Vector2 &uuv, const float sides, const float size, const float p_edge) {
	float edge = p_edge;

	Vector2 uv = uuv;
	uv.x = 2.0 * uv.x - 1.0;
	uv.y = 2.0 * uv.y - 1.0;
	edge = MAX(edge, 1.0e-8);
	//simple no branch for division by zero;
	uv.x += 0.0000001;
	float angle = atan(uv.y / uv.x) + 3.14159265359;
	float slice = 6.28318530718 / sides;
	return CLAMP((size - cos(floor(0.5 + angle / slice) * slice - angle) * uv.length()) / (edge * size), 0.0, 1.0);
}

//float shape_star(vec2 uv, float sides, float size, float edge) {;
//	uv = 2.0*uv-1.0;
//	edge = MAX(edge, 1.0e-8);
//	float angle = atan(uv.x, uv.y);
//	float slice = 6.28318530718/sides;
//	return CLAMP((1.0-(cos(floor(angle*sides/6.28318530718-0.5+2.0*step(fract(angle*sides/6.28318530718), 0.5))*slice-angle)*length(uv))/size)/edge, 0.0, 1.0);
//};

float MMAlgos::shape_star(const Vector2 &uuv, const float sides, const float size, const float p_edge) {
	Vector2 uv = uuv;
	float edge = p_edge;

	uv.x = 2.0 * uv.x - 1.0;
	uv.y = 2.0 * uv.y - 1.0;
	edge = MAX(edge, 1.0e-8);
	//simple no branch for division by zero;
	uv.x += 0.0000001;
	float angle = atan(uv.y / uv.x);
	float slice = 6.28318530718 / sides;
	return CLAMP((size - cos(floor(1.5 + angle / slice - 2.0 * step(0.5 * slice, modf(angle, slice))) * slice - angle) * uv.length()) / (edge * size), 0.0, 1.0);
}

//float shape_curved_star(vec2 uv, float sides, float size, float edge) {;
//	uv = 2.0*uv-1.0;
//	edge = MAX(edge, 1.0e-8);
//	float angle = 2.0*(atan(uv.x, uv.y)+3.14159265359);
//	float slice = 6.28318530718/sides;
//	return CLAMP((1.0-cos(floor(0.5+0.5*angle/slice)*2.0*slice-angle)*length(uv)/size)/edge, 0.0, 1.0);
//};

float MMAlgos::shape_curved_star(const Vector2 &uuv, const float sides, const float size, const float p_edge) {
	Vector2 uv = uuv;
	float edge = p_edge;

	uv.x = 2.0 * uv.x - 1.0;
	uv.y = 2.0 * uv.y - 1.0;
	edge = MAX(edge, 1.0e-8);
	//simple no branch for division by zero;
	uv.x += 0.0000001;
	float angle = 2.0 * (atan(uv.y / uv.x) + 3.14159265359);
	float slice = 6.28318530718 / sides;
	return CLAMP((size - cos(floor(0.5 + 0.5 * angle / slice) * 2.0 * slice - angle) * uv.length()) / (edge * size), 0.0, 1.0);
}

//float shape_rays(vec2 uv, float sides, float size, float edge) {;
//	uv = 2.0*uv-1.0;
//	edge = 0.5*MAX(edge, 1.0e-8)*size;
//	float slice = 6.28318530718/sides;
//	float angle = mod(atan(uv.x, uv.y)+3.14159265359, slice)/slice;
//	return CLAMP(min((size-angle)/edge, angle/edge), 0.0, 1.0);
//};

float MMAlgos::shape_rays(const Vector2 &uuv, const float sides, const float size, const float p_edge) {
	Vector2 uv = uuv;
	float edge = p_edge;

	uv.x = 2.0 * uv.x - 1.0;
	uv.y = 2.0 * uv.y - 1.0;
	edge = 0.5 * MAX(edge, 1.0e-8) * size;
	//simple no branch for division by zero;
	uv.x += 0.0000001;
	float slice = 6.28318530718 / sides;
	float angle = modf(atan(uv.y / uv.x) + 3.14159265359, slice) / slice;
	return CLAMP(MIN((size - angle) / edge, angle / edge), 0.0, 1.0);
}

//float box(vec2 uv, vec3 center, vec3 rad, vec3 rot) {\n\t;
//	vec3 ro = vec3(uv, 1.0)-center;\n\t;
//	vec3 rd = vec3(0.0000001, 0.0000001, -1.0);\n\t;
//	mat3 r = mat3(vec3(1, 0, 0), vec3(0, cos(rot.x), -sin(rot.x)), vec3(0, sin(rot.x), cos(rot.x)));\n\t;
//;
//	r *= mat3(vec3(cos(rot.y), 0, -sin(rot.y)), vec3(0, 1, 0), vec3(sin(rot.y), 0, cos(rot.y)));\n\t;
//	r *= mat3(vec3(cos(rot.z), -sin(rot.z), 0), vec3(sin(rot.z), cos(rot.z), 0), vec3(0, 0, 1));\n\t;
//	ro = r * ro;\n\t;
//	rd = r * rd;\n;
//	vec3 m = 1.0/rd;\n;
//	vec3 n = m*ro;\n;
//	vec3 k = abs(m)*rad;\n;
//	vec3 t1 = -n - k;\n;
//	vec3 t2 = -n + k;\n\n;
//;
//	float tN = MAX(MAX(t1.x, t1.y), t1.z);\n;
//	float tF = MIN(min(t2.x, t2.y), t2.z);\n;
//;
//	if(tN>tF || tF<0.0) return 1.0;\n;
//;
//	return tN;\n;
//};

// ============================== SIMPLE.GD ========================;
//----------------------;
//profile.mmg;
//----------------------;
//uniform_greyscale.mmg;

//float draw_profile_fill(vec2 uv, float y, float dy, float w) {\n\t;
//	return 1.0-clamp(sin(1.57079632679-atan(dy))*(1.0-uv.y-y)/w, 0.0, 1.0);\n;
//};
//float draw_profile_curve(vec2 uv, float y, float dy, float w) {\n\t;
//	return 1.0-clamp(sin(1.57079632679-atan(dy))*abs(1.0-uv.y-y)/w, 0.0, 1.0);\n;
//};

// ======================== TEX3D.GD =============================;
//----------------------;
//tex3d_apply.mmg;
//----------------------;
//tex3d_apply_invuvmap.mmg;
//----------------------;
//tex3d_blend.mmg;
//----------------------;
//tex3d_colorize.mmg;
//----------------------;
//tex3d_distort.mmg;
//----------------------;
//tex3d_fbm.mmg;
//----------------------;
//tex3d_from2d.mmg;
//----------------------;
//tex3d_rotate.mmg;
//----------------------;
//tex3d_select.mmg;
//----------------------;
//tex3d_select_shape.mmg;
//----------------------;
//tex3d_apply.mmg;

//vec3 blend3d_normal(vec3 c1, vec3 c2, float opacity) {\n\t;
//	return opacity*c1 + (1.0-opacity)*c2;\n;
//};

//vec3 blend3d_multiply(vec3 c1, vec3 c2, float opacity) {\n\t;
//	return opacity*c1*c2 + (1.0-opacity)*c2;\n;
//};

//vec3 blend3d_screen(vec3 c1, vec3 c2, float opacity) {\n\t;
//	return opacity*(1.0-(1.0-c1)*(1.0-c2)) + (1.0-opacity)*c2;\n;
//};

//float blend3d_overlay_f(float c1, float c2) {\n\t;
//	return (c1 < 0.5) ? (2.0*c1*c2) : (1.0-2.0*(1.0-c1)*(1.0-c2));\n;
//};

//vec3 blend3d_overlay(vec3 c1, vec3 c2, float opacity) {\n\t;
//	return opacity*vec3(blend3d_overlay_f(c1.x, c2.x), blend3d_overlay_f(c1.y, c2.y), blend3d_overlay_f(c1.z, c2.z)) + (1.0-opacity)*c2;\n;
//};

//vec3 blend3d_hard_light(vec3 c1, vec3 c2, float opacity) {\n\t;
//	return opacity*0.5*(c1*c2+blend3d_overlay(c1, c2, 1.0)) + (1.0-opacity)*c2;\n;
//};

//float blend3d_soft_light_f(float c1, float c2) {\n\t;
//	return (c2 < 0.5) ? (2.0*c1*c2+c1*c1*(1.0-2.0*c2)) : 2.0*c1*(1.0-c2)+sqrt(c1)*(2.0*c2-1.0);\n;
//};

//vec3 blend3d_soft_light(vec3 c1, vec3 c2, float opacity) {\n\t;
//	return opacity*vec3(blend3d_soft_light_f(c1.x, c2.x), blend3d_soft_light_f(c1.y, c2.y), blend3d_soft_light_f(c1.z, c2.z)) + (1.0-opacity)*c2;\n;
//};

//float blend3d_burn_f(float c1, float c2) {\n\t;
//	return (c1==0.0)?c1:MAX((1.0-((1.0-c2)/c1)),0.0);\n;
//};

//vec3 blend3d_burn(vec3 c1, vec3 c2, float opacity) {\n\t;
//	return opacity*vec3(blend3d_burn_f(c1.x, c2.x), blend3d_burn_f(c1.y, c2.y), blend3d_burn_f(c1.z, c2.z)) + (1.0-opacity)*c2;\n;
//};

//float blend3d_dodge_f(float c1, float c2) {\n\t;
//	return (c1==1.0)?c1:min(c2/(1.0-c1),1.0);\n;
//};

//vec3 blend3d_dodge(vec3 c1, vec3 c2, float opacity) {\n\t;
//	return opacity*vec3(blend3d_dodge_f(c1.x, c2.x), blend3d_dodge_f(c1.y, c2.y), blend3d_dodge_f(c1.z, c2.z)) + (1.0-opacity)*c2;\n;
//};

//vec3 blend3d_lighten(vec3 c1, vec3 c2, float opacity) {\n\t;
//	return opacity*MAX(c1, c2) + (1.0-opacity)*c2;\n;
//};

//vec3 blend3d_darken(vec3 c1, vec3 c2, float opacity) {\n\t;
//	return opacity*min(c1, c2) + (1.0-opacity)*c2;
//};

//vec3 blend3d_difference(vec3 c1, vec3 c2, float opacity) {\n\t;
//	return opacity*clamp(c2-c1, vec3(0.0), vec3(1.0)) + (1.0-opacity)*c2;\n;
//};

//float rand31(vec3 p) {\n\t;
//	return fract(sin(dot(p,vec3(127.1,311.7, 74.7)))*43758.5453123);\n;
//};

//vec3 rand33(vec3 p) {\n\t;
//	p = vec3( dot(p,vec3(127.1,311.7, 74.7)),;
//		dot(p,vec3(269.5,183.3,246.1)),\n\t\t\t;
//		dot(p,vec3(113.5,271.9,124.6)));\n\n\t;
//;
//	return -1.0 + 2.0*fract(sin(p)*43758.5453123);
//};

//float tex3d_fbm_value(vec3 coord, vec3 size, float seed) {\n\t;
//	vec3 o = Math::floor(coord)+rand3(vec2(seed, 1.0-seed))+size;\n\t;
//	vec3 f = fract(coord);\n\t;
//	float p000 = rand31(mod(o, size));\n\t;
//	float p001 = rand31(mod(o + vec3(0.0, 0.0, 1.0), size));\n\t;
//	float p010 = rand31(mod(o + vec3(0.0, 1.0, 0.0), size));\n\t;
//	float p011 = rand31(mod(o + vec3(0.0, 1.0, 1.0), size));\n\t;
//	float p100 = rand31(mod(o + vec3(1.0, 0.0, 0.0), size));\n\t;
//	float p101 = rand31(mod(o + vec3(1.0, 0.0, 1.0), size));\n\t;
//	float p110 = rand31(mod(o + vec3(1.0, 1.0, 0.0), size));\n\t;
//	float p111 = rand31(mod(o + vec3(1.0, 1.0, 1.0), size));\n\t;
//;
//	vec3 t = f * f * (3.0 - 2.0 * f);\n\t;
//;
//	return mix(mix(mix(p000, p100, t.x), mix(p010, p110, t.x), t.y), mix(mix(p001, p101, t.x), mix(p011, p111, t.x), t.y), t.z);\n;
//};

//float tex3d_fbm_perlin(vec3 coord, vec3 size, float seed) {\n\t;
//	vec3 o = Math::floor(coord)+rand3(vec2(seed, 1.0-seed))+size;\n\t;
//	vec3 f = fract(coord);\n\t;
//	vec3 v000 = normalize(rand33(mod(o, size))-vec3(0.5));\n\t;
//	vec3 v001 = normalize(rand33(mod(o + vec3(0.0, 0.0, 1.0), size))-vec3(0.5));\n\t;
//	vec3 v010 = normalize(rand33(mod(o + vec3(0.0, 1.0, 0.0), size))-vec3(0.5));\n\t;
//	vec3 v011 = normalize(rand33(mod(o + vec3(0.0, 1.0, 1.0), size))-vec3(0.5));\n\t;
//	vec3 v100 = normalize(rand33(mod(o + vec3(1.0, 0.0, 0.0), size))-vec3(0.5));\n\t;
//	vec3 v101 = normalize(rand33(mod(o + vec3(1.0, 0.0, 1.0), size))-vec3(0.5));\n\t;
//	vec3 v110 = normalize(rand33(mod(o + vec3(1.0, 1.0, 0.0), size))-vec3(0.5));\n\t;
//	vec3 v111 = normalize(rand33(mod(o + vec3(1.0, 1.0, 1.0), size))-vec3(0.5));\n\t;
//;
//	float p000 = dot(v000, f);\n\tfloat p001 = dot(v001, f - vec3(0.0, 0.0, 1.0));\n\t;
//	float p010 = dot(v010, f - vec3(0.0, 1.0, 0.0));\n\t;
//	float p011 = dot(v011, f - vec3(0.0, 1.0, 1.0));\n\t;
//	float p100 = dot(v100, f - vec3(1.0, 0.0, 0.0));\n\t;
//	float p101 = dot(v101, f - vec3(1.0, 0.0, 1.0));\n\t;
//	float p110 = dot(v110, f - vec3(1.0, 1.0, 0.0));\n\t;
//	float p111 = dot(v111, f - vec3(1.0, 1.0, 1.0));\n\t;
//;
//	vec3 t = f * f * (3.0 - 2.0 * f);\n\t;
//;
//	return 0.5 + mix(mix(mix(p000, p100, t.x), mix(p010, p110, t.x), t.y), mix(mix(p001, p101, t.x), mix(p011, p111, t.x), t.y), t.z);
//};

//float tex3d_fbm_cellular(vec3 coord, vec3 size, float seed) {\n\t;
//	vec3 o = Math::floor(coord)+rand3(vec2(seed, 1.0-seed))+size;\n\t;
//	vec3 f = fract(coord);\n\tfloat min_dist = 3.0;\n\t;
//;
//	for (float x = -1.0; x <= 1.0; x++) {\n\t\t;
//		for (float y = -1.0; y <= 1.0; y++) {\n\t\t\t;
//			for (float z = -1.0; z <= 1.0; z++) {\n\t\t\t\t;
//				vec3 node = 0.4*rand33(mod(o + vec3(x, y, z), size)) + vec3(x, y, z);\n\t\t\t\t;
//				float dist = sqrt((f - node).x * (f - node).x + (f - node).y * (f - node).y + (f - node).z * (f - node).z);\n\t\t\t\t;
//				min_dist = MIN(min_dist, dist);\n\t\t\t;
//			}\n\t\t;
//;
//		}\n\t;
//;
//	}\n\t;
//;
//	return min_dist;
//};

//float wave3d_constant(float x) {\n\t;
//	return 1.0;\n;
//};

//float wave3d_sine(float x) {\n\t;
//	return 0.5-0.5*cos(3.14159265359*2.0*x);\n;
//};

//float wave3d_triangle(float x) {\n\t;
//	x = fract(x);\n\t;
//	return MIN(2.0*x, 2.0-2.0*x);\n;
//};

//float wave3d_sawtooth(float x) {\n\t;
//	return fract(x);\n;
//};

//float wave3d_square(float x) {\n\t;
//	return (fract(x) < 0.5) ? 0.0 : 1.0;\n;
//};

//float wave3d_bounce(float x) {\n\t;
//	x = 2.0*(fract(x)-0.5);\n\t;
//	return sqrt(1.0-x*x);\n;
//};

//float mix3d_mul(float x, float y, float z) {\n\t;
//	return x*y*z;\n;
//};

//float mix3d_add(float x, float y, float z) {\n\t;
//	return MIN(x+y+z, 1.0);\n;
//};

//float mix3d_max(float x, float y, float z) {\n\t;
//	return MAX(MAX(x, y), z);\n;
//};

//float mix3d_min(float x, float y, float z) {\n\t;
//	return MIN(min(x, y), z);\n;
//};

//float mix3d_xor(float x, float y, float z) {\n\t;
//	float xy = MIN(x+y, 2.0-x-y);\n\t;
//	return MIN(xy+z, 2.0-xy-z);\n;
//};

//float mix3d_pow(float x, float y, float z) {\n\t;
//	return pow(pow(x, y), z);\n;
//};

//vec3 tex3d_rotate(vec3 p, vec3 a) {\n\t;
//	vec3 rv;\n\t;
//	float c;\n\t;
//	float s;\n\t;
//	c = cos(a.x);\n\t;
//	s = sin(a.x);\n\t;
//	rv.x = p.x;\n\t;
//	rv.y = p.y*c+p.z*s;\n\t;
//	rv.z = -p.y*s+p.z*c;\n\t;
//	c = cos(a.y);\n\t;
//	s = sin(a.y);\n\t;
//	p.x = rv.x*c+rv.z*s;\n\t;
//	p.y = rv.y;\n\t;
//	p.z = -rv.x*s+rv.z*c;\n\t;
//	c = cos(a.z);\n\t;
//	s = sin(a.z);\n\t;
//	rv.x = p.x*c+p.y*s;\n\t;
//	rv.y = -p.x*s+p.y*c;\n\t;
//	rv.z = p.z;\n\t;
//	return rv;\n;
//};

// ================================== TILE.GD ============================;
//----------------------;
//tile2x2.mmg;
//----------------------;
//tile2x2_variations.mmg;
//----------------------;
//tiler.mmg;
//Tiles several occurrences of an input image while adding randomness.;
//vec4 $(name_uv)_rch = tiler_$(name)($uv, vec2($tx, $ty), int($overlap), vec2(float($seed)));
//instance;
//vec4 tiler_$(name)(vec2 uv, vec2 tile, int overlap, vec2 _seed) {\n\t;
//	float c = 0.0;\n\t;
//	vec3 rc = vec3(0.0);\n\t;
//	vec3 rc1;\n\t;
//	for (int dx = -overlap; dx <= overlap; ++dx) {\n\t\t;
//		for (int dy = -overlap; dy <= overlap; ++dy) {\n\t\t\t;
//			vec2 pos = fract((floor(uv*tile)+vec2(float(dx), float(dy))+vec2(0.5))/tile-vec2(0.5));\n\t\t\t;
//			vec2 seed = rand2(pos+_seed);\n\t\t\t;
//			rc1 = rand3(seed);\n\t\t\t;
//			pos = fract(pos+vec2($fixed_offset/tile.x, 0.0)*floor(mod(pos.y*tile.y, 2.0))+$offset*seed/tile);\n\t\t\t;
//			float mask = $mask(fract(pos+vec2(0.5)));\n\t\t\t;
//;
//			if (mask > 0.01) {\n\t\t\t\t;
//				vec2 pv = fract(uv - pos)-vec2(0.5);\n\t\t\t\t;
//				seed = rand2(seed);\n\t\t\t\t;
//					float angle = (seed.x * 2.0 - 1.0) * $rotate * 0.01745329251;\n\t\t\t\t;
//				float ca = cos(angle);\n\t\t\t\t;
//				float sa = sin(angle);\n\t\t\t\t;
//				pv = vec2(ca*pv.x+sa*pv.y, -sa*pv.x+ca*pv.y);\n\t\t\t\t;
//				pv *= (seed.y-0.5)*2.0*$scale+1.0;\n\t\t\t\t;
//				pv /= vec2($scale_x, $scale_y);\n\t\t\t\t;
//				pv += vec2(0.5);\n\t\t\t\t;
//				seed = rand2(seed);\n\t\t\t\t;
//				vec2 clamped_pv = CLAMP(pv, vec2(0.0), vec2(1.0));\n\t\t\t\t;
//				if (pv.x != clamped_pv.x || pv.y != clamped_pv.y) {\n\t\t\t\t\t;
//					continue;\n\t\t\t\t;
//				}\n\t\t\t\t;
//;
//				$select_inputs\n\t\t\t\t;
//;
//				float c1 = $in.variation(pv, $variations ? seed.x : 0.0)*mask*(1.0-$value*seed.x);\n\t\t\t\t;
//				c = MAX(c, c1);\n\t\t\t\t;
//				rc = mix(rc, rc1, step(c, c1));\n\t\t\t;
//			}\n\t\t;
//		}\n\t;
//	}\n\t;
//;
//	return vec4(rc, c);\n;
//};
//Inputs:;
//in, float, default: 0, - The input image or atlas of 4 or 16 input images;
//Mask, float, default: 1, - The mask applied to the pattern;
//Outputs:;
//Output, float, Shows the generated pattern;
//$(name_uv)_rch.a;
//Instance map, rgb, Shows a random color for each instance of the input image;
//$(name_uv)_rch.rgb;
//select_inputs enum;
//1,  " ";
//4, "pv = CLAMP(0.5*(pv+floor(rand2(seed)*2.0)), vec2(0.0), vec2(1.0));";
//16, "pv = CLAMP(0.25*(pv+floor(rand2(seed)*4.0)), vec2(0.0), vec2(1.0));";
//Parameters:;
//tile, Vector2, default 4, min:1, max:64, step:1 - The number of columns of the tiles pattern;
//overlap, float, default 1, min 0, max 5, step 1 - The number of neighbour tiles an instance of the input image can overlap. Set this parameter to the lowest value that generates the expected result (where all instances are fully visible) to improve performance.;
//select_inputs (Inputs), enum, default 0, values 1, 4, 16;
//scale, Vector2, default 1, min:0, max:2, step:0.01 - "The scale of input images on the X axis;
//fixed_offset, float, default 0.5, min 0, max 1, step 0.01 - The relative offset of odd rows;
//offset (rnd_offset), float, default 0.5, min 0, max 1, step 0.01;
//rotate (rnd_rotate), float, default 0, min 0, max 180, step 0.1;
//scale (rnd_scale), float, default 0.5, min 0, max 1, step 0.01 - The random scale applied to each image instance;
//value (rnd_value), float, default 0.5, min 0, max 1, step 0.01 - The random greyscale value applied to each image instance;
//variations, bool, default false, (disabled) - Check to tile variations of the input;
//----------------------;
//tiler_advanced.mmg;
//----------------------;
//tiler_advanced_color.mmg;
//----------------------;
//tiler_color.mmg;
//Tiles several occurrences of an input image while adding randomness.;
//vec3 $(name_uv)_random_color;\n;
//vec4 $(name_uv)_tiled_output = tiler_$(name)($uv, vec2($tx, $ty), int($overlap), vec2(float($seed)), $(name_uv)_random_color);
//vec4 tiler_$(name)(vec2 uv, vec2 tile, int overlap, vec2 _seed, out vec3 random_color) {\n\t;
//	vec4 c = vec4(0.0);\n\t;
//	vec3 rc = vec3(0.0);\n\t;
//	vec3 rc1;\n\t;
//;
//	for (int dx = -overlap; dx <= overlap; ++dx) {\n\t\t;
//		for (int dy = -overlap; dy <= overlap; ++dy) {\n\t\t\t;
//			vec2 pos = fract((floor(uv*tile)+vec2(float(dx), float(dy))+vec2(0.5))/tile-vec2(0.5));\n\t\t\t;
//			vec2 seed = rand2(pos+_seed);\n\t\t\t;
//			rc1 = rand3(seed);\n\t\t\t;
//			pos = fract(pos+vec2($fixed_offset/tile.x, 0.0)*floor(mod(pos.y*tile.y, 2.0))+$offset*seed/tile);\n\t\t\t;
//			float mask = $mask(fract(pos+vec2(0.5)));\n\t\t\t;
//			if (mask > 0.01) {\n\t\t\t\t;
//				vec2 pv = fract(uv - pos)-vec2(0.5);\n\t\t\t\t;
//				seed = rand2(seed);\n\t\t\t\t;
//				float angle = (seed.x * 2.0 - 1.0) * $rotate * 0.01745329251;\n\t\t\t\t;
//				float ca = cos(angle);\n\t\t\t\t;
//				float sa = sin(angle);\n\t\t\t\t;
//				pv = vec2(ca*pv.x+sa*pv.y, -sa*pv.x+ca*pv.y);\n\t\t\t\t;
//				pv *= (seed.y-0.5)*2.0*$scale+1.0;\n\t\t\t\t;
//				pv /= vec2($scale_x, $scale_y);\n\t\t\t\t;
//				pv += vec2(0.5);\n\t\t\t\t;
//				pv = CLAMP(pv, vec2(0.0), vec2(1.0));\n\t\t\t\t;
//;
//				$select_inputs\n\t\t\t\t;
//;
//				vec4 n = $in.variation(pv, $variations ? seed.x : 0.0);\n\t\t\t\t;
//;
//				seed = rand2(seed);\n\t\t\t\t;
//				float na = n.a*mask*(1.0-$opacity*seed.x);\n\t\t\t\t;
//				float a = (1.0-c.a)*(1.0*na);\n\t\t\t\t;
//;
//				c = mix(c, n, na);\n\t\t\t\t;
//				rc = mix(rc, rc1, n.a);\n\t\t\t;
//			}\n\t\t;
//		}\n\t;
//	}\n\t;
//;
//	random_color = rc;\n\t;
//	return c;\n;
//};
//Inputs:;
//in, rgba, default: 0, - The input image or atlas of 4 or 16 input images;
//Mask, float, default: 1, - The mask applied to the pattern;
//Outputs:;
//Output, float, Shows the generated pattern;
//$(name_uv)_tiled_output;
//Instance map, rgb, Shows a random color for each instance of the input image;
//$(name_uv)_random_color;
//select_inputs enum;
//1,  " ";
//4, "pv = CLAMP(0.5*(pv+floor(rand2(seed)*2.0)), vec2(0.0), vec2(1.0));";
//16, "pv = CLAMP(0.25*(pv+floor(rand2(seed)*4.0)), vec2(0.0), vec2(1.0));";
//Parameters:;
//tile, Vector2, default 4, min:1, max:64, step:1 - The number of columns of the tiles pattern;
//overlap, float, default 1, min 0, max 5, step 1 - The number of neighbour tiles an instance of the input image can overlap. Set this parameter to the lowest value that generates the expected result (where all instances are fully visible) to improve performance.;
//select_inputs (Inputs), enum, default 0, values 1, 4, 16;
//scale, Vector2, default 1, min:0, max:2, step:0.01 - "The scale of input images on the X axis;
//fixed_offset, float, default 0.5, min 0, max 1, step 0.01 - The relative offset of odd rows;
//offset (rnd_offset), float, default 0.5, min 0, max 1, step 0.01;
//rotate (rnd_rotate), float, default 0, min 0, max 180, step 0.1;
//scale (rnd_scale), float, default 0.5, min 0, max 1, step 0.01 - The random scale applied to each image instance;
//opacity (rnd_opacity), float, default 0, min 0, max 1, step 0.01 - The random greyscale value applied to each image instance;
//variations, bool, default false, (disabled) - Check to tile variations of the input;

// ========================= TRANSFORMS.GD =======================;

//----------------------;
//transform.mmg;
//----------------------;
//transform2.mmg;
//----------------------;
//translate.mmg;
//----------------------;
//rotate.mmg;
//----------------------;
//scale.mmg;
//----------------------;
//shear.mmg;
//----------------------;
//mirror.mmg;
//----------------------;
//kaleidoscope.mmg;
//----------------------;
//warp.mmg;
//----------------------;
//warp2.mmg;
//----------------------;
//circle_map.mmg;
//----------------------;
//custom_uv.mmg;
//----------------------;
//splatter.mmg;
//Spreads several occurrences of an input image randomly.;
//vec4 $(name_uv)_rch = splatter_$(name)($uv, int($count), vec2(float($seed)));
//vec4 splatter_$(name)(vec2 uv, int count, vec2 seed) {\n\t;
//	float c = 0.0;\n\t;
//	vec3 rc = vec3(0.0);\n\t;
//	vec3 rc1;\n\t;
//;
//	for (int i = 0; i < count; ++i) {\n\t\t;
//		seed = rand2(seed);\n\t\t;
//		rc1 = rand3(seed);\n\t\t;
//		float mask = $mask(fract(seed+vec2(0.5)));\n\t\t;
//;
//		if (mask > 0.01) {\n\t\t\t;
//			vec2 pv = fract(uv - seed)-vec2(0.5);\n\t\t\t;
//			seed = rand2(seed);\n\t\t\t;
//			float angle = (seed.x * 2.0 - 1.0) * $rotate * 0.01745329251;\n\t\t\t;
//			float ca = cos(angle);\n\t\t\t;
//			float sa = sin(angle);\n\t\t\t;
//			pv = vec2(ca*pv.x+sa*pv.y, -sa*pv.x+ca*pv.y);\n\t\t\t;
//			pv *= (seed.y-0.5)*2.0*$scale+1.0;\n\t\t\t;
//			pv /= vec2($scale_x, $scale_y);\n\t\t\t;
//			pv += vec2(0.5);\n\t\t\t;
//			seed = rand2(seed);\n\t\t\t;
//			vec2 clamped_pv = CLAMP(pv, vec2(0.0), vec2(1.0));\n\t\t\t;
//;
//			if (pv.x != clamped_pv.x || pv.y != clamped_pv.y) {\n\t\t\t\t;
//				continue;\n\t\t\t;
//			}\n\t\t\t;
//;
//			$select_inputs\n\t\t\t;
//;
//			float c1 = $in.variation(pv, $variations ? seed.x : 0.0)*mask*(1.0-$value*seed.x);\n\t\t\t;
//			c = MAX(c, c1);\n\t\t\t;
//			rc = mix(rc, rc1, step(c, c1));\n\t\t;
//		}\n\t;
//	}\n\t;
//;
//	return vec4(rc, c);\n;
//};
//Inputs:;
//in, float, default: 0, - The input image or atlas of 4 or 16 input images;
//Mask, float, default: 1, - The mask applied to the pattern;
//Outputs:;
//Output, float, Shows the generated pattern;
//$(name_uv)_rch.a;
//Instance map, rgb, Shows a random color for each instance of the input image;
//$(name_uv)_rch.rgb;
//select_inputs enum;
//1,  " ";
//4, "pv = CLAMP(0.5*(pv+floor(rand2(seed)*2.0)), vec2(0.0), vec2(1.0));";
//16, "pv = CLAMP(0.25*(pv+floor(rand2(seed)*4.0)), vec2(0.0), vec2(1.0));";
//Parameters:;
//count, int, default 25, min 1, max 100, - The number of occurrences of the input image;
//select_inputs (Inputs), enum, default 0, values 1, 4, 16;
//tile, Vector2, default 4, min:1, max:64, step:1 - The number of columns of the tiles pattern;
//overlap, float, default 1, min 0, max 5, step 1 - The number of neighbour tiles an instance of the input image can overlap. Set this parameter to the lowest value that generates the expected result (where all instances are fully visible) to improve performance.;
//scale, Vector2, default 1, min:0, max:2, step:0.01 - "The scale of input images on the X axis;
//rotate (rnd_rotate), float, default 0, min 0, max 180, step 0.1;
//scale (rnd_scale), float, default 0, min 0, max 1, step 0.01 - The random scale applied to each image instance;
//value (rnd_value), float, default 0, min 0, max 1, step 0.01;
//----------------------;
//splatter_color.mmg;
//preads several occurrences of an input image randomly.;
//vec4 splatter_$(name)(vec2 uv, int count, vec2 seed) {\n\t;
//	vec4 c = vec4(0.0);\n\t;
//;
//	for (int i = 0; i < count; ++i) {\n\t\t;
//		seed = rand2(seed);\n\t\t;
//		float mask = $mask(fract(seed+vec2(0.5)));\n\t\t;
//;
//		if (mask > 0.01) {\n\t\t\t;
//			vec2 pv = fract(uv - seed)-vec2(0.5);\n\t\t\t;
//			seed = rand2(seed);\n\t\t\t;
//			float angle = (seed.x * 2.0 - 1.0) * $rotate * 0.01745329251;\n\t\t\t;
//			float ca = cos(angle);\n\t\t\t;
//			float sa = sin(angle);\n\t\t\t;
//			pv = vec2(ca*pv.x+sa*pv.y, -sa*pv.x+ca*pv.y);\n\t\t\t;
//			pv *= (seed.y-0.5)*2.0*$scale+1.0;\n\t\t\t;
//			pv /= vec2($scale_x, $scale_y);\n\t\t\tpv += vec2(0.5);\n\t\t\t;
//			seed = rand2(seed);\n\t\t\t;
//;
//			if (pv != CLAMP(pv, vec2(0.0), vec2(1.0))) {\n\t\t\t\t;
//				continue;\n\t\t\t;
//			}\n\t\t\t;
//;
//			$select_inputs\n\t\t\t;
//;
//			vec4 n = $in.variation(pv, $variations ? seed.x : 0.0);\n\t\t\t;
//;
//			float na = n.a*mask*(1.0-$opacity*seed.x);\n\t\t\t;
//			float a = (1.0-c.a)*(1.0*na);\n\t\t\t;
//			c = mix(c, n, na);\n\t\t;
//		}\n\t;
//	}\n\t;
//;
//	return c;\n;
//};
//Inputs:;
//in, rgba, default: 0, - The input image or atlas of 4 or 16 input images;
//Mask, float, default: 1, - The mask applied to the pattern;
//Outputs:;
//Output, rgba, Shows the generated pattern;
//splatter_$(name)($uv, int($count), vec2(float($seed)));
//select_inputs enum;
//1,  " ";
//4, "pv = CLAMP(0.5*(pv+floor(rand2(seed)*2.0)), vec2(0.0), vec2(1.0));";
//16, "pv = CLAMP(0.25*(pv+floor(rand2(seed)*4.0)), vec2(0.0), vec2(1.0));";
//Parameters:;
//count, int, default 25, min 1, max 100, - The number of occurrences of the input image;
//select_inputs (Inputs), enum, default 0, values 1, 4, 16;
//tile, Vector2, default 4, min:1, max:64, step:1 - The number of columns of the tiles pattern;
//overlap, float, default 1, min 0, max 5, step 1 - The number of neighbour tiles an instance of the input image can overlap. Set this parameter to the lowest value that generates the expected result (where all instances are fully visible) to improve performance.;
//scale, Vector2, default 1, min:0, max:2, step:0.01 - "The scale of input images on the X axis;
//rotate (rnd_rotate), float, default 0, min 0, max 180, step 0.1;
//scale (rnd_scale), float, default 0, min 0, max 1, step 0.01 - The random scale applied to each image instance;
//value (rnd_value), float, default 0, min 0, max 1, step 0.01;
//variations bool;
//----------------------;
//circle_splatter.mmg;
//Spreads several occurrences of an input image in a circle or spiral pattern.;
//vec4 $(name_uv)_rch = splatter_$(name)($uv, int($count), int($rings), vec2(float($seed)));
//vec4 splatter_$(name)(vec2 uv, int count, int rings, vec2 seed) {\n\t;
//	float c = 0.0;\n\t;
//	vec3 rc = vec3(0.0);\n\t;
//	vec3 rc1;\n\t;
//	seed = rand2(seed);\n\t;
//;
//	for (int i = 0; i < count; ++i) {\n\t\t;
//		float a = -1.57079632679+6.28318530718*float(i)*$rings/float(count);\n\t\t;
//		float rings_distance = ceil(float(i+1)*float(rings)/float(count))/float(rings);\n\t\t;
//		float spiral_distance = float(i+1)/float(count);\n\t\t;
//		vec2 pos = $radius*mix(rings_distance, spiral_distance, $spiral)*vec2(cos(a), sin(a));\n\t\t;
//		float mask = $mask(fract(pos-vec2(0.5)));\n\t\t;
//;
//		if (mask > 0.01) {\n\t\t\t;
//			vec2 pv = uv-0.5-pos;\n\t\t\t;
//			rc1 = rand3(seed);\n\t\t\tseed = rand2(seed);\n\t\t\t;
//			float angle = (seed.x * 2.0 - 1.0) * $rotate * 0.01745329251 + (a+1.57079632679) * $i_rotate;\n\t\t\t;
//			float ca = cos(angle);\n\t\t\t;
//			float sa = sin(angle);\n\t\t\t;
//			pv = vec2(ca*pv.x+sa*pv.y, -sa*pv.x+ca*pv.y);\n\t\t\t;
//			pv /= mix(1.0, float(i+1)/float(count+1), $i_scale);\n\t\t\t;
//			pv /= vec2($scale_x, $scale_y);\n\t\t\t;
//			pv *= (seed.y-0.5)*2.0*$scale+1.0;\n\t\t\t;
//			pv += vec2(0.5);\n\t\t\tseed = rand2(seed);\n\t\t\t;
//;
//			if (pv != CLAMP(pv, vec2(0.0), vec2(1.0))) {\n\t\t\t\t;
//				continue;\n\t\t\t;
//			}\n\t\t\t;
//;
//			$select_inputs\n\t\t\t;
//;
//			float c1 = $in(pv)*mask*(1.0-$value*seed.x);\n\t\t\t;
//;
//			c = MAX(c, c1);\n\t\t\trc = mix(rc, rc1, step(c, c1));\n\t\t;
//		}\n\t;
//	}\n\t;
//;
//	return vec4(rc, c);\n;
//};
//Inputs:;
//in, float, default: 0, - The input image or atlas of 4 or 16 input images;
//Mask, float, default: 1, - The mask applied to the pattern;
//Outputs:;
//Output, float, Shows the generated pattern;
//$(name_uv)_rch.rgb;
//select_inputs enum;
//1,  " ";
//4, "pv = CLAMP(0.5*(pv+floor(rand2(seed)*2.0)), vec2(0.0), vec2(1.0));";
//16, "pv = CLAMP(0.25*(pv+floor(rand2(seed)*4.0)), vec2(0.0), vec2(1.0));";
//Parameters:;
//count, int, default 10, min 1, max 256, - The number of occurrences of the input image;
//rings, int, default 1, min 1, max 16, - The number of occurrences of the input image;
//select_inputs (Inputs), enum, default 0, values 1, 4, 16;
//scale, Vector2, default 1, min:0, max:2, step:0.01 - "The scale of input images on the X axis;
//radius, float, default 0.4, min 0, max 0.5, step 0.01;
//spiral, float, default 0, min 0, max 1, step 0.01;
//i_rotate (inc_rotate), float, default 0, min 0, max 180, step 0.1;
//i_scale (inc_scale), float, default 0, min 0, max 1, step 0.01;
//rotate (rnd_rotate), float, default 0, min 0, max 180, step 0.1;
//scale (rnd_scale), float, default 0, min 0, max 1, step 0.01 - The random scale applied to each image instance;
//value (rnd_value), float, default 0, min 0, max 1, step 0.01;
//----------------------;
//warp_dilation.mmg;
//----------------------;
//warp_dilation_nobuf.mmg;
//----------------------;
//remap.mmg;
//----------------------;
//height_to_angle.mmg;

//vec2 transform(vec2 uv, vec2 translate, float rotate, vec2 scale, bool repeat) {\n \t;
//	vec2 rv;\n\t;
//	uv -= translate;\n\t;
//	uv -= vec2(0.5);\n\t;
//	rv.x = cos(rotate)*uv.x + sin(rotate)*uv.y;\n\t;
//	rv.y = -sin(rotate)*uv.x + cos(rotate)*uv.y;\n\t;
//	rv /= scale;\n\t;
//	rv += vec2(0.5);\n;
//;
//	if (repeat) {\n\t\t;
//		return fract(rv);\n\t;
//	} else {\n\t\t;
//		return CLAMP(rv, vec2(0.0), vec2(1.0));\n\t;
//	}\t\n;
//};

//static func transform(uv : Vector2, translate : Vector2, rotate : float, scale : Vector2, repeat : bool) -> Vector2:;
//	var rv : Vector2 = Vector2();
//	uv -= translate;
//	uv -= Vector2(0.5, 0.5);
//	rv.x = cos(rotate)*uv.x + sin(rotate)*uv.y;
//	rv.y = -sin(rotate)*uv.x + cos(rotate)*uv.y;
//	rv /= scale;
//	rv += Vector2(0.5, 0.5);
//;
//	if (repeat):;
//		return fractv2(rv);
//	else:;
//		return clampv2(rv, Vector2(0, 0), Vector2(1, 1));
//vec2 transform2_clamp(vec2 uv) {\n\t;
//	return CLAMP(uv, vec2(0.0), vec2(1.0));\n;
//};

Vector2 MMAlgos::transform2_clamp(const Vector2 &uv) {
	return clampv2(uv, Vector2(0, 0), Vector2(1, 1));
}

//vec2 transform2(vec2 uv, vec2 translate, float rotate, vec2 scale) {\n \t;
//	vec2 rv;\n\t;
//	uv -= translate;\n\t;
//	uv -= vec2(0.5);\n\t;
//	rv.x = cos(rotate)*uv.x + sin(rotate)*uv.y;\n\t;
//	rv.y = -sin(rotate)*uv.x + cos(rotate)*uv.y;\n\t;
//	rv /= scale;\n\t;
//	rv += vec2(0.5);\n\t;
//	return rv;\t\n;
//};

Vector2 MMAlgos::transform2(const Vector2 &uuv, const Vector2 &translate, const float rotate, const Vector2 &scale) {
	Vector2 uv = uuv;
	Vector2 rv = Vector2();
	uv -= translate;
	uv -= Vector2(0.5, 0.5);
	rv.x = cos(rotate) * uv.x + sin(rotate) * uv.y;
	rv.y = -sin(rotate) * uv.x + cos(rotate) * uv.y;
	rv /= scale;
	rv += Vector2(0.5, 0.5);
	return rv;
}

//vec2 rotate(vec2 uv, vec2 center, float rotate) {\n \t;
//	vec2 rv;\n\t;
//	uv -= center;\n\t;
//	rv.x = cos(rotate)*uv.x + sin(rotate)*uv.y;\n\t;
//	rv.y = -sin(rotate)*uv.x + cos(rotate)*uv.y;\n\t;
//	rv += center;\n;
//	return rv;\t\n;
//};

Vector2 MMAlgos::rotate(const Vector2 &uuv, const Vector2 &center, const float rotate) {
	Vector2 uv = uuv;
	Vector2 rv = Vector2();
	uv -= center;
	rv.x = cos(rotate) * uv.x + sin(rotate) * uv.y;
	rv.y = -sin(rotate) * uv.x + cos(rotate) * uv.y;
	rv += center;
	return rv;
}

//vec2 scale(vec2 uv, vec2 center, vec2 scale) {\n\t;
//	uv -= center;\n\t;
//	uv /= scale;\n\t;
//	uv += center;\n;
//	return uv;\n;
//};

Vector2 MMAlgos::scale(const Vector2 &uuv, const Vector2 &center, const Vector2 &scale) {
	Vector2 uv = uuv;
	uv -= center;
	uv /= scale;
	uv += center;
	return uv;
}

//vec2 uvmirror_h(vec2 uv, float offset) {\n\t;
//	return vec2(MAX(0, abs(uv.x-0.5)-0.5*offset)+0.5, uv.y);
//};

Vector2 MMAlgos::uvmirror_h(const Vector2 &uv, const float offset) {
	return Vector2(MAX(0, abs(uv.x - 0.5) - 0.5 * offset) + 0.5, uv.y);
}

//vec2 uvmirror_v(vec2 uv, float offset) {\n\t;
//	return vec2(uv.x, MAX(0, abs(uv.y-0.5)-0.5*offset)+0.5);\n;
//};

Vector2 MMAlgos::uvmirror_v(const Vector2 &uv, const float offset) {
	return Vector2(uv.x, MAX(0, abs(uv.y - 0.5) - 0.5 * offset) + 0.5);
}

//vec2 kal_rotate(vec2 uv, float count, float offset) {\n\t;
//	float pi = 3.14159265359;\n\t;
//	offset *= pi/180.0;\n\t;
//	offset += pi*(1.0/count+0.5);\n\t;
//	uv -= vec2(0.5);\n\t;
//;
//	float l = length(uv);\n\t;
//	float a = mod(atan(uv.y, uv.x)+offset, 2.0*pi/count)-offset;\n\t;
//;
//	return vec2(0.5)+l*vec2(cos(a), sin(a));\n;
//};

Vector2 MMAlgos::kal_rotate(const Vector2 &uuv, const float count, const float p_offset) {
	float offset = p_offset;

	Vector2 uv = uuv;
	float pi = 3.14159265359;
	offset *= pi / 180.0;
	offset += pi * (1.0 / count + 0.5);
	uv -= Vector2(0.5, 0.5);
	float l = uv.length();
	float a = modf(atan2(uv.y, uv.x) + offset, 2.0 * pi / count) - offset;
	return Vector2(0.5, 0.5) + l * Vector2(cos(a), sin(a));
}

//vec2 get_from_tileset(float count, float seed, vec2 uv) {\n\t;
//	return CLAMP((uv+floor(rand2(vec2(seed))*count))/count, vec2(0.0), vec2(1.0));\n;
//};

Vector2 MMAlgos::get_from_tileset(const float count, const float pseed, const Vector2 &uv) {
	return clampv2((uv + floorv2(rand2(Vector2(pseed, pseed)) * count)) / count, Vector2(0, 0), Vector2(1, 1));
}

//vec2 custom_uv_transform(vec2 uv, vec2 cst_scale, float rnd_rotate, float rnd_scale, vec2 seed) {\n\t;
//	seed = rand2(seed);\n\t;
//	uv -= vec2(0.5);\n\t;
//	float angle = (seed.x * 2.0 - 1.0) * rnd_rotate;\n\t;
//	float ca = cos(angle);\n\t;
//	float sa = sin(angle);\n\t;
//	uv = vec2(ca*uv.x+sa*uv.y, -sa*uv.x+ca*uv.y);\n\t;
//	uv *= (seed.y-0.5)*2.0*rnd_scale+1.0;\n\t;
//	uv /= cst_scale;\n\t;
//	uv += vec2(0.5);\n\t;
//;
//	return uv;\n;
//};

Vector2 MMAlgos::custom_uv_transform(const Vector2 &uuv, const Vector2 &cst_scale, const float rnd_rotate, const float rnd_scale, const Vector2 &p_pseed) {
	Vector2 uv = uuv;
	Vector2 pseed = p_pseed;
	pseed = rand2(pseed);
	uv -= Vector2(0.5, 0.5);
	float angle = (pseed.x * 2.0 - 1.0) * rnd_rotate;
	float ca = cos(angle);
	float sa = sin(angle);
	uv = Vector2(ca * uv.x + sa * uv.y, -sa * uv.x + ca * uv.y);
	uv *= (pseed.y - 0.5) * 2.0 * rnd_scale + 1.0;
	uv /= cst_scale;
	uv += Vector2(0.5, 0.5);
	return uv;
}

Ref<Image> MMAlgos::generate_histogram(const Ref<Image> &input, const int texture_size) {
	ERR_FAIL_COND_V(!input.is_valid(), Ref<Image>());

	Ref<Image> scaled_input;
	scaled_input.instance();
	scaled_input->copy_internals_from(input);
	scaled_input->resize(texture_size, texture_size);

	Ref<Image> step_1;
	step_1.instance();
	step_1->copy_internals_from(scaled_input);

	scaled_input->lock();
	step_1->lock();

	// texture_size, texture_size
	//step 1
	/*
	shader_type canvas_item;
	render_mode blend_disabled;

	uniform sampler2D tex;
	uniform float size = 256;

	void fragment() {
		float e = 1.0 / size;
		vec4 sum = vec4(0.0);
		for (float y = 0.5 * e; y < 1.0; y += e) {
			sum += max(vec4(0.0), vec4(1.0) - 16.0 * abs(texture(tex, vec2(UV.x, y)) - UV.y));
		}
		COLOR = sum / size;
	}
	*/

	Vector2i size = step_1->get_size();

	for (int x = 0; x < size.x; ++x) {
		for (int y = 0; y < size.y; ++y) {
			float e = 1.0 / size.y;

			Color sum = Color();

			for (float yy = 0.5 * e; yy < 1.0; yy += e) {
				Color sic = scaled_input->get_pixel(x, yy * size.y);
				sic -= Color(e, e, e, e);

				sic.r = MAX(0, 1.0 - 16.0 * ABS(sic.r));
				sic.g = MAX(0, 1.0 - 16.0 * ABS(sic.g));
				sic.b = MAX(0, 1.0 - 16.0 * ABS(sic.b));
				sic.a = MAX(0, 1.0 - 16.0 * ABS(sic.a));

				sum += sic;
			}

			sum.r /= size.y;
			sum.g /= size.y;
			sum.b /= size.y;
			sum.a /= size.y;

			step_1->set_pixel(x, y, sum);
		}
	}

	step_1->unlock();
	scaled_input->unlock();

	//texture_size, 2
	//step 2
	/*
	shader_type canvas_item;
	render_mode blend_disabled;

	uniform sampler2D tex;
	uniform float size = 256;

	void fragment() {
		float e = 1.0 / size;
		vec4 sum = vec4(0.0);
		for (float y = 0.5 * e; y < 1.0; y += e) {
			sum += texture(tex, vec2(y, UV.x));
		}
		COLOR = sum / size;
	}
	*/

	Ref<Image> step_2;
	step_2.instance();
	step_2->copy_internals_from(step_1);
	step_2->resize(texture_size, 2);

	scaled_input->lock();
	step_2->lock();

	size = step_2->get_size();

	for (int x = 0; x < size.x; ++x) {
		for (int y = 0; y < size.y; ++y) {
			float e = 1.0 / size.y;
			Color sum = Color();

			for (float yy = 0.5 * e; yy < 1.0; yy += e) {
				Color sic = scaled_input->get_pixel(yy * size.y, x);

				sum += sic;
			}

			sum.r /= size.y;
			sum.g /= size.y;
			sum.b /= size.y;
			sum.a /= size.y;

			step_2->set_pixel(x, y, sum);
		}
	}

	step_2->unlock();
	scaled_input->unlock();

	//step 3

	//texture_size, texture_size

	/*
	shader_type canvas_item;
	render_mode blend_disabled;

	uniform sampler2D tex;
	uniform float gradient_width = 0.1;

	void fragment() {
		if (abs(fract(UV.y + gradient_width)) < 2.0 * gradient_width) {
			COLOR = vec4(vec3(UV.x), 1.0);
		} else {
			float e = 1.0 / 256.0;
			vec4 highest = vec4(0.0);
			for (float x = 0.5 * e; x < 1.0; x += e) {
				highest = max(highest, texture(tex, vec2(x, 0.0)));
			}
			vec4 raw_value = texture(tex, vec2(UV.x, 0.0));
			vec4 value = step(vec4(1.0 - gradient_width - UV.y) * highest / (1.0 - 2.0 * gradient_width), raw_value);
			float alpha = step(2.0 * gradient_width, dot(value, vec4(1.0)));
			COLOR = vec4(mix(value.rgb, vec3(0.5), 0.3 * value.a), alpha);
		}
	}
	*/

	Ref<Image> result;
	result.instance();
	result->copy_internals_from(input);
	result->resize(texture_size, texture_size);

	result->lock();
	step_2->lock();

	size = result->get_size();

	float gradient_width = 0.1;

	for (int x = 0; x < size.x; ++x) {
		float uv_1_x = (1.0 / size.x);
		float uv_x = uv_1_x * x;

		for (int y = 0; y < size.y; ++y) {
			float e = 1.0 / size.y;
			float uv_y = e * y;

			if (ABS(MMAlgos::fractf(uv_y + gradient_width)) < 2.0 * gradient_width) {
				result->set_pixel(x, y, Color(uv_x, uv_x, uv_x, 1));
			} else {
				Color highest = Color(0, 0, 0, 0);

				for (float xx = 0.5 * uv_1_x; xx < 1.0; xx += uv_1_x) {
					Color sic = step_2->get_pixel(xx * uv_1_x, 0);

					highest.r = MAX(highest.r, sic.r);
					highest.g = MAX(highest.g, sic.g);
					highest.b = MAX(highest.b, sic.b);
					highest.a = MAX(highest.a, sic.a);
				}

				Color raw_value = step_2->get_pixel(x * uv_1_x, 0);

				Vector4 highest_v4 = Vector4(highest.r, highest.g, highest.b, highest.a);
				float gv = 1.0 - gradient_width - uv_y;
				Vector4 value = Vector4(gv, gv, gv, gv) * highest_v4 / (1.0 - 2.0 * gradient_width);

				value.x = step(value.x, raw_value.r);
				value.y = step(value.y, raw_value.g);
				value.z = step(value.z, raw_value.b);
				value.w = step(value.w, raw_value.a);

				float alpha = step(2.0 * gradient_width, value.dot(Vector4(1, 1, 1, 1)));

				Vector3 val3 = Vector3(value.x, value.y, value.z);
				val3 = val3.linear_interpolate(Vector3(0.5, 0.5, 0.5), 0.3 * value.w);

				Color f = Color(val3.x, val3.y, val3.z, alpha);

				result->set_pixel(x, y, f);
			}
		}
	}

	step_2->unlock();
	result->unlock();

	return result;
}

void MMAlgos::register_node_class(const String &category, const String &cls) {
	for (int i = 0; i < mm_node_registry.size(); ++i) {
		const MMNodeRegistryCategory &categ = mm_node_registry[i];

		if (categ.category_name == category) {
			for (int j = 0; j < categ.entries.size(); ++j) {
				ERR_FAIL_COND(categ.entries[j].data == cls);
			}

			MMNodeRegistryEntry e;
			e.type = MMNODE_REGISTRY_TYPE_CLASS;
			e.data = cls;

			mm_node_registry.write[i].entries.push_back(e);

			return;
		}
	}

	MMNodeRegistryCategory categ;
	categ.category_name = category;

	MMNodeRegistryEntry e;
	e.type = MMNODE_REGISTRY_TYPE_CLASS;
	e.data = cls;

	categ.entries.push_back(e);

	mm_node_registry.push_back(categ);
}
void MMAlgos::unregister_node_class(const String &category, const String &cls) {
	for (int i = 0; i < mm_node_registry.size(); ++i) {
		const MMNodeRegistryCategory &categ = mm_node_registry[i];

		if (categ.category_name == category) {
			for (int j = 0; j < categ.entries.size(); ++j) {
				if (categ.entries[j].data == cls) {
					mm_node_registry.write[j].entries.remove(j);

					if (categ.entries.size() == 0) {
						mm_node_registry.remove(i);
					}

					return;
				}
			}
		}
	}
}

void MMAlgos::register_node_script(const String &category, const String &file_path) {
	for (int i = 0; i < mm_node_registry.size(); ++i) {
		const MMNodeRegistryCategory &categ = mm_node_registry[i];

		if (categ.category_name == category) {
			for (int j = 0; j < categ.entries.size(); ++j) {
				ERR_FAIL_COND(categ.entries[j].data == file_path);
			}

			MMNodeRegistryEntry e;
			e.type = MMNODE_REGISTRY_TYPE_SCRIPT;
			e.data = file_path;

			mm_node_registry.write[i].entries.push_back(e);

			return;
		}
	}

	MMNodeRegistryCategory categ;
	categ.category_name = category;

	MMNodeRegistryEntry e;
	e.type = MMNODE_REGISTRY_TYPE_SCRIPT;
	e.data = file_path;

	categ.entries.push_back(e);

	mm_node_registry.push_back(categ);
}
void MMAlgos::unregister_node_script(const String &category, const String &file_path) {
	for (int i = 0; i < mm_node_registry.size(); ++i) {
		const MMNodeRegistryCategory &categ = mm_node_registry[i];

		if (categ.category_name == category) {
			for (int j = 0; j < categ.entries.size(); ++j) {
				if (categ.entries[j].data == file_path) {
					mm_node_registry.write[j].entries.remove(j);

					if (categ.entries.size() == 0) {
						mm_node_registry.remove(i);
					}

					return;
				}
			}
		}
	}
}

MMAlgos::MMAlgos() {
}

MMAlgos::~MMAlgos() {
}

void MMAlgos::_bind_methods() {
}

Vector<MMAlgos::MMNodeRegistryCategory> MMAlgos::mm_node_registry;
