#!python
import os
import sys
import subprocess

# Workaround for MinGW. See:
# http://www.scons.org/wiki/LongCmdLinesOnWin32
if os.name == "nt":
    import subprocess

    def mySubProcess(cmdline,env):
        #print "SPAWNED : " + cmdline
        startupinfo = subprocess.STARTUPINFO()
        startupinfo.dwFlags |= subprocess.STARTF_USESHOWWINDOW
        proc = subprocess.Popen(cmdline, stdin=subprocess.PIPE, stdout=subprocess.PIPE,
            stderr=subprocess.PIPE, startupinfo=startupinfo, shell = False, env = env)
        data, err = proc.communicate()
        rv = proc.wait()
        if rv:
            print("=====")
            print(err.decode("utf-8"))
            print("=====")
        return rv

    def mySpawn(sh, escape, cmd, args, env):

        newargs = ' '.join(args[1:])
        cmdline = cmd + " " + newargs

        rv=0
        if len(cmdline) > 32000 and cmd.endswith("ar") :
            cmdline = cmd + " " + args[1] + " " + args[2] + " "
            for i in range(3,len(args)) :
                rv = mySubProcess( cmdline + args[i], env )
                if rv :
                    break
        else:
            rv = mySubProcess( cmdline, env )

        return rv

# Try to detect the host platform automatically.
# This is used if no `platform` argument is passed
if sys.platform.startswith('linux'):
    host_platform = 'linux'
elif sys.platform.startswith('freebsd'):
    host_platform = 'freebsd'
elif sys.platform == 'darwin':
    host_platform = 'osx'
elif sys.platform == 'win32' or sys.platform == 'msys':
    host_platform = 'windows'
else:
    raise ValueError(
        'Could not detect platform automatically, please specify with '
        'platform=<platform>'
    )

libpath = 'demo/bin/'

env = Environment(ENV = os.environ)

is64 = sys.maxsize > 2**32
if (
    env['TARGET_ARCH'] == 'amd64' or
    env['TARGET_ARCH'] == 'emt64' or
    env['TARGET_ARCH'] == 'x86_64' or
    env['TARGET_ARCH'] == 'arm64-v8a'
):
    is64 = True

opts = Variables([], ARGUMENTS)
opts.Add(EnumVariable(
    'platform',
    'Target platform',
    host_platform,
    allowed_values=('linux', 'freebsd', 'osx', 'windows', 'android', 'ios', 'javascript'),
    ignorecase=2
))
opts.Add(EnumVariable(
    'bits',
    'Target platform bits',
    '64' if is64 else '32',
    ('32', '64')
))
opts.Add(BoolVariable(
    'use_llvm',
    'Use the LLVM compiler - only effective when targeting Linux or FreeBSD',
    False
))
opts.Add(BoolVariable(
    'use_mingw',
    'Use the MinGW compiler instead of MSVC - only effective on Windows',
    False
))
# Must be the same setting as used for cpp_bindings
opts.Add(EnumVariable(
    'target',
    'Compilation target',
    'debug',
    allowed_values=('debug', 'release'),
    ignorecase=2
))

opts.Add(EnumVariable(
    'android_arch',
    'Target Android architecture',
    'armv7',
    ['armv7','arm64v8','x86','x86_64']
))
opts.Add(
    'macos_deployment_target',
    'macOS deployment target',
    'default'
)
opts.Add(
    'macos_sdk_path',
    'macOS SDK path',
    ''
)
opts.Add(EnumVariable(
    'macos_arch',
    'Target macOS architecture',
    'universal',
    ['universal', 'x86_64', 'arm64']
))
opts.Add(EnumVariable(
    'ios_arch',
    'Target iOS architecture',
    'arm64',
    ['armv7', 'arm64', 'x86_64']
))
opts.Add(BoolVariable(
    'ios_simulator',
    'Target iOS Simulator',
    False
))
opts.Add(
    'IPHONEPATH',
    'Path to iPhone toolchain',
    '/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain',
)
opts.Add(
    'android_api_level',
    'Target Android API level',
    '18' if ARGUMENTS.get("android_arch", 'armv7') in ['armv7', 'x86'] else '21'
)
opts.Add(
    'ANDROID_NDK_ROOT',
    'Path to your Android NDK installation. By default, uses ANDROID_NDK_ROOT from your defined environment variables.',
    os.environ.get("ANDROID_NDK_ROOT", None)
)

opts.Update(env)
Help(opts.GenerateHelpText(env))


process = subprocess.run("rustup toolchain list", capture_output=True, text=True, shell=True)
process.check_returncode()
lines = process.stdout.splitlines()
channel = None
for line in lines:
    if line.endswith('(default)'):
        if line.startswith('stable'):
            channel = 'stable'
            break
        elif line.startswith('nightly'):
            channel = 'nightly'
            break
if channel is None:
    print('No toolchain detected. Installing stable toolchain')
    channel = 'stable'

command = 'cargo build --manifest-path=./Cargo.toml'
rust_libpath = './godot_yaml_rust/target'

if env['bits'] == '64':
    rust_target_arch = 'x86_64'
else:
    rust_target_arch = 'i686'

# This makes sure to keep the session environment variables on windows,
# that way you can run scons in a vs 2017 prompt and it will find all the required tools
if host_platform == 'windows' and env['platform'] != 'android':
    if env['bits'] == '64':
        env = Environment(TARGET_ARCH='amd64')
    elif env['bits'] == '32':
        env = Environment(TARGET_ARCH='x86')

    opts.Update(env)

def add_sources(sources, directory):
    for file in os.listdir(directory):
        if file.endswith('.cpp'):
            sources.append(directory + '/' + file)

if env['platform'] == 'linux' or env['platform'] == 'freebsd':
    if env['use_llvm']:
        env['CXX'] = 'clang++'

    process = subprocess.run('rustup target add ' + rust_target_arch + '-unknown-linux-gnu', shell=True)
    process.check_returncode()
    process = subprocess.run('rustup default ' + channel + '-' + rust_target_arch + '-unknown-linux-gnu', shell=True)
    process.check_returncode()

    env.Append(CCFLAGS=['-fPIC', '-std=c++14', '-Wwrite-strings'])
    env.Append(LINKFLAGS=["-Wl,-R,'$$ORIGIN'"])

    if env['target'] == 'debug':
        env.Append(CCFLAGS=['-Og', '-g'])
        rust_libpath += '/debug'
    elif env['target'] == 'release':
        command = command + ' --release'
        rust_libpath += '/release'
        env.Append(CCFLAGS=['-O3'])

    if env['bits'] == '64':
        env.Append(CCFLAGS=['-m64'])
        env.Append(LINKFLAGS=['-m64'])
    elif env['bits'] == '32':
        env.Append(CCFLAGS=['-m32'])
        env.Append(LINKFLAGS=['-m32'])

    libpath = libpath + 'x11/'
    env.Append(LIBPATH=[os.path.abspath(rust_libpath)])
    env.Append(LIBS=['godot_yaml_rust'])

elif env['platform'] == 'osx':
    if env['bits'] == '32':
        raise ValueError(
            'Only 64-bit builds are supported for the macOS target.'
        )

    rust_mac_arch = ''
    if env["macos_arch"] == "universal":
        rust_mac_arch = 'aarch64-apple-darwin'
        env.Append(LINKFLAGS=["-arch", "x86_64", "-arch", "arm64"])
        env.Append(CCFLAGS=["-arch", "x86_64", "-arch", "arm64"])
    else:
        rust_mac_arch = 'x86_64-apple-darwin'
        env.Append(LINKFLAGS=["-arch", env["macos_arch"]])
        env.Append(CCFLAGS=["-arch", env["macos_arch"]])

    process = subprocess.run('rustup target add ' + rust_mac_arch, shell=True)
    process.check_returncode()
    process = subprocess.run('rustup default ' + channel + '-' + rust_mac_arch, shell=True)
    process.check_returncode()

    env.Append(CCFLAGS=['-std=c++14'])

    if env['macos_deployment_target'] != 'default':
        env.Append(CCFLAGS=['-mmacosx-version-min=' + env['macos_deployment_target']])
        env.Append(LINKFLAGS=['-mmacosx-version-min=' + env['macos_deployment_target']])

    if env['macos_sdk_path']:
        env.Append(CCFLAGS=['-isysroot', env['macos_sdk_path']])
        env.Append(LINKFLAGS=['-isysroot', env['macos_sdk_path']])

    env.Append(LINKFLAGS=[
        '-framework',
        'Cocoa',
        '-Wl,-undefined,dynamic_lookup',
    ])

    if env['target'] == 'debug':
        env.Append(CCFLAGS=['-Og', '-g'])
        rust_libpath += '/debug'
    elif env['target'] == 'release':
        env.Append(CCFLAGS=['-O3'])
        command = command + ' --release'
        rust_libpath += '/release'

    libpath = libpath + 'osx/'
    env.Append(LIBPATH=[os.path.abspath(rust_libpath)])
    env.Append(LIBS=['godot_yaml_rust'])

elif env['platform'] == 'ios':
    rust_ios_arch = ''
    if env['ios_simulator']:
        if env['ios_arch'] == 'arm64':
            rust_ios_arch = 'aarch64-apple-ios-sim'
        else:
            rust_ios_arch = 'x86_64-apple-ios'
        sdk_name = 'iphonesimulator'
        env.Append(CCFLAGS=['-mios-simulator-version-min=10.0'])
        env['LIBSUFFIX'] = ".simulator" + env['LIBSUFFIX']
    else:
        if env['ios_arch'] == 'arm64':
            rust_ios_arch = 'aarch64-apple-ios'
        else:
            rust_ios_arch = 'x86_64-apple-ios'
        sdk_name = 'iphoneos'
        env.Append(CCFLAGS=['-miphoneos-version-min=10.0'])

    process = subprocess.run('rustup target add ' + rust_ios_arch, shell=True)
    process.check_returncode()
    process = subprocess.run('rustup default ' + channel + '-' + rust_ios_arch, shell=True)
    process.check_returncode()

    try:
        sdk_path = decode_utf8(subprocess.check_output(['xcrun', '--sdk', sdk_name, '--show-sdk-path']).strip())
    except (subprocess.CalledProcessError, OSError):
        raise ValueError("Failed to find SDK path while running xcrun --sdk {} --show-sdk-path.".format(sdk_name))

    compiler_path = env['IPHONEPATH'] + '/usr/bin/'
    env['ENV']['PATH'] = env['IPHONEPATH'] + "/Developer/usr/bin/:" + env['ENV']['PATH']

    env['CC'] = compiler_path + 'clang'
    env['CXX'] = compiler_path + 'clang++'
    env['AR'] = compiler_path + 'ar'
    env['RANLIB'] = compiler_path + 'ranlib'

    env.Append(CCFLAGS=['-std=c++14', '-arch', env['ios_arch'], '-isysroot', sdk_path])
    env.Append(LINKFLAGS=[
        '-arch',
        env['ios_arch'],
        '-framework',
        'Cocoa',
        '-Wl,-undefined,dynamic_lookup',
        '-isysroot', sdk_path,
        '-F' + sdk_path
    ])

    if env['target'] == 'debug':
        env.Append(CCFLAGS=['-Og', '-g'])
        rust_libpath += '/debug'
    elif env['target'] == 'release':
        env.Append(CCFLAGS=['-O3'])
        command = command + ' --release'
        rust_libpath += '/release'
    libpath = libpath + 'ios/'
    env.Append(LIBPATH=[os.path.abspath(rust_libpath)])
    env.Append(LIBS=['godot_yaml_rust'])

elif env['platform'] == 'windows':
    print('Installing and/or selecting rust msvc toolchain')

    env.Append(LINKFLAGS=['/WX'])

    process = subprocess.run('rustup target add ' + rust_target_arch + '-pc-windows-msvc', shell=True)
    process.check_returncode()
    process = subprocess.run('rustup default ' + channel + '-' + rust_target_arch + '-pc-windows-msvc', shell=True)
    process.check_returncode()

    env.Append(LINKFLAGS=['/WX'])

    if host_platform == 'windows' and not env['use_mingw']:
        # MSVC
        env.Append(LINKFLAGS=['/WX'])
        if env['target'] == 'debug':
            env.Append(CCFLAGS=['/Z7', '/Od', '/EHsc', '/D_DEBUG', '/MDd'])
            rust_libpath += '/debug'
        elif env['target'] == 'release':
            command = command + ' --release'
            rust_libpath += '/release'
            env.Append(CCFLAGS=['/O2', '/EHsc', '/DNDEBUG', '/MD'])
        env.Append(LIBS=[File(rust_libpath + '/godot_yaml_rust.lib')])
        env.Append(LINKFLAGS=['userenv.lib'])

    elif host_platform == 'linux' or host_platform == 'freebsd' or host_platform == 'osx':
        # Cross-compilation using MinGW
        if env['bits'] == '64':
            env['CXX'] = 'x86_64-w64-mingw32-g++'
            env['AR'] = "x86_64-w64-mingw32-ar"
            env['RANLIB'] = "x86_64-w64-mingw32-ranlib"
            env['LINK'] = "x86_64-w64-mingw32-g++"
        elif env['bits'] == '32':
            env['CXX'] = 'i686-w64-mingw32-g++'
            env['AR'] = "i686-w64-mingw32-ar"
            env['RANLIB'] = "i686-w64-mingw32-ranlib"
            env['LINK'] = "i686-w64-mingw32-g++"
        env.Append(LIBPATH=[os.path.abspath(rust_libpath)])
        env.Append(LIBS=['godot_yaml_rust'])
        env.Append(LIBS=['userenv'])

    elif host_platform == 'windows' and env['use_mingw']:
        # Don't Clone the environment. Because otherwise, SCons will pick up msvc stuff.
        env = Environment(ENV = os.environ, tools=["mingw"])
        opts.Update(env)
        #env = env.Clone(tools=['mingw'])
        env.Append(LIBPATH=[os.path.abspath(rust_libpath)])
        env.Append(LIBS=['godot_yaml_rust'])

        env["SPAWN"] = mySpawn
    if env['target'] == 'debug':
        env.Append(CCFLAGS=['/Z7', '/Od', '/EHsc', '/D_DEBUG', '/MDd'])
        rust_libpath += '/debug'
    elif env['target'] == 'release':
        command = command + ' --release'
        rust_libpath += '/release'
        env.Append(CCFLAGS=['/O2', '/EHsc', '/DNDEBUG', '/MD'])


    libpath = libpath + 'win' + str(env['bits']) + '/'
elif env['platform'] == 'android':
    if host_platform == 'windows':
        # Don't Clone the environment. Because otherwise, SCons will pick up msvc stuff.
        env = Environment(ENV = os.environ, tools=["mingw"])
        opts.Update(env)
        #env = env.Clone(tools=['mingw'])

        env["SPAWN"] = mySpawn

    # Verify NDK root
    if not 'ANDROID_NDK_ROOT' in env:
        raise ValueError("To build for Android, ANDROID_NDK_ROOT must be defined. Please set ANDROID_NDK_ROOT to the root folder of your Android NDK installation.")

    # Validate API level
    api_level = int(env['android_api_level'])
    if env['android_arch'] in ['x86_64', 'arm64v8'] and api_level < 21:
        print("WARN: 64-bit Android architectures require an API level of at least 21; setting android_api_level=21")
        env['android_api_level'] = '21'
        api_level = 21

    # Setup toolchain
    toolchain = env['ANDROID_NDK_ROOT'] + "/toolchains/llvm/prebuilt/"
    if host_platform == "windows":
        toolchain += "windows"
        import platform as pltfm
        if pltfm.machine().endswith("64"):
            toolchain += "-x86_64"
            rust_and_arch = 'x86_64-linux-android'
        else:
            rust_and_arch = 'i686-linux-android'
    elif host_platform == "linux":
        toolchain += "linux-x86_64"
        rust_and_arch = 'x86_64-linux-android'
    elif host_platform == "osx":
        toolchain += "darwin-x86_64"
        rust_and_arch = 'x86_64-apple-darwin'
    env.PrependENVPath('PATH', toolchain + "/bin") # This does nothing half of the time, but we'll put it here anyways

    # Get architecture info
    arch_info_table = {
        "armv7" : {
            "march":"armv7-a", "target":"armv7a-linux-androideabi", "tool_path":"arm-linux-androideabi", "compiler_path":"armv7a-linux-androideabi",
            "ccflags" : ['-mfpu=neon']
            },
        "arm64v8" : {
            "march":"armv8-a", "target":"aarch64-linux-android", "tool_path":"aarch64-linux-android", "compiler_path":"aarch64-linux-android",
            "ccflags" : []
            },
        "x86" : {
            "march":"i686", "target":"i686-linux-android", "tool_path":"i686-linux-android", "compiler_path":"i686-linux-android",
            "ccflags" : ['-mstackrealign']
            },
        "x86_64" : {"march":"x86-64", "target":"x86_64-linux-android", "tool_path":"x86_64-linux-android", "compiler_path":"x86_64-linux-android",
            "ccflags" : []
        }
    }
    arch_info = arch_info_table[env['android_arch']]

    # Setup tools
    env['CC'] = toolchain + "/bin/clang"
    env['CXX'] = toolchain + "/bin/clang++"
    env['AR'] = toolchain + "/bin/" + arch_info['tool_path'] + "-ar"
    env["AS"] = toolchain + "/bin/" + arch_info['tool_path'] + "-as"
    env["LD"] = toolchain + "/bin/" + arch_info['tool_path'] + "-ld"
    env["STRIP"] = toolchain + "/bin/" + arch_info['tool_path'] + "-strip"
    env["RANLIB"] = toolchain + "/bin/" + arch_info['tool_path'] + "-ranlib"

    env.Append(CCFLAGS=['--target=' + arch_info['target'] + env['android_api_level'], '-march=' + arch_info['march'], '-fPIC'])
    env.Append(CCFLAGS=arch_info['ccflags'])

    if env['target'] == 'debug':
        env.Append(CCFLAGS=['-Og', '-g'])
        rust_libpath += '/debug'
    elif env['target'] == 'release':
        env.Append(CCFLAGS=['-O3'])
        command = command + ' --release'
        rust_libpath += '/release'
    env.Append(LIBPATH=[os.path.abspath(rust_libpath)])
    env.Append(LIBS=['godot_yaml_rust'])
    process = subprocess.run('rustup target add ' + rust_and_arch, shell=True)
    process.check_returncode()
    process = subprocess.run('rustup default ' + channel + '-' + rust_and_arch, shell=True)
    process.check_returncode()


elif env['platform'] == "javascript":
    print('summator_rust: installing emscripen target for rust')
    process = subprocess.run('rustup target add wasm32-unknown-emscripten', shell=True)
    process.check_returncode()
    rust_libpath += '/wasm32-unknown-emscripten'
    command += ' --target=wasm32-unknown-emscripten'
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

    if env['target'] == 'debug':
        env.Append(CCFLAGS=['-O0', '-g'])
        rust_libpath += '/debug'
    elif env['target'] == 'release':
        env.Append(CCFLAGS=['-O3'])
        command = command + ' --release'
        rust_libpath += '/release'
    env.Append(CCFLAGS = ['-fPIC', '-std=c++14'])
    libpath = libpath + 'wasm/'
    bits='wasm'
    env.Append(LIBPATH=[os.path.abspath(rust_libpath)])
    print('LIBPATH: ' + os.path.abspath(rust_libpath))
    env.Append(LIBS=['godot_yaml_rust'])

rust_env = {
    **os.environ,
    'GODOT_PLATFORM': env['platform'],
    'EMCC_CFLAGS': str.join(' ', env['CPPFLAGS']),
}

old_dir = os.getcwd()
os.chdir('godot_yaml_rust')

print('godot-component-system: Compiling rust library')
process = subprocess.run(command , shell=True, env=rust_env)
process.check_returncode()
os.chdir(old_dir)

env.Append(CPPPATH=['.', 'src/', 'godot-cpp/godot-headers/', 'godot-cpp/include/', 'godot-cpp/include/core', 'godot-cpp/include/gen'])
env.Append(LIBPATH=['godot-cpp/bin'])
env.Append(LIBS=['libgodot-cpp' + '.' + env['platform'] + '.' + env['target'] + '.' + str(env['bits'])])

sources = []
add_sources(sources, 'src')

library = env.SharedLibrary(target=libpath + 'libgodotyaml', source=sources)
Default(library)
