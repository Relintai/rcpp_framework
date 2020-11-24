#!/usr/bin/env python

EnsureSConsVersion(0, 98, 1)

# System
import glob
import os
import pickle
import sys
from collections import OrderedDict

# scan possible build platforms

platform_list = []  # list of platforms
platform_opts = {}  # options for each platform
platform_flags = {}  # flags for each platform

active_platforms = []
active_platform_ids = []
platform_exporters = []
platform_apis = []

custom_tools = ["default"]

platform_arg = ARGUMENTS.get("platform", ARGUMENTS.get("p", False))

if os.name == "nt" and (platform_arg == "android" or ARGUMENTS.get("use_mingw", False)):
    custom_tools = ["mingw"]
elif platform_arg == "javascript":
    # Use generic POSIX build toolchain for Emscripten.
    custom_tools = ["cc", "c++", "ar", "link", "textfile", "zip"]

env_base = Environment(tools=custom_tools)
if "TERM" in os.environ:
    env_base["ENV"]["TERM"] = os.environ["TERM"]
env_base.AppendENVPath("PATH", os.getenv("PATH"))
env_base.AppendENVPath("PKG_CONFIG_PATH", os.getenv("PKG_CONFIG_PATH"))
env_base.disabled_modules = []
env_base.use_ptrcall = False
env_base.module_version_string = ""
env_base.msvc = False

# avoid issues when building with different versions of python out of the same directory
env_base.SConsignFile(".sconsign{0}.dblite".format(pickle.HIGHEST_PROTOCOL))

# Build options


profile = ARGUMENTS.get("profile", False)
if profile:
    if os.path.isfile(profile):
        customs.append(profile)
    elif os.path.isfile(profile + ".py"):
        customs.append(profile + ".py")

opts = Variables(customs, ARGUMENTS)

# Target build options
opts.Add("arch", "Platform-dependent architecture (arm/arm64/x86/x64/mips/...)", "")
opts.Add(EnumVariable("bits", "Target platform bits", "default", ("default", "32", "64")))
opts.Add("p", "Platform (alias for 'platform')", "")
opts.Add("platform", "Target platform (%s)" % ("|".join(platform_list),), "")
opts.Add(EnumVariable("target", "Compilation target", "debug", ("debug", "release_debug", "release")))
opts.Add(EnumVariable("optimize", "Optimization type", "speed", ("speed", "size")))
opts.Add(BoolVariable("tools", "Build the tools (a.k.a. the Godot editor)", True))
opts.Add(BoolVariable("use_lto", "Use link-time optimization", False))
opts.Add(BoolVariable("use_precise_math_checks", "Math checks use very precise epsilon (debug option)", False))

# Compilation environment setup
opts.Add("CXX", "C++ compiler")
opts.Add("CC", "C compiler")
opts.Add("LINK", "Linker")
opts.Add("CCFLAGS", "Custom flags for both the C and C++ compilers")
opts.Add("CFLAGS", "Custom flags for the C compiler")
opts.Add("CXXFLAGS", "Custom flags for the C++ compiler")
opts.Add("LINKFLAGS", "Custom flags for the linker")

# add platform specific options

for k in platform_opts.keys():
    opt_list = platform_opts[k]
    for o in opt_list:
        opts.Add(o)

# Update the environment now as the "custom_modules" option may be
# defined in a file rather than specified via the command line.
opts.Update(env_base)

# Detect modules.
modules_detected = OrderedDict()
module_search_paths = ["modules"]  # Built-in path.

# Update the environment again after all the module options are added.
opts.Update(env_base)
Help(opts.GenerateHelpText(env_base))

# add default include paths

env_base.Prepend(CPPPATH=["#"])

# configure ENV for platform
env_base.platform_exporters = platform_exporters
env_base.platform_apis = platform_apis

env_base.platforms = {}

selected_platform = ""

if env_base["platform"] != "":
    selected_platform = env_base["platform"]
elif env_base["p"] != "":
    selected_platform = env_base["p"]
    env_base["platform"] = selected_platform
else:
    # Missing `platform` argument, try to detect platform automatically
    if sys.platform.startswith("linux"):
        selected_platform = "x11"
    elif sys.platform == "darwin":
        selected_platform = "osx"
    elif sys.platform == "win32":
        selected_platform = "windows"
    else:
        print("Could not detect platform automatically. Supported platforms:")
        for x in platform_list:
            print("\t" + x)
        print("\nPlease run SCons again and select a valid platform: platform=<string>")

    if selected_platform != "":
        print("Automatically detected platform: " + selected_platform)
        env_base["platform"] = selected_platform

if selected_platform in ["linux", "bsd", "linuxbsd"]:
    if selected_platform == "linuxbsd":
        # Alias for forward compatibility.
        print('Platform "linuxbsd" is still called "x11" in Godot 3.2.x. Building for platform "x11".')
    # Alias for convenience.
    selected_platform = "x11"
    env_base["platform"] = selected_platform

if selected_platform in platform_list:
    tmppath = "./platform/" + selected_platform
    sys.path.insert(0, tmppath)
    import detect

    if "create" in dir(detect):
        env = detect.create(env_base)
    else:
        env = env_base.Clone()

    # Compilation DB requires SCons 3.1.1+.
    from SCons import __version__ as scons_raw_version

    scons_ver = env._get_major_minor_revision(scons_raw_version)

    if scons_ver >= (4, 0, 0):
        env.Tool("compilation_db")
        env.Alias("compiledb", env.CompilationDatabase())

    if env["dev"]:
        env["verbose"] = True
        env["warnings"] = "extra"
        env["werror"] = True

    env.extra_suffix = ""

    if env["extra_suffix"] != "":
        env.extra_suffix += "." + env["extra_suffix"]

    # Environment flags
    CCFLAGS = env.get("CCFLAGS", "")
    env["CCFLAGS"] = ""
    env.Append(CCFLAGS=str(CCFLAGS).split())

    CFLAGS = env.get("CFLAGS", "")
    env["CFLAGS"] = ""
    env.Append(CFLAGS=str(CFLAGS).split())

    CXXFLAGS = env.get("CXXFLAGS", "")
    env["CXXFLAGS"] = ""
    env.Append(CXXFLAGS=str(CXXFLAGS).split())

    LINKFLAGS = env.get("LINKFLAGS", "")
    env["LINKFLAGS"] = ""
    env.Append(LINKFLAGS=str(LINKFLAGS).split())

    # Platform specific flags
    flag_list = platform_flags[selected_platform]
    for f in flag_list:
        if not (f[0] in ARGUMENTS):  # allow command line to override platform flags
            env[f[0]] = f[1]

    # Must happen after the flags definition, so that they can be used by platform detect
    detect.configure(env)

    # Set our C and C++ standard requirements.
    # Prepending to make it possible to override
    # This needs to come after `configure`, otherwise we don't have env.msvc.
    if not env.msvc:
        # Specifying GNU extensions support explicitly, which are supported by
        # both GCC and Clang. This mirrors GCC and Clang's current default
        # compile flags if no -std is specified.
        env.Prepend(CFLAGS=["-std=gnu11"])
        env.Prepend(CXXFLAGS=["-std=gnu++14"])
    else:
        # MSVC doesn't have clear C standard support, /std only covers C++.
        # We apply it to CCFLAGS (both C and C++ code) in case it impacts C features.
        env.Prepend(CCFLAGS=["/std:c++14"])

    # Configure compiler warnings
    if env.msvc:
        # Truncations, narrowing conversions, signed/unsigned comparisons...
        disable_nonessential_warnings = ["/wd4267", "/wd4244", "/wd4305", "/wd4018", "/wd4800"]
        if env["warnings"] == "extra":
            env.Append(CCFLAGS=["/Wall"])  # Implies /W4
        elif env["warnings"] == "all":
            env.Append(CCFLAGS=["/W3"] + disable_nonessential_warnings)
        elif env["warnings"] == "moderate":
            env.Append(CCFLAGS=["/W2"] + disable_nonessential_warnings)
        else:  # 'no'
            env.Append(CCFLAGS=["/w"])
        # Set exception handling model to avoid warnings caused by Windows system headers.
        env.Append(CCFLAGS=["/EHsc"])
        if env["werror"]:
            env.Append(CCFLAGS=["/WX"])
        # Force to use Unicode encoding
        env.Append(MSVC_FLAGS=["/utf8"])
    else:  # Rest of the world
        version = methods.get_compiler_version(env) or [-1, -1]

        shadow_local_warning = []
        all_plus_warnings = ["-Wwrite-strings"]

        if methods.using_gcc(env):
            env.Append(CCFLAGS=["-Wno-misleading-indentation"])
            if version[0] >= 7:
                shadow_local_warning = ["-Wshadow-local"]

        if env["warnings"] == "extra":
            # Note: enable -Wimplicit-fallthrough for Clang (already part of -Wextra for GCC)
            # once we switch to C++11 or later (necessary for our FALLTHROUGH macro).
            env.Append(CCFLAGS=["-Wall", "-Wextra", "-Wno-unused-parameter"] + all_plus_warnings + shadow_local_warning)
            env.Append(CXXFLAGS=["-Wctor-dtor-privacy", "-Wnon-virtual-dtor"])
            if methods.using_gcc(env):
                env.Append(
                    CCFLAGS=[
                        "-Walloc-zero",
                        "-Wduplicated-branches",
                        "-Wduplicated-cond",
                        "-Wstringop-overflow=4",
                        "-Wlogical-op",
                    ]
                )
                env.Append(CXXFLAGS=["-Wnoexcept", "-Wplacement-new=1"])
                if version[0] >= 9:
                    env.Append(CCFLAGS=["-Wattribute-alias=2"])
        elif env["warnings"] == "all":
            env.Append(CCFLAGS=["-Wall"] + shadow_local_warning)
        elif env["warnings"] == "moderate":
            env.Append(CCFLAGS=["-Wall", "-Wno-unused"] + shadow_local_warning)
        else:  # 'no'
            env.Append(CCFLAGS=["-w"])
        if env["werror"]:
            env.Append(CCFLAGS=["-Werror"])
        else:  # always enable those errors
            env.Append(CCFLAGS=["-Werror=return-type"])

    if hasattr(detect, "get_program_suffix"):
        suffix = "." + detect.get_program_suffix()
    else:
        suffix = "." + selected_platform

    if env["target"] == "release":
        if env["tools"]:
            print("Tools can only be built with targets 'debug' and 'release_debug'.")
            sys.exit(255)
        suffix += ".opt"
        env.Append(CPPDEFINES=["NDEBUG"])

    elif env["target"] == "release_debug":
        if env["tools"]:
            suffix += ".opt.tools"
        else:
            suffix += ".opt.debug"
    else:
        if env["tools"]:
            suffix += ".tools"
        else:
            suffix += ".debug"

    if env["arch"] != "":
        suffix += "." + env["arch"]
    elif env["bits"] == "32":
        suffix += ".32"
    elif env["bits"] == "64":
        suffix += ".64"

    suffix += env.extra_suffix

    sys.path.remove(tmppath)
    sys.modules.pop("detect")

    modules_enabled = OrderedDict()
    env.module_icons_paths = []
    env.doc_class_path = {}

    methods.update_version(env.module_version_string)

    env["PROGSUFFIX"] = suffix + env.module_version_string + env["PROGSUFFIX"]
    env["OBJSUFFIX"] = suffix + env["OBJSUFFIX"]
    # (SH)LIBSUFFIX will be used for our own built libraries
    # LIBSUFFIXES contains LIBSUFFIX and SHLIBSUFFIX by default,
    # so we need to append the default suffixes to keep the ability
    # to link against thirdparty libraries (.a, .so, .lib, etc.).
    if os.name == "nt":
        # On Windows, only static libraries and import libraries can be
        # statically linked - both using .lib extension
        env["LIBSUFFIXES"] += [env["LIBSUFFIX"]]
    else:
        env["LIBSUFFIXES"] += [env["LIBSUFFIX"], env["SHLIBSUFFIX"]]
    env["LIBSUFFIX"] = suffix + env["LIBSUFFIX"]
    env["SHLIBSUFFIX"] = suffix + env["SHLIBSUFFIX"]

    if not env["verbose"]:
        methods.no_verbose(sys, env)

    scons_cache_path = os.environ.get("SCONS_CACHE")
    if scons_cache_path != None:
        CacheDir(scons_cache_path)
        print("Scons cache enabled... (path: '" + scons_cache_path + "')")

    if env["vsproj"]:
        env.vs_incs = []
        env.vs_srcs = []

    Export("env")

    # build subdirs, the build order is dependent on link order.

    # Microsoft Visual Studio Project Generation
    if env["vsproj"]:
        env["CPPPATH"] = [Dir(path) for path in env["CPPPATH"]]
        methods.generate_vs_project(env, GetOption("num_jobs"))
        methods.generate_cpp_hint_file("cpp.hint")

    # Check for the existence of headers
    conf = Configure(env)
    if "check_c_headers" in env:
        for header in env["check_c_headers"]:
            if conf.CheckCHeader(header[0]):
                env.AppendUnique(CPPDEFINES=[header[1]])

elif selected_platform != "":
    if selected_platform == "list":
        print("The following platforms are available:\n")
    else:
        print('Invalid target platform "' + selected_platform + '".')
        print("The following platforms were detected:\n")

    for x in platform_list:
        print("\t" + x)

    print("\nPlease run SCons again and select a valid platform: platform=<string>")

    if selected_platform == "list":
        # Exit early to suppress the rest of the built-in SCons messages
        sys.exit(0)
    else:
        sys.exit(255)

# The following only makes sense when the 'env' is defined, and assumes it is.
if "env" in locals():
    methods.show_progress(env)
    # TODO: replace this with `env.Dump(format="json")`
    # once we start requiring SCons 4.0 as min version.
    methods.dump(env)
