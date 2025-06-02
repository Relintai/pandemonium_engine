#ifndef TOTP_H
#define TOTP_H

/*************************************************************************/
/*  totp.h                                                               */
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

#include "core/object/reference.h"
#include "core/string/ustring.h"
#include "core/variant/variant.h"

class TOTP : public Reference {
	GDCLASS(TOTP, Reference);

public:
	enum TOTPResult {
		TOTP_RESULT_OK = 0,
		TOTP_RESULT_VALIDATION_FAIL,
		TOTP_RESULT_ERROR_SECRET_BAD_ENCODING,
		TOTP_RESULT_ERROR_SECRET_TOO_SHORT,
		TOTP_RESULT_ERROR_SECRET_NOT_SET,
		TOTP_RESULT_ERROR_NO_MBEDTLS,
		TOTP_RESULT_ERROR_UNKNOWN,
	};

	enum TOTPAlgorithm {
		TOTP_ALGORITHM_SHA1 = 0,
		TOTP_ALGORITHM_SHA256,
		//TOTP_ALGORITHM_SHA512, Will need to be implemented for core/hashing context.
	};

	TOTPAlgorithm get_algorithm() const;
	void set_algorithm(const TOTPAlgorithm p_algorithm);

	// RFC 4226: The value displayed on the token MUST be easily read and entered
	// by the user: This requires the HOTP value to be of reasonable length.
	// The HOTP value must be at least a 6-digit value.
	// Implementations MUST extract a 6-digit code at a minimum and possibly
	// 7 and 8-digit code. Depending on security requirements, Digit = 7 or
	// more SHOULD be considered in order to extract a longer HOTP value.
	uint8_t get_digit_count() const;
	void set_digit_count(const uint8_t p_digits);

	uint8_t get_period() const;
	void set_period(const uint8_t p_period);

	// Base32 encoded
	String get_secret_string() const;
	void set_secret_string(const String &p_secret);

	// RFC 4226 recommends the value to be 160 (= 20) (it must be at least 128 (= 16))
	String generate_secret(const uint8_t p_length_bytes = 20);
	void create_secret(const uint8_t p_length_bytes = 20);

	String get_uri();

	struct TOTPResultData {
		TOTPResult result;
		int totp;
		uint64_t time_point;

		TOTPResultData() {
			result = TOTP_RESULT_OK;
			time_point = 0;
		}
	};

	TOTPResultData calculate_totp();
	TOTPResultData calculate_totp_at(const uint64_t p_unix_time);
	TOTPResultData calculate_hotp(const uint64_t p_counter);

	TOTPResult validate_totp(const String &p_input);
	TOTPResult validate_totp_at(const uint64_t p_unix_time, const String &p_input);
	TOTPResult validate_hotp(const uint64_t p_counter, const String &p_input);

	Array calculate_totp_bind();
	Array calculate_totp_at_bind(const uint64_t p_unix_time);
	Array calculate_hotp_bind(const uint64_t p_counter);

	uint64_t calculate_totp_time_point(const uint64_t p_unix_time);

	void reset_to_defaults();

	int dynamic_truncation(const PoolByteArray &p_hmac_result) const;

	String raw_to_base32(const PoolVector<uint8_t> &p_data);
	PoolVector<uint8_t> base32_to_raw(const String &p_str);
	bool is_valid_base32(const String &p_str);

	TOTP();
	~TOTP();

protected:
	static void _bind_methods();

	TOTPAlgorithm _algorithm;
	uint8_t _digit_count;
	uint8_t _period;
	String _secret_string;
};

VARIANT_ENUM_CAST(TOTP::TOTPResult);
VARIANT_ENUM_CAST(TOTP::TOTPAlgorithm);

#endif
