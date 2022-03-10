use crate::godot::variant::ffi::Variant;
use cxx::{type_id, ExternType};
use godot::variant::VariantType;

#[cxx::bridge(namespace = godot::ffi)]
pub mod ffi {
    unsafe extern "C++" {
        include!("godot_yaml_rust/include/godot/variant.h");
        include!("rust/cxx.h");
        pub type Variant;
        #[cxx_name = "VariantType"]
        pub type CXXVariantType = crate::godot::variant::CXXVariantType;

        pub fn get_type(self: &Variant) -> CXXVariantType;

        pub fn variant_as_i64(variant: &Variant) -> i64;
        pub fn variant_as_string(variant: &Variant) -> String;
        pub fn variant_as_bool(variant: &Variant) -> bool;
        pub fn variant_as_f64(variant: &Variant) -> f64;
    }
}

#[derive(Hash, Eq, PartialEq, Clone, Default)]
pub struct CXXVariantType(pub(crate) VariantType);

unsafe impl ExternType for CXXVariantType {
    type Id = type_id!("godot::ffi::VariantType");
    type Kind = cxx::kind::Trivial;
}

impl From<&ffi::Variant> for i64 {
    fn from(variant: &Variant) -> Self {
        ffi::variant_as_i64(variant)
    }
}

impl From<&ffi::Variant> for String {
    fn from(variant: &Variant) -> Self {
        ffi::variant_as_string(variant)
    }
}

impl From<&ffi::Variant> for bool {
    fn from(variant: &Variant) -> Self {
        ffi::variant_as_bool(variant)
    }
}

impl From<&ffi::Variant> for f64 {
    fn from(variant: &Variant) -> Self {
        ffi::variant_as_f64(variant)
    }
}
