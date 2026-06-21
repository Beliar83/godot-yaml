# Archival note
I have not worked on this for some time, and I also have not used yaml.
Because of that I will archive this repository

# Arm64
While I could not test it myself, here are instructions on how to build for MacOS with Arm64. 
Thanks to Jumpman (https://bsky.app/profile/jumpmanfr.bsky.social/post/3lf6j2jvbjt2c)

The SConsctruct needs to be updated to arm64, which it seems to be if using godot 3.6

<pre>
For compiling rapidyaml (from the build folder)
cmake .. -DCMAKE_OSX_ARCHITECTURES="x86_64;arm64"
make -j$(sysctl -n hw.ncpu)

And then on the main project, set "platform" to "osx" and:
cmake ..
make -j$(sysctl -n hw.ncpu)
scons -c
scons
</pre>
# godot-yaml
Library to parse from and print to yaml.

This is a native library that allows parsing to and reading from YAML files.

It uses rapidyaml (https://github.com/biojppm/rapidyaml).

# How to Build
- Apply patches/rapidyaml-Allow_stopping_of_parser.patch to rapidyaml
- Build and install rapidyaml (Sconstruct will also look in (godot-yaml)/yaml-cpp if you do not want it in a general location)
- Make sure you have pulled the godot-cpp module (with all its modules)
- run scons
