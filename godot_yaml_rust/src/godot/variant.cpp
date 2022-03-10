#include "godot/string.h" // NOLINT(modernize-deprecated-headers)
#include "core/Variant.hpp"
#include "core/String.hpp"

namespace gcs {
    namespace ffi {
        int64_t variant_as_i64(const ::godot::Variant &variant) {
            return variant.operator int64_t();
        }

        ::rust::string variant_as_string(const ::godot::Variant &variant) {
            auto as_string = variant.operator ::godot::String();
            return godot_string_to_rust_string(as_string);
        }

        bool variant_as_bool(const ::godot::Variant &variant) {
            return variant.operator bool();
        }

        double variant_as_f64(const ::godot::Variant &variant) {
            return variant.operator double();
        }

        const ::godot::Variant &empty_variant() {
            return *(new ::godot::Variant());
        }

        const ::godot::Variant &variant_from_i64(int64_t value) {
            return *(new ::godot::Variant(value));
        }

        const ::godot::Variant &variant_from_string(::rust::string value) {
            return *(new ::godot::Variant(string_from_rust_string(value)));
        }

        const ::godot::Variant &variant_from_bool(bool value) {
            return *(new ::godot::Variant(value));
        }

        const ::godot::Variant &variant_from_f64(double value) {
            return *(new ::godot::Variant(value));
        }
    }
}