/**************************************************************************/
/*  FBXProperties.h                                                       */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

/*
Open Asset Import Library (assimp)
----------------------------------------------------------------------

Copyright (c) 2006-2019, assimp team

All rights reserved.

Redistribution and use of this software in source and binary forms,
with or without modification, are permitted provided that the
following conditions are met:

* Redistributions of source code must retain the above
  copyright notice, this list of conditions and the
  following disclaimer.

* Redistributions in binary form must reproduce the above
  copyright notice, this list of conditions and the
  following disclaimer in the documentation and/or other
  materials provided with the distribution.

* Neither the name of the assimp team, nor the names of its
  contributors may be used to endorse or promote products
  derived from this software without specific prior
  written permission of the assimp team.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

----------------------------------------------------------------------
*/

/** @file  FBXProperties.h
 *  @brief FBX dynamic properties
 */

#ifndef FBXPROPERTIES_H
#define FBXPROPERTIES_H

#include "FBXParser.h"
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace FBXDocParser {

// Forward declarations
class Element;

/** Represents a dynamic property. Type info added by deriving classes,
 *  see #TypedProperty.
 Example:
 @verbatim
   P: "ShininessExponent", "double", "Number", "",0.5
 @endvebatim
*/
class Property {
protected:
	Property();

public:
	virtual ~Property();

public:
	template <typename T>
	const T *As() const {
		return dynamic_cast<const T *>(this);
	}
};

template <typename T>
class TypedProperty : public Property {
public:
	explicit TypedProperty(const T &value) :
			value(value) {
		// empty
	}

	const T &Value() const {
		return value;
	}

private:
	T value;
};

#define new_Property new Property
typedef Property *PropertyPtr;
typedef std::map<std::string, PropertyPtr> DirectPropertyMap;
typedef std::map<std::string, PropertyPtr> PropertyMap;
typedef std::map<std::string, ElementPtr> LazyPropertyMap;

/**
 *  Represents a property table as can be found in the newer FBX files (Properties60, Properties70)
 */
class PropertyTable {
public:
	// in-memory property table with no source element
	PropertyTable();
	PropertyTable(const PropertyTable *templateProps);
	PropertyTable(const ElementPtr element, const PropertyTable *templateProps);
	~PropertyTable();

	PropertyPtr Get(const std::string &name) const;

	// PropertyTable's need not be coupled with FBX elements so this can be NULL
	ElementPtr GetElement() const {
		return element;
	}

	const PropertyMap &GetProperties() const {
		return props;
	}

	const LazyPropertyMap &GetLazyProperties() const {
		return lazyProps;
	}

	const PropertyTable *TemplateProps() const {
		return templateProps;
	}

	DirectPropertyMap GetUnparsedProperties() const;

private:
	LazyPropertyMap lazyProps;
	mutable PropertyMap props;
	const PropertyTable *templateProps = nullptr;
	const ElementPtr element = nullptr;
};

// ------------------------------------------------------------------------------------------------
template <typename T>
inline T PropertyGet(const PropertyTable *in, const std::string &name, const T &defaultValue) {
	PropertyPtr prop = in->Get(name);
	if (nullptr == prop) {
		return defaultValue;
	}

	// strong typing, no need to be lenient
	const TypedProperty<T> *const tprop = prop->As<TypedProperty<T>>();
	if (nullptr == tprop) {
		return defaultValue;
	}

	return tprop->Value();
}

// ------------------------------------------------------------------------------------------------
template <typename T>
inline T PropertyGet(const PropertyTable *in, const std::string &name, bool &result, bool useTemplate = false) {
	PropertyPtr prop = in->Get(name);
	if (nullptr == prop) {
		if (!useTemplate) {
			result = false;
			return T();
		}
		const PropertyTable *templ = in->TemplateProps();
		if (nullptr == templ) {
			result = false;
			return T();
		}
		prop = templ->Get(name);
		if (nullptr == prop) {
			result = false;
			return T();
		}
	}

	// strong typing, no need to be lenient
	const TypedProperty<T> *const tprop = prop->As<TypedProperty<T>>();
	if (nullptr == tprop) {
		result = false;
		return T();
	}

	result = true;
	return tprop->Value();
}

} // namespace FBXDocParser

#endif // FBXPROPERTIES_H
