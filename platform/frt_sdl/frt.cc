/*************************************************************************/
/*  frt.cc                                                               */
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

// frt.cc
/*
  FRT - A Godot platform targeting single board computers
  Copyright (c) 2017-2022  Emanuele Fornara
  SPDX-License-Identifier: MIT
 */

#include "frt.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

static void print_msg(const char *format, va_list ap) {
	fprintf(stderr, "frt: ");
	vfprintf(stderr, format, ap);
	fprintf(stderr, "\n");
}

namespace frt {

void warn(const char *format, ...) {
	va_list ap;
	va_start(ap, format);
	print_msg(format, ap);
	va_end(ap);
}

void fatal(const char *format, ...) {
	va_list ap;
	va_start(ap, format);
	print_msg(format, ap);
	va_end(ap);
	exit(1);
}

} // namespace frt

#include "frt_lib.h"

extern "C" void frt_parse_frt_args(int argc, char *argv[]) {
}
