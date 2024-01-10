/*************************************************************************/
/*  smtp_client.cpp                                                      */
/*************************************************************************/
/*                         This file is part of:                         */
/*                   PANDEMONIUM ENGINE'S SMTP MODULE                    */
/*             https://github.com/Relintai/pandemonium_engine            */
/*************************************************************************/
/* Copyright (c) 2022-present Péter Magyar.                              */
/* Copyright (c) 2021-2024 Nicolò Santilio                               */
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

#include "smtp_client.h"


SMTPClient::SMTPClient() {
}
SMTPClient::~SMTPClient() {
}

void SMTPClient::_bind_methods() {
	//ClassDB::bind_method(D_METHOD("get_ignored_urls"), &SMTPClient::get_ignored_urls);
	//ClassDB::bind_method(D_METHOD("set_ignored_urls", "val"), &SMTPClient::set_ignored_urls);
	//ADD_PROPERTY(PropertyInfo(Variant::POOL_STRING_ARRAY, "ignored_urls"), "set_ignored_urls", "get_ignored_urls");
}
