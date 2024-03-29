#ifndef SEARCH_ARRAY_H
#define SEARCH_ARRAY_H

/*************************************************************************/
/*  search_array.h                                                       */
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

#include "sort_array.h"

template <class T, class Comparator = _DefaultComparator<T>>
class SearchArray {
public:
	Comparator compare;

	inline int bisect(const T *p_array, int p_len, const T &p_value, bool p_before) const {
		int lo = 0;
		int hi = p_len;
		if (p_before) {
			while (lo < hi) {
				const int mid = (lo + hi) / 2;
				if (compare(p_array[mid], p_value)) {
					lo = mid + 1;
				} else {
					hi = mid;
				}
			}
		} else {
			while (lo < hi) {
				const int mid = (lo + hi) / 2;
				if (compare(p_value, p_array[mid])) {
					hi = mid;
				} else {
					lo = mid + 1;
				}
			}
		}
		return lo;
	}
};

#endif // SEARCH_ARRAY_H
