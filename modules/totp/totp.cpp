/*************************************************************************/
/*  totp.cpp                                                             */
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

#include "totp.h"

#include "core/bind/core_bind.h"
#include "core/crypto/crypto.h"
#include "core/crypto/crypto_core.h"
#include "core/error/error_macros.h"
#include "core/io/json.h"
#include "core/io/marshalls.h"
#include "core/os/os.h"
#include "core/typedefs.h"

TOTP::TOTPAlgorithm TOTP::get_algorithm() const {
	return _algorithm;
}
void TOTP::set_algorithm(const TOTPAlgorithm p_algorithm) {
	_algorithm = p_algorithm;
}

uint8_t TOTP::get_digit_count() const {
	return _digit_count;
}
void TOTP::set_digit_count(const uint8_t p_digits) {
	ERR_FAIL_COND(p_digits < 6 || p_digits > 10);

	_digit_count = p_digits;
}

uint8_t TOTP::get_period() const {
	return _period;
}
void TOTP::set_period(const uint8_t p_period) {
	ERR_FAIL_COND(p_period > 120);

	_period = p_period;
}

String TOTP::get_secret_string() const {
	return _secret_string;
}
void TOTP::set_secret_string(const String &p_secret) {
	String normalized = p_secret.strip_edges().to_upper().replace(" ", "");

	ERR_FAIL_COND(!is_valid_base32(normalized));

	_secret_string = normalized;
}

String TOTP::generate_secret(const uint8_t p_length_bytes) {
	ERR_FAIL_COND_V_MSG(p_length_bytes < 16, String(), "RFC 4226: The length of the shared secret must be at least 128 bits = 16 bytes).");

	Ref<Crypto> crypto = Ref<Crypto>(Crypto::create());

	ERR_FAIL_COND_V(!crypto.is_valid(), String());

	PoolByteArray data = crypto->generate_random_bytes(p_length_bytes);

	String ret = raw_to_base32(data);
	ERR_FAIL_COND_V(ret == "", ret);
	return ret;
}
void TOTP::create_secret(const uint8_t p_length_bytes) {
	ERR_FAIL_COND_MSG(p_length_bytes < 16, "RFC 4226: The length of the shared secret must be at least 128 bits = 16 bytes).");

	_secret_string = generate_secret(p_length_bytes);
}

String TOTP::get_otpauth_uri(const String &p_issuer, const String &p_label, const String &p_image_url, const bool is_totp, const int p_hotp_counter) {
	String uri;

	if (is_totp) {
		uri = "otpauth://totp/";
	} else {
		uri = "otpauth://hotp/";
	}

	uri += p_issuer.uri_encode();
	uri += "%3A"; // ":" uri encoded
	uri += p_label.uri_encode();
	uri += "?secret=" + _secret_string.uri_encode();
	uri += "&issuer=" + p_issuer.uri_encode();

	if (_period != 30) {
		uri += "&period=" + String::num_int64(_period);
	}

	if (_digit_count != 6) {
		uri += "&digits=" + String::num_int64(_digit_count);
	}

	if (_algorithm == TOTP_ALGORITHM_SHA256) {
		uri += "&algorithm=SHA256";
	}

	if (!p_image_url.empty()) {
		uri += "&image=" + p_image_url.uri_encode();
	}

	if (!is_totp) {
		uri += "&counter=" + String::num_int64(p_hotp_counter);
	}

	return uri;
}

TOTP::TOTPResultData TOTP::calculate_totp() {
	return calculate_totp_at(OS::get_singleton()->get_unix_time());
}
TOTP::TOTPResultData TOTP::calculate_totp_at(const uint64_t p_unix_time) {
	uint64_t counter = p_unix_time / _period;

	TOTPResultData d = calculate_hotp(counter);

	d.time_period = _period - p_unix_time % _period;

	return d;
}
TOTP::TOTPResultData TOTP::calculate_hotp(const uint64_t p_counter) {
	TOTPResultData d;

	if (_secret_string.length() == 0) {
		d.result = TOTP_RESULT_ERROR_SECRET_NOT_SET;
		return d;
	}

	PoolVector<uint8_t> secret_key_raw = base32_to_raw(_secret_string);

	// RFC 4226 recommends the value to be 160 bits (= 20 bytes) (it must be at least 128 bits (= 16 bytes))
	if (secret_key_raw.size() < 16) {
		d.result = TOTP_RESULT_ERROR_SECRET_TOO_SHORT;
		return d;
	}

	Ref<Crypto> crypto = Ref<Crypto>(Crypto::create());

	if (!crypto.is_valid()) {
		d.result = TOTP_RESULT_ERROR_NO_MBEDTLS;
		return d;
	}

	// RFC 4226: The Key (K), the Counter (C), and Data values are hashed high-order byte first.
	uint64_t counter = p_counter;
#ifndef BIG_ENDIAN_ENABLED
	counter = BSWAP64(counter);
#endif

	// RFC 4226: Step 1: Generate an HMAC-SHA-1 value Let HS = HMAC-SHA-1(K,C) is a 20-byte string

	PoolByteArray c_msg;
	c_msg.resize(sizeof(uint64_t));
	encode_uint64(counter, c_msg.write().ptr());

	HashingContext::HashType hash_type;

	switch (_algorithm) {
		case TOTP_ALGORITHM_SHA1:
			hash_type = HashingContext::HASH_SHA1;
			break;
		case TOTP_ALGORITHM_SHA256:
			hash_type = HashingContext::HASH_SHA256;
			break;
		default:
			d.result = TOTP_RESULT_ERROR_UNKNOWN;
			return d;
	}

	PoolByteArray result = crypto->hmac_digest(hash_type, secret_key_raw, c_msg);

	// RFC 4226:
	// Step 2: Generate a 4-byte string (Dynamic Truncation)
	// Step 3: Compute an HOTP value
	d.totp = dynamic_truncation(result);
	return d;
}

TOTP::TOTPResult TOTP::validate_totp(const String &p_input) {
	return validate_totp_at(OS::get_singleton()->get_unix_time(), p_input);
}
TOTP::TOTPResult TOTP::validate_totp_at(const uint64_t p_unix_time, const String &p_input) {
	uint64_t counter = p_unix_time / _period;

	return validate_hotp(counter, p_input);
}
TOTP::TOTPResult TOTP::validate_hotp(const uint64_t p_counter, const String &p_input) {
	if (p_input.length() == 0 || !p_input.is_valid_unsigned_integer()) {
		return TOTP_RESULT_VALIDATION_FAIL;
	}

	TOTPResultData d = calculate_hotp(p_counter);

	if (d.result != TOTP_RESULT_OK) {
		return d.result;
	}

	// validate input
	int input_int = p_input.to_int();

	if (d.totp == input_int) {
		return TOTP_RESULT_OK;
	} else {
		return TOTP_RESULT_VALIDATION_FAIL;
	}
}

Array TOTP::calculate_totp_bind() {
	Array ret;

	TOTPResultData d = calculate_totp();

	ret.push_back(d.result);
	ret.push_back(d.totp);
	ret.push_back(d.time_period);

	return ret;
}
Array TOTP::calculate_totp_at_bind(const uint64_t p_unix_time) {
	Array ret;

	TOTPResultData d = calculate_totp_at(p_unix_time);

	ret.push_back(d.result);
	ret.push_back(d.totp);
	ret.push_back(d.time_period);

	return ret;
}
Array TOTP::calculate_hotp_bind(const uint64_t p_counter) {
	Array ret;

	TOTPResultData d = calculate_hotp(p_counter);

	ret.push_back(d.result);
	ret.push_back(d.totp);
	ret.push_back(d.time_period);

	return ret;
}

uint64_t TOTP::calculate_totp_time_period(const uint64_t p_unix_time) {
	return _period - p_unix_time % _period;
}

void TOTP::reset_to_defaults() {
	_algorithm = TOTP_ALGORITHM_SHA1;
	_digit_count = 6;
	_period = 30;
}

int TOTP::dynamic_truncation(const PoolByteArray &p_hmac_result) const {
	// RFC 4226:
	// The Truncate function performs Step 2 and Step 3, i.e., the dynamic
	// truncation and then the reduction modulo 10^Digit. The purpose of
	// the dynamic offset truncation technique is to extract a 4-byte
	// dynamic binary code from a 160-bit (20-byte) HMAC-SHA-1 result.

	int hmac_result_size = p_hmac_result.size();

	ERR_FAIL_COND_V(p_hmac_result.size() == 0, 0);

	PoolByteArray::Read r = p_hmac_result.read();

	// Let OffsetBits be the low-order 4 bits of String[19]
	// Offset = StToNum(OffsetBits) // 0 <= OffSet <= 15
	int offset = r[hmac_result_size - 1] & 0x0F;

	ERR_FAIL_INDEX_V(offset + 3, hmac_result_size, 0);

	// Let P = String[OffSet]...String[OffSet+3]
	// Return the Last 31 bits of P
	uint64_t bin_code = (r[offset] & 0x7f) << 24 | (r[offset + 1] & 0xff) << 16 | (r[offset + 2] & 0xff) << 8 | (r[offset + 3] & 0xff);

	// Step 3: Compute an HOTP value
	uint64_t d = 1;
	for (int i = 0; i < _digit_count; ++i) {
		d *= 10;
	}

	int token = static_cast<int>(bin_code % d);

	return token;
}

String TOTP::raw_to_base32(const PoolVector<uint8_t> &p_data) {
	if (p_data.size() == 0) {
		return String();
	}

	const char b32_alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";

	int data_size = p_data.size();

	uint64_t output_length = (data_size * 8 + 4) / 5;
	PoolVector<uint8_t>::Read r = p_data.read();

	String res;
	res.set_length(output_length);

	for (int i = 0, j = 0; i < data_size; i += 5) {
		uint64_t data_slice = 0;

		int kmax = i + 5;

		if (kmax > data_size) {
			kmax = data_size;
		}

		for (int k = i; k < kmax; ++k) {
			data_slice = (data_slice << 8) | r[k];
		}

		for (int s = 35; s >= 0; s -= 5) {
			res[j++] = b32_alphabet[(data_slice >> s) & 0x1F];
		}
	}

	return res;
}

PoolVector<uint8_t> TOTP::base32_to_raw(const String &p_str) {
	PoolVector<uint8_t> buf;

	if (p_str.length() == 0) {
		return buf;
	}

	ERR_FAIL_COND_V(!is_valid_base32(p_str), buf);

	//                             0  ...                    26   31
	//const char b32_alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
	// "A-(65) BCDEFGHIJKLMNOPQRSTUVWXYZ-(90) 2-(50) 34567-(55)";
	// Substract 50 from every char, then lookup:
	// This shouldn't have any issues due to the is_valid_base32 check above.
	const uint8_t reverse_b32_alphabet[] = {
		26, 27, 28, 29, 30, 31, // 234567
		255, 255, 255, 255, 255, 255, 255, 255, 255, // ASCII entries between 7 and A. This has the padding = character!
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, // ABCDEFGHIJKLMNO
		15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, // PQRSTUVWXYZ
	};

	int strlen = p_str.length();
	CharString cstr = p_str.ascii();
	const char *cd = cstr.get_data();

	int arr_next_index = 0;
	{
		buf.resize(Math::ceil(strlen / 1.6) + 0.001);
		PoolVector<uint8_t>::Write w = buf.write();

		uint8_t current_byte = 0;
		int remaining_bits = 8;
		for (int i = 0; i < strlen; ++i) {
			uint8_t char_alphabet_index = reverse_b32_alphabet[static_cast<uint8_t>(cd[i]) - static_cast<uint8_t>(50)];

			if (char_alphabet_index == 255) {
				// Padding character hit, because input is valid base32 at this point.
				break;
			}

			if (remaining_bits > 5) {
				remaining_bits -= 5;
				current_byte |= char_alphabet_index << remaining_bits;
			} else {
				current_byte |= char_alphabet_index >> (5 - remaining_bits);
				w[arr_next_index++] = current_byte;
				remaining_bits += 3;
				current_byte = char_alphabet_index << remaining_bits;
			}
		}
	}
	buf.resize(arr_next_index);

	return buf;
};

bool TOTP::is_valid_base32(const String &p_str) {
	if (p_str.empty()) {
		return false;
	}

	CharString cstr = p_str.ascii();

	int strlen = cstr.length();
	const char *cd = cstr.get_data();

	for (int i = 0; i < strlen; ++i) {
		char c = cd[i];

		// Base 32 alphabet: ABCDEFGHIJKLMNOPQRSTUVWXYZ234567=
		if (!((c >= 'A' && c <= 'Z') || (c >= '2' && c <= '7') || c == '=')) {
			return false;
		}
	}

	return true;
}

Dictionary TOTP::to_dict() {
	Dictionary dict;

	dict["_algorithm"] = _algorithm;
	dict["_digit_count"] = _digit_count;
	dict["_period"] = _period;
	dict["_secret_string"] = _secret_string;

	return dict;
}
void TOTP::from_dict(const Dictionary &dict) {
	_algorithm = (TOTPAlgorithm)(int)dict["_algorithm"];
	_digit_count = dict["_digit_count"];
	_period = dict["_period"];
	_secret_string = dict["_secret_string"];
}

String TOTP::to_json() {
	return JSON::print(to_dict());
}
void TOTP::from_json(const String &data) {
	Error err;
	String err_txt;
	int err_line;
	Variant v;
	err = JSON::parse(data, v, err_txt, err_line);

	ERR_FAIL_COND(err != OK);

	Dictionary d = v;

	from_dict(d);
}

TOTP::TOTP() {
	_algorithm = TOTP_ALGORITHM_SHA1;
	_digit_count = 6;
	_period = 30;
}

TOTP::~TOTP() {
}

void TOTP::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_algorithm"), &TOTP::get_algorithm);
	ClassDB::bind_method(D_METHOD("set_algorithm", "algorithm"), &TOTP::set_algorithm);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "algorithm", PROPERTY_HINT_ENUM, "SHA1,SHA256,SHA512"), "set_algorithm", "get_algorithm");

	ClassDB::bind_method(D_METHOD("get_digit_count"), &TOTP::get_digit_count);
	ClassDB::bind_method(D_METHOD("set_digit_count", "digits"), &TOTP::set_digit_count);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "digit_count"), "set_digit_count", "get_digit_count");

	ClassDB::bind_method(D_METHOD("get_period"), &TOTP::get_period);
	ClassDB::bind_method(D_METHOD("set_period", "period"), &TOTP::set_period);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "period"), "set_period", "get_period");

	ClassDB::bind_method(D_METHOD("get_secret_string"), &TOTP::get_secret_string);
	ClassDB::bind_method(D_METHOD("set_secret_string", "secret"), &TOTP::set_secret_string);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "secret_string"), "set_secret_string", "get_secret_string");

	ClassDB::bind_method(D_METHOD("generate_secret", "length_bytes"), &TOTP::generate_secret, DEFVAL(20));
	ClassDB::bind_method(D_METHOD("create_secret", "length_bytes"), &TOTP::create_secret, DEFVAL(20));

	ClassDB::bind_method(D_METHOD("get_otpauth_uri", "issuer", "label", "image_url", "is_totp", "hotp_counter"), &TOTP::get_otpauth_uri, DEFVAL(String()), DEFVAL(true), DEFVAL(0));

	ClassDB::bind_method(D_METHOD("calculate_totp"), &TOTP::calculate_totp_bind);
	ClassDB::bind_method(D_METHOD("calculate_totp_at", "unix_time"), &TOTP::calculate_totp_at_bind);
	ClassDB::bind_method(D_METHOD("calculate_hotp", "counter"), &TOTP::calculate_hotp_bind);

	ClassDB::bind_method(D_METHOD("validate_totp", "input"), &TOTP::validate_totp);
	ClassDB::bind_method(D_METHOD("validate_totp_at", "unix_time", "input"), &TOTP::validate_totp_at);
	ClassDB::bind_method(D_METHOD("validate_hotp", "counter", "input"), &TOTP::validate_hotp);

	ClassDB::bind_method(D_METHOD("calculate_totp_time_period", "unix_time"), &TOTP::calculate_totp_time_period);

	ClassDB::bind_method(D_METHOD("reset_to_defaults"), &TOTP::reset_to_defaults);

	ClassDB::bind_method(D_METHOD("raw_to_base32", "data"), &TOTP::raw_to_base32);
	ClassDB::bind_method(D_METHOD("base32_to_raw", "str"), &TOTP::base32_to_raw);
	ClassDB::bind_method(D_METHOD("is_valid_base32", "str"), &TOTP::is_valid_base32);

	ClassDB::bind_method(D_METHOD("from_dict", "dict"), &TOTP::from_dict);
	ClassDB::bind_method(D_METHOD("to_dict"), &TOTP::to_dict);

	ClassDB::bind_method(D_METHOD("to_json"), &TOTP::to_json);
	ClassDB::bind_method(D_METHOD("from_json", "data"), &TOTP::from_json);

	BIND_ENUM_CONSTANT(TOTP_RESULT_OK);
	BIND_ENUM_CONSTANT(TOTP_RESULT_VALIDATION_FAIL);
	BIND_ENUM_CONSTANT(TOTP_RESULT_ERROR_SECRET_BAD_ENCODING);
	BIND_ENUM_CONSTANT(TOTP_RESULT_ERROR_SECRET_TOO_SHORT);
	BIND_ENUM_CONSTANT(TOTP_RESULT_ERROR_SECRET_NOT_SET);
	BIND_ENUM_CONSTANT(TOTP_RESULT_ERROR_NO_MBEDTLS);
	BIND_ENUM_CONSTANT(TOTP_RESULT_ERROR_UNKNOWN);

	BIND_ENUM_CONSTANT(TOTP_ALGORITHM_SHA1);
	BIND_ENUM_CONSTANT(TOTP_ALGORITHM_SHA256);
	//BIND_ENUM_CONSTANT(TOTP_ALGORITHM_SHA512);
}
