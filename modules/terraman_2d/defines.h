
#ifndef TERRAIN_2D_DEFINES_H
#define TERRAIN_2D_DEFINES_H

#define VARIANT_ARRAY_GET(arr)             \
	Vector<Variant> r;                     \
	for (int i = 0; i < arr.size(); i++) { \
		r.push_back(arr[i].get_ref_ptr()); \
	}                                      \
	return r;

#define VARIANT_ARRAY_SET(arr, arr_into, type) \
	arr_into.clear();                          \
	for (int i = 0; i < arr.size(); i++) {     \
		Ref<type> e = Ref<type>(arr[i]);       \
		arr_into.push_back(e);                 \
	}

#define CALL(func, ...) \
	call(#func, ##__VA_ARGS__);

#define RETURN_CALL(ret_type, func) \
	return call(#func);

#define RETURN_CALLP(ret_type, func, ...) \
	return call(#func, ##__VA_ARGS__);

#define GET_CALL(ret_type, ret_var, func) \
	ret_var = call(#func);

#define GET_CALLP(ret_type, ret_var, func, ...) \
	ret_var = call(#func, ##__VA_ARGS__);

#define RETURN_CALLD(ret_type, def_val, func) \
	return call(#func);

#define RETURN_CALLPD(ret_type, def_val, func, ...) \
	return call(#func, ##__VA_ARGS__);

#define GET_CALLD(ret_type, def_val, ret_var, func) \
	ret_var = call(#func);

#define GET_CALLPD(ret_type, def_val, ret_var, func, ...) \
	return call(#func, ##__VA_ARGS__);

#endif
