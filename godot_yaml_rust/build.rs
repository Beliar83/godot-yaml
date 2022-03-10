use std::env;
use std::path::Path;

fn main() {
    let platform = env::var("GODOT_PLATFORM");

    let platform = match platform {
        Ok(platform) => platform,
        Err(err) => {
            panic!("Could not read GODOT_PLATFORM: {}", err)
        }
    };

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
    .compile("godot-component-system");

    println!("cargo:rerun-if-changed=src/godot/variant.rs");
}
