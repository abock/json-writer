//
// json-writer.h: a tiny JSON serializer with flexible formatting
//
// Authors:
//   Aaron Bockover (abock@xamarin.com)
//
// Copyright 2013-2014 Xamarin Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef _JSONWRITER_H
#define _JSONWRITER_H

#include <iostream>
#include <string>
#include <stack>

class JsonWriter
{
public:

	enum ContainerType {
		CONTAINER_TYPE_ARRAY,
		CONTAINER_TYPE_OBJECT
	};

	enum ContainerLayout {
		CONTAINER_LAYOUT_INHERIT,
		CONTAINER_LAYOUT_MULTI_LINE,
		CONTAINER_LAYOUT_SINGLE_LINE
	};

	explicit JsonWriter () :
		writer (NULL),
		initialIndentDepth (0),
		indent ("  "),
		containerPadding (" "),
		keyPaddingLeft (""),
		keyPaddingRight (" "),
		defaultContainerLayout (CONTAINER_LAYOUT_MULTI_LINE),
		forceDefaultContainerLayout (false)
	{
	}

	void ConfigureCompressedOutput ()
	{
		SetIndent ("");
		SetContainerPadding ("");
		SetKeyPaddingLeft ("");
		SetKeyPaddingRight ("");
		SetDefaultContainerLayout (CONTAINER_LAYOUT_SINGLE_LINE);
		SetForceDefaultContainerLayout (true);
	}

	std::ostream *GetWriter () { return writer; }
	void SetWriter (std::ostream *writer) { this->writer = writer; }

	int GetInitialIndentDepth () { return initialIndentDepth; }
	void SetInitialIndentDepth (int depth) { initialIndentDepth = depth; }

	const char *GetIndent () { return indent; }
	void SetIndent (const char *indent) { this->indent = indent; }

	const char *GetContainerPadding () { return containerPadding; }
	void SetContainerPadding (const char *padding) { containerPadding = padding; }

	const char *GetKeyPaddingLeft () { return keyPaddingLeft; }
	void SetKeyPaddingLeft (const char *padding) { keyPaddingLeft = padding; }

	const char *GetKeyPaddingRight () { return keyPaddingRight; }
	void SetKeyPaddingRight (const char *padding) { keyPaddingRight = padding; }

	ContainerLayout GetDefaultContainerLayout () { return defaultContainerLayout; }
	void SetDefaultContainerLayout (ContainerLayout layout) { defaultContainerLayout = layout; }

	bool GetForceDefaultContainerLayout () { return forceDefaultContainerLayout; }
	void SetForceDefaultContainerLayout (bool force) { forceDefaultContainerLayout = force; }

	std::ostream& Write ()
	{
		if (writer == NULL)
			return std::cout;
		return *writer;
	}

	void WriteEscapedChar (char c);
	void WriteString (const char *str);
	
	void StartChild (bool isKey);
	void StartChild () { StartChild (false); }

	void StartContainer (ContainerType type, ContainerLayout layout);
	void EndContainer ();

	void StartArray () { StartContainer (CONTAINER_TYPE_ARRAY, CONTAINER_LAYOUT_INHERIT); }
	void StartArray (ContainerLayout layout) { StartContainer (CONTAINER_TYPE_ARRAY, layout); }
	void StartShortArray () { StartContainer (CONTAINER_TYPE_ARRAY, CONTAINER_LAYOUT_SINGLE_LINE); }
	void EndArray () { EndContainer (); }

	void StartObject () { StartContainer (CONTAINER_TYPE_OBJECT, CONTAINER_LAYOUT_INHERIT); }
	void StartObject (ContainerLayout layout) { StartContainer (CONTAINER_TYPE_OBJECT, layout); }
	void StartShortObject () { StartContainer (CONTAINER_TYPE_OBJECT, CONTAINER_LAYOUT_SINGLE_LINE); }
	void EndObject () { EndContainer (); }

	void Key (const char *key);

	void NullValue ();
	void Value (const char *value);
	void Value (std::string value);
	void Value (bool value);

	#define VALUE_DEF(t) void Value (t value) { StartChild (); Write () << value; }
	#define KEYVALUE_DEF(t) void KeyValue (const char *key, t value) { Key (key); Value (value); }

	VALUE_DEF(int8_t)
	VALUE_DEF(uint8_t)
	VALUE_DEF(int16_t)
	VALUE_DEF(uint16_t)
	VALUE_DEF(int32_t)
	VALUE_DEF(uint32_t)
	VALUE_DEF(int64_t)
	VALUE_DEF(uint64_t)
	VALUE_DEF(float)
	VALUE_DEF(double)

	void KeyNullValue (const char *key) { Key (key); NullValue (); }

	KEYVALUE_DEF(const char *)
	KEYVALUE_DEF(std::string)
	KEYVALUE_DEF(bool)
	KEYVALUE_DEF(int8_t)
	KEYVALUE_DEF(uint8_t)
	KEYVALUE_DEF(int16_t)
	KEYVALUE_DEF(uint16_t)
	KEYVALUE_DEF(int32_t)
	KEYVALUE_DEF(uint32_t)
	KEYVALUE_DEF(int64_t)
	KEYVALUE_DEF(uint64_t)
	KEYVALUE_DEF(float)
	KEYVALUE_DEF(double)

private:

	std::ostream *writer;
	int initialIndentDepth;
	const char *indent;
	const char *containerPadding;
	const char *keyPaddingLeft;
	const char *keyPaddingRight;
	ContainerLayout defaultContainerLayout;
	bool forceDefaultContainerLayout;

	struct Container {
		ContainerType type;
		ContainerLayout layout;
		bool isKey;
		int childCount;

		Container (ContainerType type, ContainerLayout layout) :
			type (type),
			layout (layout),
			isKey (false),
			childCount (0)
		{
		}
	};

	std::stack<Container *> depth;;

	void Indent ();
};

#endif /* _JSONWRITER_H */
