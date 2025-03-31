#ifndef STRING_NAME_H
#define STRING_NAME_H

/*************************************************************************/
/*  string_name.h                                                        */
/*************************************************************************/
/*                         This file is part of:                         */
/*                          PANDEMONIUM ENGINE                           */
/*             https://github.com/Relintai/pandemonium_engine            */
/*************************************************************************/
/* Copyright (c) 2022-present Péter Magyar.                              */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "core/os/mutex.h"
#include "core/os/safe_refcount.h"
#include "core/string/ustring.h"

struct StaticCString {
	const char *ptr;
	static StaticCString create(const char *p_ptr);
};

class StringName {
	enum {
		STRING_TABLE_BITS = 14,
		STRING_TABLE_LEN = 1 << STRING_TABLE_BITS,
		STRING_TABLE_MASK = STRING_TABLE_LEN - 1
	};

	struct _Data {
		SafeRefCount refcount;
		SafeNumeric<uint32_t> static_count;
		const char *cname;
		String name;

#ifdef DEBUG_ENABLED
		uint32_t debug_references;
#endif

		String get_name() const {
			return cname ? String(cname) : name;
		}

		int idx;
		uint32_t hash;
		_Data *prev;
		_Data *next;

		_Data() {
#ifdef DEBUG_ENABLED
			debug_references = 0;
#endif
			cname = nullptr;
			prev = nullptr;
			next = nullptr;
			idx = 0;
			hash = 0;
		}
	};

	static _Data *_table[STRING_TABLE_LEN];

	_Data *_data;

	union _HashUnion {
		_Data *ptr;
		uint32_t hash;

		_HashUnion() {
			ptr = nullptr;
		}
	};

	void unref();
	friend void register_core_types();
	friend void unregister_core_types();

	static Mutex lock;
	static void setup();
	static void cleanup();
	static bool configured;
#ifdef DEBUG_ENABLED
	struct DebugSortReferences {
		bool operator()(const _Data *p_left, const _Data *p_right) const {
			return p_left->debug_references > p_right->debug_references;
		}
	};

	static bool debug_stringname;
#endif

	StringName(_Data *p_data) {
		_data = p_data;
	}

public:
	operator const void *() const {
		return (_data && (_data->cname || !_data->name.empty())) ? (void *)1 : nullptr;
	}

	bool operator==(const String &p_name) const;
	bool operator==(const char *p_name) const;
	bool operator!=(const String &p_name) const;

	_FORCE_INLINE_ bool is_node_unique_name() const {
		if (!_data) {
			return false;
		}

		if (_data->cname != nullptr) {
			return _data->cname[0] == '%';
		} else {
			return _data->name[0] == '%';
		}
	}

	_FORCE_INLINE_ bool operator<(const StringName &p_name) const {
		return _data < p_name._data;
	}
	_FORCE_INLINE_ bool operator==(const StringName &p_name) const {
		// the real magic of all this mess happens here.
		// this is why path comparisons are very fast
		return _data == p_name._data;
	}
	_FORCE_INLINE_ uint32_t hash() const {
		if (_data) {
			return _data->hash;
		} else {
			return 0;
		}
	}
	_FORCE_INLINE_ const void *data_unique_pointer() const {
		return (void *)_data;
	}
	bool operator!=(const StringName &p_name) const;

	_FORCE_INLINE_ operator String() const {
		if (_data) {
			if (_data->cname) {
				return String(_data->cname);
			} else {
				return _data->name;
			}
		}

		return String();
	}

	static StringName search(const char *p_name);
	static StringName search(const CharType *p_name);
	static StringName search(const String &p_name);

	struct AlphCompare {
		_FORCE_INLINE_ bool operator()(const StringName &l, const StringName &r) const {
			const char *l_cname = l._data ? l._data->cname : "";
			const char *r_cname = r._data ? r._data->cname : "";

			if (l_cname) {
				if (r_cname) {
					return is_str_less(l_cname, r_cname);
				} else {
					return is_str_less(l_cname, r._data->name.ptr());
				}
			} else {
				if (r_cname) {
					return is_str_less(l._data->name.ptr(), r_cname);
				} else {
					return is_str_less(l._data->name.ptr(), r._data->name.ptr());
				}
			}
		}
	};

	void operator=(const StringName &p_name);

	StringName &operator=(StringName &&p_name) {
		if (_data == p_name._data) {
			return *this;
		}

		unref();

		_data = p_name._data;
		p_name._data = nullptr;

		return *this;
	}

	StringName(StringName &&p_name) {
		_data = p_name._data;
		p_name._data = nullptr;
	}

	StringName(const char *p_name, bool p_static = false);
	StringName(const StringName &p_name);
	StringName(const String &p_name, bool p_static = false);
	StringName(const StaticCString &p_static_string, bool p_static = false);
	StringName();
	_FORCE_INLINE_ ~StringName() {
		if (likely(configured) && _data) { //only free if configured
			unref();
		}
	}
};

/*
bool operator==(const String &p_name, const StringName &p_string_name);
bool operator!=(const String &p_name, const StringName &p_string_name);
bool operator==(const char *p_name, const StringName &p_string_name);
bool operator!=(const char *p_name, const StringName &p_string_name);
*/

StringName _scs_create(const char *p_chr, bool p_static = false);

/*
 * The SNAME macro is used to speed up StringName creation, as it allows caching it after the first usage in a very efficient way.
 * It should NOT be used everywhere, but instead in places where high performance is required and the creation of a StringName
 * can be costly. Places where it should be used are:
 * - Control::get_theme_*(<name> and Window::get_theme_*(<name> functions.
 * - emit_signal(<name>,..) function
 * - call_deferred(<name>,..) function
 * - Comparisons to a StringName in overridden _set and _get methods.
 *
 * Use in places that can be called hundreds of times per frame (or more) is recommended, but this situation is very rare. If in doubt, do not use.
 */

//#define SNAME(m_arg) ([]() -> const StringName & { static StringName sname = _scs_create(m_arg, true); return sname; })()

#endif // STRING_NAME_H
