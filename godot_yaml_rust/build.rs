use std::env;
use std::path::Path;

fn main() {
    cxx_build::bridges(vec![
        "src/godot/variant.rs",
    ])
    .include(Path::new("include"))
    .include(Path::new("../godot-cpp/include"))
    .include(Path::new("../godot-cpp/include/gen"))
    .include(Path::new("../godot-cpp/include/core"))
    .include(Path::new("../godot-cpp/godot-headers"))
    .files(vec!["src/godot/string.cpp", "src/godot/variant.cpp"])
    .flag_if_supported("-std=g++14")
    .define("RUST_CXX_NO_EXCEPTIONS", None)
    .compile("godot-yaml-rust");

    println!("cargo:rerun-if-changed=src/godot/variant.rs");
}
