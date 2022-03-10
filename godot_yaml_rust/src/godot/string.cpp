#include <string>
#include "rust/cxx.h"
#include "godot/string.h" // NOLINT(modernize-deprecated-headers)

::rust::string godot_string_to_rust_string(const ::godot::String &value) {
    auto as_wstring = std::wstring(value.unicode_str());
    auto as_string = std::string(as_wstring.begin(), as_wstring.end());
    return as_string;
}

const ::godot::String &string_from_rust_string(::rust::string string) {
    return *(new ::godot::String(string.c_str()));
}
