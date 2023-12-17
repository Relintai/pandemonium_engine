/*************************************************************************/
/*  lz4_compressor.cpp                                                   */
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

#include "lz4_compressor.h"

#include "./thirdparty/lz4/lz4.h"

PoolByteArray LZ4Compressor::compress_data(const PoolByteArray &data) const {
	PoolByteArray arr;

	int size = data.size();
	PoolByteArray::Read dr = data.read();
	const uint8_t *ch = dr.ptr();

	int bound = LZ4_compressBound(size);
	arr.resize(bound + sizeof(int));

	PoolByteArray::Write w = arr.write();
	uint8_t *wptr = w.ptr();
	reinterpret_cast<int *>(wptr)[0] = data.size();

	int ns = LZ4_compress_default(reinterpret_cast<const char *>(ch), reinterpret_cast<char *>(&wptr[sizeof(int)]), size, bound);

	w.release();

	arr.resize(ns + sizeof(int));

	return arr;
}
PoolByteArray LZ4Compressor::decompress_data(const PoolByteArray &data) const {
	PoolByteArray arr;

	int ds = data.size();

	if (ds == 0) {
		return arr;
	}

	PoolByteArray::Read dr = data.read();
	const uint8_t *ch = dr.ptr();
	int size = reinterpret_cast<const int *>(ch)[0];

	arr.resize(size);
	PoolByteArray::Write w = arr.write();
	uint8_t *wptr = w.ptr();

	int ns = LZ4_decompress_safe(reinterpret_cast<const char *>(&ch[sizeof(int)]), reinterpret_cast<char *>(wptr), ds - sizeof(int), size);

	if (ns != size) {
		arr.resize(ns);
		ERR_PRINT("ns != size");
	}

	return arr;
}

int LZ4Compressor::LZ4_compressBound(int inputSize) {
	return ::LZ4_compressBound(inputSize);
}
int LZ4Compressor::LZ4_compress_default(const char *src, char *dst, int srcSize, int dstCapacity) {
	return ::LZ4_compress_default(src, dst, srcSize, dstCapacity);
}
int LZ4Compressor::LZ4_decompress_safe(const char *src, char *dst, int compressedSize, int dstCapacity) {
	return ::LZ4_decompress_safe(src, dst, compressedSize, dstCapacity);
}

LZ4Compressor::LZ4Compressor() {
}

LZ4Compressor::~LZ4Compressor() {
}

void LZ4Compressor::_bind_methods() {
	ClassDB::bind_method(D_METHOD("compress_data", "data"), &LZ4Compressor::compress_data);
	ClassDB::bind_method(D_METHOD("decompress_data", "data"), &LZ4Compressor::decompress_data);
}
