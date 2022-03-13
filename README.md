# godot-yaml
Library to parse from and print to yaml.

This is a native library that allows parsing to and reading from YAML files.

It uses rapidyaml (https://github.com/biojppm/rapidyaml).

# How to Build
- Apply patches/rapidyaml-Allow_stopping_of_parser.patch to rapidyaml
- Build and install rapidyaml (Sconstruct will also look in (godot-yaml)/yaml-cpp if you do not want it in a general location)
- Make sure you have pulled the godot-cpp module (with all its modules)
- run scons
