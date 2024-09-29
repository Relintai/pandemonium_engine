#pragma once
/*
 The MIT License (MIT)
 
 Copyright (c) 2013 Ivan Vashchaev
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 the Software, and to permit persons to whom the Software is furnished to do so,
 subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <stdint.h>
#include <stddef.h>
#include <assert.h>

#define JSON_VALUE_PAYLOAD_MASK 0x00007FFFFFFFFFFFULL
#define JSON_VALUE_NAN_MASK 0x7FF8000000000000ULL
#define JSON_VALUE_NULL 0x7FFF800000000000ULL
#define JSON_VALUE_TAG_MASK 0xF
#define JSON_VALUE_TAG_SHIFT 47

enum JsonTag {
	JSON_TAG_NUMBER = 0,
	JSON_TAG_STRING,
	JSON_TAG_BOOL,
	JSON_TAG_ARRAY,
	JSON_TAG_OBJECT,
	JSON_TAG_NULL = 0xF
};

struct JsonNode;

union JsonValue {
	uint64_t ival;
	double fval;

	JsonValue()
		: ival(JSON_VALUE_NULL) {
	}
	JsonValue(double x)
		: fval(x) {
	}
	JsonValue(JsonTag tag, void *p) {
		uint64_t x = (uint64_t)p;
		assert(tag <= JSON_VALUE_TAG_MASK);
		assert(x <= JSON_VALUE_PAYLOAD_MASK);
		ival = JSON_VALUE_NAN_MASK | ((uint64_t)tag << JSON_VALUE_TAG_SHIFT) | x;
	}
	bool isDouble() const {
		return (int64_t)ival <= (int64_t)JSON_VALUE_NAN_MASK;
	}
	JsonTag getTag() const {
		return isDouble() ? JSON_TAG_NUMBER : JsonTag((ival >> JSON_VALUE_TAG_SHIFT) & JSON_VALUE_TAG_MASK);
	}
	uint64_t getPayload() const {
		assert(!isDouble());
		return ival & JSON_VALUE_PAYLOAD_MASK;
	}
	double toNumber() const {
		assert(getTag() == JSON_TAG_NUMBER);
		return fval;
	}
	bool toBool() const {
		assert(getTag() == JSON_TAG_BOOL);
		return (bool)getPayload();
	}
	char *toString() const {
		assert(getTag() == JSON_TAG_STRING);
		return (char *)getPayload();
	}
	JsonNode *toNode() const {
		assert(getTag() == JSON_TAG_ARRAY || getTag() == JSON_TAG_OBJECT);
		return (JsonNode *)getPayload();
	}
};

struct JsonNode {
	JsonValue value;
	JsonNode *next;
	char *key;
};

struct JsonIterator {
	JsonNode *p;

	void operator++() {
		p = p->next;
	}
	bool operator!=(const JsonIterator &x) const {
		return p != x.p;
	}
	JsonNode *operator*() const {
		return p;
	}
	JsonNode *operator->() const {
		return p;
	}
};

inline JsonIterator JsonBegin(JsonValue o) {
	return JsonIterator{o.toNode()};
}
inline JsonIterator JsonEnd(JsonValue) {
	return JsonIterator{nullptr};
}

enum JsonParseStatus {
	JSON_PARSE_OK,
	JSON_PARSE_BAD_NUMBER,
	JSON_PARSE_BAD_STRING,
	JSON_PARSE_BAD_IDENTIFIER,
	JSON_PARSE_STACK_OVERFLOW,
	JSON_PARSE_STACK_UNDERFLOW,
	JSON_PARSE_MISMATCH_BRACKET,
	JSON_PARSE_UNEXPECTED_CHARACTER,
	JSON_PARSE_UNQUOTED_KEY,
	JSON_PARSE_BREAKING_BAD
};

class JsonAllocator {
	struct Zone {
		Zone *next;
		size_t used;
	} *head = nullptr;

public:
	~JsonAllocator();
	void *allocate(size_t size);
	void deallocate();
};

JsonParseStatus jsonParse(char *str, char **endptr, JsonValue *value, JsonAllocator &allocator);
