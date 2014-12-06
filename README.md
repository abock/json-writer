# json-writer

A very simple and light-weight JSON writer written in C++ with flexible
formatting and human readability in mind.

Released under the [Apache 2.0 License](LICENSE).

## API & Features

The writer is has fully configurable formatting with condensed ("short")
objects and arrays for easier readability. When writing JSON contents,
use the API to generate nice human readable content. For fully compressed
output, simply call `ConfigureCompressedOutput` before writing. This can
be great for easy to read JSON in debug builds, but tightly compressed
JSON in production builds:

```c++
#if !DEBUG
writer->ConfigureCompressedOuput ();
#endif
```

### White Space
```c++
void SetInitialIndentDepth (int depth);
void SetIndent (const char *indent);
void SetContainerPadding (const char *padding);
void SetKeyPaddingLeft (const char *padding);
void SetKeyPaddingRight (const char *padding);
void ConfigureCompressedOutput ();
```

### Object & Array Layout
```c++
enum ContainerLayout {
  CONTAINER_LAYOUT_INHERIT,
  CONTAINER_LAYOUT_MULTI_LINE,
  CONTAINER_LAYOUT_SINGLE_LINE
};

void SetDefaultContainerLayout (ContainerLayout layout);
void SetForceDefaultContainerLayout (bool force);
```

### Writing

Write to any `std::ostream`, defaulting to standard output:

```c++
void SetWriter (std::ostream *writer)
```

Objects:

```c++
void StartObject (ContainerLayout layout);
void StartObject ();      // inherits parent's layout
void StartShortObject (); // uses CONTAINER_LAYOUT_SINGLE_LINE
void EndObject ();        // same as EndContainer ()
```

Arrays:

```c++
void StartArray (ContainerLayout layout);
void StartArray ();      // inherits parent's layout
void StartShortArray (); // uses CONTAINER_LAYOUT_SINGLE_LINE
void EndArray ();        // same as EndContainer ()
```

Keys & Values:

```c++
void Key (const char *key);

void NullValue ();
void Value (const char *value);
void Value (std::string value);
void Value (bool value);
void Value (int8_t);
void Value (uint8_t);
void Value (int16_t);
void Value (uint16_t);
void Value (int32_t);
void Value (uint32_t);
void Value (int64_t);
void Value (uint64_t);
void Value (float);
void Value (double);

void KeyNullValue (const char *key);
void KeyValue (const char *key, const char *value);
void KeyValue (const char *key, std::string value);
void KeyValue (const char *key, bool value);
void KeyValue (const char *key, int8_t value);
void KeyValue (const char *key, uint8_t value);
void KeyValue (const char *key, int16_t value);
void KeyValue (const char *key, uint16_t value);
void KeyValue (const char *key, int32_t value);
void KeyValue (const char *key, uint32_t value);
void KeyValue (const char *key, int64_t value);
void KeyValue (const char *key, uint64_t value);
void KeyValue (const char *key, float value);
void KeyValue (const char *key, double value);
```

Low functionality for more fine-grained control of writing:

```c++
void WriteEscapedChar (char c);
void WriteString (const char *str);
void StartChild (bool isKey = false);

enum ContainerType {
  CONTAINER_TYPE_ARRAY,
  CONTAINER_TYPE_OBJECT
};

enum ContainerLayout {
  CONTAINER_LAYOUT_INHERIT,
  CONTAINER_LAYOUT_MULTI_LINE,
  CONTAINER_LAYOUT_SINGLE_LINE
};

void StartContainer (ContainerType type, ContainerLayout layout);
void EndContainer ();
```

## Example

```c++
auto writer = new JsonWriter;
writer->StartArray ();

  writer->StartShortObject ();
    writer->KeyValue ("name", "Aaron");
    writer->KeyValue ("age", 7);
  writer->EndObject ();

  writer->StartObject ();
    writer->KeyValue ("animal", "cat");
    writer->KeyValue ("life-expectancy", "forever");
    writer->KeyValue ("url", "http://catoverflow.com");
    
    writer->Key ("catch-phrases");
    writer->StartArray ();
      writer->Value ("meow");
      writer->Value ("hiss");
      writer->Value ("purr");
    writer->EndArray ();
    
    writer->Key ("short-array");
    writer->StartShortArray ();
      writer->Value (1);
      writer->Value ((uint64_t)0xabcdef123456);
      writer->Value (M_PI);
    writer->EndContainer ();

  writer->EndObject (),
  writer->Value (false);
writer->EndArray ();
```

Output:

```json
[ 
  { "name": "Aaron", "age": 7 },
  { 
    "animal": "cat",
    "life-expectancy": "forever",
    "url": "http://catoverflow.com",
    "catch-phrases": [ 
      "meow",
      "hiss",
      "purr"
    ],
    "short-array": [ 1, 188900967593046, 3.14159 ]
  },
  false
]
```
