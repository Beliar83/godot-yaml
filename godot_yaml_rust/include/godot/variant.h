#ifndef CXX_GODOT_VARIANT_H
#define CXX_GODOT_VARIANT_H
#include "core/Variant.hpp"
#include "rust/cxx.h"

namespace godot {
    namespace ffi {
        using Variant = ::godot::Variant;
        using VariantType = ::godot::Variant::Type;

        int64_t variant_as_i64(const Variant &variant);

        rust::string variant_as_string(const Variant &variant);

        bool variant_as_bool(const Variant &variant);

        double variant_as_f64(const Variant &variant);

        const Variant &empty_variant();

        const Variant &variant_from_i64(int64_t value);

        const Variant &variant_from_string(rust::string value);

        const Variant &variant_from_bool(bool value);

        const Variant &variant_from_f64(double value);
    }
}
#endif //CXX_GODOT_VARIANT_H
