#ifndef CXX_GODOT_STRING_HEADER
#define CXX_GODOT_STRING_HEADER
#include "core/String.hpp"
#include "rust/cxx.h"

::rust::string godot_string_to_rust_string(const ::godot::String& value);
const ::godot::String &string_from_rust_string(::rust::string string);

#endif //CXX_GODOT_STRING_HEADER
