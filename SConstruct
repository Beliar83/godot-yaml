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
    env.Append(LIBS=["libryml"])
    final_lib_path = final_lib_path + 'osx/'

elif platform == "linux":
    env.Append(CCFLAGS = ['-fPIC', '-g','-O3', '-std=c++14'])
    env.Append(LIBS=["libryml"])

    final_lib_path = final_lib_path + 'x11/'

elif platform == "windows":
    if target == "debug":
        env.Append(CCFLAGS = ['-EHsc', '-D_DEBUG', '-MDd'])
    else:
        env.Append(CCFLAGS = ['-O2', '-EHsc', '-DNDEBUG', '-MD'])
    env.Append(LIBS=["ryml"])

    final_lib_path = final_lib_path + 'win' + str(bits) + '/'
elif platform == "javascript":
    env["ENV"] = os.environ
    env["CC"] = "emcc"
    env["CXX"] = "em++"
    env["AR"] = "emar"
    env["RANLIB"] = "emranlib"
    env.Append(CPPFLAGS=["-s", "SIDE_MODULE=1"])
    env.Append(LINKFLAGS=["-s", "SIDE_MODULE=1"])
    env["SHOBJSUFFIX"] = ".bc"
    env["SHLIBSUFFIX"] = ".wasm"
    # Use TempFileMunge since some AR invocations are too long for cmd.exe.
    # Use POSIX-style paths, required with TempFileMunge.
    env["ARCOM_POSIX"] = env["ARCOM"].replace("$TARGET", "$TARGET.posix").replace("$SOURCES", "$SOURCES.posix")
    env["ARCOM"] = "${TEMPFILE(ARCOM_POSIX)}"

    # All intermediate files are just LLVM bitcode.
    env["OBJPREFIX"] = ""
    env["OBJSUFFIX"] = ".bc"
    env["PROGPREFIX"] = ""
    # Program() output consists of multiple files, so specify suffixes manually at builder.
    env["PROGSUFFIX"] = ""
    env["LIBPREFIX"] = "lib"
    env["LIBSUFFIX"] = ".a"
    env["LIBPREFIXES"] = ["$LIBPREFIX"]
    env["LIBSUFFIXES"] = ["$LIBSUFFIX"]
    env.Replace(SHLINKFLAGS='$LINKFLAGS')
    env.Replace(SHLINKFLAGS='$LINKFLAGS')
    env.Append(LIBS=["ryml"])

    if target == 'debug':
        env.Append(CCFLAGS=['-O0', '-g'])
    elif target == 'release':
        env.Append(CCFLAGS=['-O3'])
    bits='wasm'
    final_lib_path = final_lib_path + 'wasm/'

env.Append(CPPPATH=['.', 'src/', 'godot-cpp/godot-headers/', 'godot-cpp/include/', 'godot-cpp/include/core', 'godot-cpp/include/gen', 'rapidyaml-install/include'])
env.Append(LIBPATH=["godot-cpp/bin", "rapidyaml-install/lib"])
env.Append(LIBS=["libgodot-cpp" + "." + platform + "." + target + "." + str(bits)])

sources = []
add_sources(sources, "src")

library = env.SharedLibrary(target=final_lib_path + 'libgodotyaml', source=sources)
Default(library)
