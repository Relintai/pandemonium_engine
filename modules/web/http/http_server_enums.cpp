/*************************************************************************/
/*  http_server_enums.cpp                                                */
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

#include "http_server_enums.h"

String HTTPServerEnums::get_status_code_header_string(const HTTPServerEnums::HTTPStatusCode code) {
	switch (code) {
		case HTTP_STATUS_CODE_100_CONTINUE:
			return "100 Continue";
			break;
		case HTTP_STATUS_CODE_101_SWITCHING_PROTOCOLS:
			return "101 Switching Protocols";
			break;
		case HTTP_STATUS_CODE_102_PROCESSING:
			return "102 Processing";
			break;
		case HTTP_STATUS_CODE_103_EARLY_HINTS:
			return "103 Early Hints";
			break;
		case HTTP_STATUS_CODE_200_OK:
			return "200 OK";
			break;
		case HTTP_STATUS_CODE_201_CREATED:
			return "201 Created";
			break;
		case HTTP_STATUS_CODE_202_ACCEPTED:
			return "202 Accepted";
			break;
		case HTTP_STATUS_CODE_203_NON_AUTHORITATIVE_INFORMATION:
			return "203 Non-Authoritative Information";
			break;
		case HTTP_STATUS_CODE_204_NO_CONTENT:
			return "204 No Content";
			break;
		case HTTP_STATUS_CODE_205_RESET_CONTENT:
			return "205 Reset Content";
			break;
		case HTTP_STATUS_CODE_206_PARTIAL_CONTENT:
			return "206 Partial Content";
			break;
		case HTTP_STATUS_CODE_207_MULTI_STATUS:
			return "207 Multi-Status";
			break;
		case HTTP_STATUS_CODE_208_ALREADY_REPORTED:
			return "208 Already Reported";
			break;
		case HTTP_STATUS_CODE_226_IM_USED:
			return "226 IM Used";
			break;
		case HTTP_STATUS_CODE_300_MULTIPLE_CHOICES:
			return "300 Multiple Choices";
			break;
		case HTTP_STATUS_CODE_301_MOVED_PERMANENTLY:
			return "301 Moved Permanently";
			break;
		case HTTP_STATUS_CODE_302_FOUND:
			return "302 Found";
			break;
		case HTTP_STATUS_CODE_303_SEE_OTHER:
			return "303 See Other";
			break;
		case HTTP_STATUS_CODE_304_NOT_MODIFIED:
			return "304 Not Modified";
			break;
		case HTTP_STATUS_CODE_305_USE_PROXY:
			return "305 Use Proxy";
			break;
		case HTTP_STATUS_CODE_306_UNUSED:
			return "306 unused";
			break;
		case HTTP_STATUS_CODE_307_TEMPORARY_REDIRECT:
			return "307 Temporary Redirect";
			break;
		case HTTP_STATUS_CODE_308_PERMANENT_REDIRECT:
			return "308 Permanent Redirect";
			break;
		case HTTP_STATUS_CODE_400_BAD_REQUEST:
			return "400 Bad Request";
			break;
		case HTTP_STATUS_CODE_401_UNAUTHORIZED:
			return "401 Unauthorized";
			break;
		case HTTP_STATUS_CODE_402_PAYMENT_REQUIRED:
			return "402 Payment Required";
			break;
		case HTTP_STATUS_CODE_403_FORBIDDEN:
			return "403 Forbidden";
			break;
		case HTTP_STATUS_CODE_404_NOT_FOUND:
			return "404 Not Found";
			break;
		case HTTP_STATUS_CODE_405_METHOD_NOT_ALLOWED:
			return "405 Method Not Allowed";
			break;
		case HTTP_STATUS_CODE_406_NOT_ACCEPTABLE:
			return "406 Not Acceptable";
			break;
		case HTTP_STATUS_CODE_407_PROXY_AUTHENTICATION_REQUIRED:
			return "407 Proxy Authentication Required";
			break;
		case HTTP_STATUS_CODE_408_REQUEST_TIMEOUT:
			return "408 Request Timeout";
			break;
		case HTTP_STATUS_CODE_409_CONFLICT:
			return "409 Conflict";
			break;
		case HTTP_STATUS_CODE_410_GONE:
			return "410 Gone";
			break;
		case HTTP_STATUS_CODE_411_LENGTH_REQUIRED:
			return "411 Length Required";
			break;
		case HTTP_STATUS_CODE_412_PRECONDITION_FAILED:
			return "412 Precondition Failed";
			break;
		case HTTP_STATUS_CODE_413_REQUEST_ENTITY_TOO_LARGE:
			return "413 Payload Too Large";
			break;
		case HTTP_STATUS_CODE_414_REQUEST_URI_TOO_LONG:
			return "414 URI Too Long";
			break;
		case HTTP_STATUS_CODE_415_UNSUPPORTED_MEDIA_TYPE:
			return "415 Unsupported Media Type";
			break;
		case HTTP_STATUS_CODE_416_REQUESTED_RANGE_NOT_SATISFIABLE:
			return "416 Range Not Satisfiable";
			break;
		case HTTP_STATUS_CODE_417_EXPECTATION_FAILED:
			return "417 Expectation Failed";
			break;
		case HTTP_STATUS_CODE_418_IM_A_TEAPOT:
			return "418 I'm a teapot";
			break;
		case HTTP_STATUS_CODE_421_MISDIRECTED_REQUEST:
			return "421 Misdirected Request";
			break;
		case HTTP_STATUS_CODE_422_UNPROCESSABLE_ENTITY:
			return "422 Unprocessable Entity";
			break;
		case HTTP_STATUS_CODE_423_LOCKED:
			return "423 Locked";
			break;
		case HTTP_STATUS_CODE_424_FAILED_DEPENDENCY:
			return "424 Failed Dependency";
			break;
		case HTTP_STATUS_CODE_425_TOO_EARLY:
			return "425 Too Early";
			break;
		case HTTP_STATUS_CODE_426_UPGRADE_REQUIRED:
			return "426 Upgrade Required";
			break;
		case HTTP_STATUS_CODE_428_PRECONDITION_REQUIRED:
			return "428 Precondition Required";
			break;
		case HTTP_STATUS_CODE_429_TOO_MANY_REQUESTS:
			return "429 Too Many Requests";
			break;
		case HTTP_STATUS_CODE_431_REQUEST_HEADER_FIELDS_TOO_LARGE:
			return "431 Request Header Fields Too Large";
			break;
		case HTTP_STATUS_CODE_451_UNAVAILABLE_FOR_LEGAL_REASONS:
			return "451 Unavailable For Legal Reasons";
			break;
		case HTTP_STATUS_CODE_500_INTERNAL_SERVER_ERROR:
			return "500 Internal Server Error";
			break;
		case HTTP_STATUS_CODE_501_NOT_IMPLEMENTED:
			return "501 Not Implemented";
			break;
		case HTTP_STATUS_CODE_502_BAD_GATEWAY:
			return "502 Bad Gateway";
			break;
		case HTTP_STATUS_CODE_503_SERVICE_UNAVAILABLE:
			return "503 Service Unavailable";
			break;
		case HTTP_STATUS_CODE_504_GATEWAY_TIMEOUT:
			return "504 Gateway Timeout";
			break;
		case HTTP_STATUS_CODE_505_HTTP_VERSION_NOT_SUPPORTED:
			return "505 HTTP Version Not Supported";
			break;
		case HTTP_STATUS_CODE_506_VARIANT_ALSO_NEGOTIATES:
			return "506 Variant Also Negotiates";
			break;
		case HTTP_STATUS_CODE_507_INSUFFICIENT_STORAGE:
			return "507 Insufficient Storage";
			break;
		case HTTP_STATUS_CODE_508_LOOP_DETECTED:
			return "508 Loop Detected";
			break;
		case HTTP_STATUS_CODE_510_NOT_EXTENDED:
			return "510 Not Extended";
			break;
		case HTTP_STATUS_CODE_511_NETWORK_AUTHENTICATION_REQUIRED:
			return "511 Network Authentication Required";
			break;
		case HTTP_STATUS_CODE_UNKNOWN:
		default:
			return "0 UNK";
	}

	return "0 UNK";
};

String HTTPServerEnums::get_status_code_header_stringi(const int code) {
	return get_status_code_header_string(static_cast<HTTPStatusCode>(code));
}

void HTTPServerEnums::_bind_methods() {
	BIND_ENUM_CONSTANT(HTTP_METHOD_GET);
	BIND_ENUM_CONSTANT(HTTP_METHOD_POST);
	BIND_ENUM_CONSTANT(HTTP_METHOD_HEAD);
	BIND_ENUM_CONSTANT(HTTP_METHOD_PUT);
	BIND_ENUM_CONSTANT(HTTP_METHOD_DELETE);
	BIND_ENUM_CONSTANT(HTTP_METHOD_OPTIONS);
	BIND_ENUM_CONSTANT(HTTP_METHOD_PATCH);
	BIND_ENUM_CONSTANT(HTTP_METHOD_INVALID);

	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_UNKNOWN);

	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_100_CONTINUE);
	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_101_SWITCHING_PROTOCOLS);
	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_102_PROCESSING);
	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_103_EARLY_HINTS);

	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_200_OK);
	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_201_CREATED);
	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_202_ACCEPTED);
	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_203_NON_AUTHORITATIVE_INFORMATION);
	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_204_NO_CONTENT);
	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_205_RESET_CONTENT);
	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_206_PARTIAL_CONTENT);
	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_207_MULTI_STATUS);
	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_208_ALREADY_REPORTED);
	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_226_IM_USED);

	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_300_MULTIPLE_CHOICES);
	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_301_MOVED_PERMANENTLY);
	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_302_FOUND);
	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_303_SEE_OTHER);
	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_304_NOT_MODIFIED);
	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_305_USE_PROXY);
	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_306_UNUSED);
	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_307_TEMPORARY_REDIRECT);
	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_308_PERMANENT_REDIRECT);

	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_400_BAD_REQUEST);
	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_401_UNAUTHORIZED);
	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_402_PAYMENT_REQUIRED);
	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_403_FORBIDDEN);
	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_404_NOT_FOUND);
	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_405_METHOD_NOT_ALLOWED);
	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_406_NOT_ACCEPTABLE);
	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_407_PROXY_AUTHENTICATION_REQUIRED);
	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_408_REQUEST_TIMEOUT);
	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_409_CONFLICT);
	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_410_GONE);
	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_411_LENGTH_REQUIRED);
	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_412_PRECONDITION_FAILED);
	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_413_REQUEST_ENTITY_TOO_LARGE);
	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_414_REQUEST_URI_TOO_LONG);
	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_415_UNSUPPORTED_MEDIA_TYPE);
	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_416_REQUESTED_RANGE_NOT_SATISFIABLE);
	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_417_EXPECTATION_FAILED);
	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_418_IM_A_TEAPOT);
	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_421_MISDIRECTED_REQUEST);
	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_422_UNPROCESSABLE_ENTITY);
	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_423_LOCKED);

	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_424_FAILED_DEPENDENCY);
	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_425_TOO_EARLY);
	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_426_UPGRADE_REQUIRED);
	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_428_PRECONDITION_REQUIRED);
	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_429_TOO_MANY_REQUESTS);
	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_431_REQUEST_HEADER_FIELDS_TOO_LARGE);
	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_451_UNAVAILABLE_FOR_LEGAL_REASONS);

	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_500_INTERNAL_SERVER_ERROR);
	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_501_NOT_IMPLEMENTED);
	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_502_BAD_GATEWAY);
	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_503_SERVICE_UNAVAILABLE);
	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_504_GATEWAY_TIMEOUT);
	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_505_HTTP_VERSION_NOT_SUPPORTED);
	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_506_VARIANT_ALSO_NEGOTIATES);
	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_507_INSUFFICIENT_STORAGE);
	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_508_LOOP_DETECTED);
	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_510_NOT_EXTENDED);
	BIND_ENUM_CONSTANT(HTTP_STATUS_CODE_511_NETWORK_AUTHENTICATION_REQUIRED);
}

HTTPServerEnums::HTTPServerEnums() {
}

HTTPServerEnums::~HTTPServerEnums() {
}
