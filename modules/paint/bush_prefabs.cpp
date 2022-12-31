/*
Copyright (c) 2019 Flairieve
Copyright (c) 2020-2022 cobrapitz
Copyright (c) 2022-2023 Péter Magyar

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
#include "core/string/print_string.h"

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

PoolVector2iArray BrushPrefabs::get_brush(const BrushPrefabs::Type type, int size) {
	PoolVector2iArray pixels;

	if (size < 1) {
		size = 1;
	}

	switch (type) {
		case CIRCLE: {
			size += 1;
			Vector2 center;
			float radius = size / 2.0;

			for (int x = 0; x < size; ++x) {
				for (int y = 0; y < size; ++y) {
					if (Vector2(x - radius, y - radius).length() < size / 3.0) {
						pixels.append(Vector2i(x, y));
					}
				}
			}

			int av = static_cast<int>(size / 2);
			Vector2i avg = Vector2i(av, av);

			for (int i = 0; i < pixels.size(); ++i) {
				Vector2i p = pixels[i];
				p -= avg;
				pixels.set(i, p);
			}
		} break;
		case RECT: {
			for (int x = 0; x < size; ++x) {
				for (int y = 0; y < size; ++y) {
					pixels.append(Vector2(x, y));
				}
			}

			Vector2i avg;
			for (int i = 0; i < pixels.size(); ++i) {
				Vector2i p = pixels[i];
				avg += p;
			}

			avg.x /= pixels.size();
			avg.y /= pixels.size();

			for (int i = 0; i < pixels.size(); ++i) {
				Vector2i p = pixels[i];
				p -= avg;
				pixels.set(i, p);
			}
		} break;
		case V_LINE: {
			Vector2i center;
			Vector2i last = center;
			pixels.append(Vector2i());

			for (int i = 0; i < size - 1; ++i) {
				int sig = SGN(last.y);

				if (sig == 0) {
					sig = 1;
				}

				if (last.y < 0) {
					center.y = abs(last.y) * -sig;
				} else {
					center.y = abs(last.y + 1) * -sig;
				}

				last = center;
				pixels.append(center);
			}

		} break;
		case H_LINE: {
			Vector2i center;
			Vector2i last = center;
			pixels.append(Vector2i());

			for (int i = 0; i < size - 1; ++i) {
				int sig = SGN(last.x);

				if (sig == 0) {
					sig = 1;
				}

				if (last.x < 0) {
					center.x = abs(last.x) * -sig;
				} else {
					center.x = abs(last.x + 1) * -sig;
				}

				last = center;
				pixels.append(center);
			}

		} break;
	}

	return pixels;
}

BrushPrefabs::BrushPrefabs() {
}

BrushPrefabs::~BrushPrefabs() {
}

void BrushPrefabs::_bind_methods() {
}
