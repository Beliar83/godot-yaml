#!python
import os

target = ARGUMENTS.get("target", "debug").lower()
platform = ARGUMENTS.get("platform", "windows").lower()
bits = ARGUMENTS.get("bits", "64")

final_lib_path = 'demo/bin/'

# This makes sure to keep the session environment variables on windows, 
# that way you can run scons in a vs 2017 prompt and it will find all the required tools
env = Environment()
if platform == "windows":
    if bits == "64":
        env = Environment(ENV = os.environ, TARGET_ARCH='amd64')
    else:
        env = Environment(ENV = os.environ, TARGET_ARCH='x86')

def add_sources(sources, directory):
    for file in os.listdir(directory):
        if file.endswith('.cpp'):
            sources.append(directory + '/' + file)

if platform == "osx":
    env.Append(CCFLAGS = ['-g','-O3', '-arch', 'x86_64', '-std=c++14'])
    env.Append(LINKFLAGS = ['-arch', 'x86_64'])
    env.Append(LIBS=["libyaml-cpp"])
    final_lib_path = final_lib_path + 'osx/'

elif platform == "linux":
    env.Append(CCFLAGS = ['-fPIC', '-g','-O3', '-std=c++14'])
    env.Append(LIBS=["libyaml-cpp"])

    final_lib_path = final_lib_path + 'x11/'

elif platform == "windows":
    if target == "debug":
        env.Append(CCFLAGS = ['-EHsc', '-D_DEBUG', '-MDd'])
        env.Append(LIBS=["libyaml-cppmdd"])
    else:
        env.Append(LIBS=["libyaml-cppmd"])
        env.Append(CCFLAGS = ['-O2', '-EHsc', '-DNDEBUG', '-MD'])

    final_lib_path = final_lib_path + 'win' + str(bits) + '/'

env.Append(CPPPATH=['.', 'src/', 'godot-cpp/godot-headers/', 'godot-cpp/include/', 'godot-cpp/include/core', 'godot-cpp/include/gen', 'yaml-cpp/include/'])
env.Append(LIBPATH=["godot-cpp/bin", "yaml-cpp_install/lib"])
env.Append(LIBS=["libgodot-cpp" + "." + platform + "." + target + "." + str(bits)])

sources = []
add_sources(sources, "src")

library = env.SharedLibrary(target=final_lib_path + 'libgodotyaml', source=sources)
Default(library)
