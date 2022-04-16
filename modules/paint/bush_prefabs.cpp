/*
Copyright (c) 2019 Flairieve
Copyright (c) 2020-2022 cobrapitz
Copyright (c) 2022 Péter Magyar

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "bush_prefabs.h"

/*
const list = [
	[   			Vector2(0, -1),
	Vector2(-1, 0), Vector2(0, 0), Vector2(1, 0),
					Vector2(0, 1)
	],
	[Vector2(-1, -1), Vector2(0, -1), Vector2(1, -1),
	Vector2(-1, 0), Vector2(0, 0), Vector2(1, 0),
	Vector2(-1, 1), Vector2(0, 1), Vector2(1, 1),
	],
	[
	Vector2(-1, 0), Vector2(0, 0), Vector2(1, 0),
	],
	[   			Vector2(0, -1),
					Vector2(0, 0),
					Vector2(0, 1)
	]
]
*/

PoolVector3iArray BrushPrefabs::get_brush(const BrushPrefabs::Type type, const int size) {
	/*
		var pixels = []
		if size < 1:
			size = 1

		match type:
			Type.CIRCLE:
				size += 1
				var center = Vector2.ZERO
				var last = center
				var radius = size / 2.0
				for x in range(size):
					for y in range(size):
						if Vector2(x - radius, y - radius).length() < size / 3.0:
							pixels.append(Vector2(x, y))

				var avg = Vector2(size / 2, size / 2)
				avg = Vector2(floor(avg.x), floor(avg.y))

				for i in range(pixels.size()):
					pixels[i] -= avg

			Type.RECT:
				var center = Vector2.ZERO
				var last = center
				for x in range(size):
					for y in range(size):
						pixels.append(Vector2(x, y))

				var avg = Vector2.ZERO
				for cell in pixels:
					avg += cell

				avg.x /= pixels.size()
				avg.y /= pixels.size()

				avg = Vector2(floor(avg.x), floor(avg.y))

				for i in range(pixels.size()):
					pixels[i] -= avg

			Type.V_LINE:
				var center = Vector2.ZERO
				var last = center
				pixels.append(Vector2.ZERO)

				for i in range(size - 1):
					var sig = sign(last.y)
					if sig == 0:
						sig = 1

					if last.y < 0:
						center.y = abs(last.y) * -sig
					else:
						center.y = abs(last.y+1) * -sig
					last = center
					pixels.append(center)
			Type.H_LINE:
				var center = Vector2.ZERO
				var last = center
				pixels.append(Vector2.ZERO)

				for i in range(size - 1):
					var sig = sign(last.x)
					if sig == 0:
						sig = 1

					if last.x < 0:
						center.x = abs(last.x) * -sig
					else:
						center.x = abs(last.x+1) * -sig
					last = center
					pixels.append(center)

		return pixels
	*/

	return PoolVector3iArray();
}

BrushPrefabs::BrushPrefabs() {
}

BrushPrefabs::~BrushPrefabs() {
}

void BrushPrefabs::_bind_methods() {
}
