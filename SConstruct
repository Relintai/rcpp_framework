#!/usr/bin/env python

EnsureSConsVersion(0, 98, 1)

# System
import glob
import os
import pickle
import sys

import re
import subprocess
from collections import OrderedDict
#from compat import iteritems, isbasestring, open_utf8, decode_utf8, qualname

from SCons import Node
from SCons.Script import Glob

def isbasestring(s):
    return isinstance(s, (str, bytes))

def add_source_files(self, sources, files, warn_duplicates=True):
    # Convert string to list of absolute paths (including expanding wildcard)
    if isbasestring(files):
        # Keep SCons project-absolute path as they are (no wildcard support)
        if files.startswith("#"):
            if "*" in files:
                print("ERROR: Wildcards can't be expanded in SCons project-absolute path: '{}'".format(files))
                return
            files = [files]
        else:
            dir_path = self.Dir(".").abspath
            files = sorted(glob.glob(dir_path + "/" + files))

    # Add each path as compiled Object following environment (self) configuration
    for path in files:
        obj = self.Object(path)
        if obj in sources:
            if warn_duplicates:
                print('WARNING: Object "{}" already included in environment sources.'.format(obj))
            else:
                continue
        sources.append(obj)

def add_library(env, name, sources, **args):
    library = env.Library(name, sources, **args)
    env.NoCache(library)
    return library

def add_program(env, name, sources, **args):
    program = env.Program(name, sources, **args)
    env.NoCache(program)
    return program

# Based on https://github.com/godotengine/godot/pull/53443
# uses the graph/topo sort from:
# https://www.geeksforgeeks.org/python-program-for-topological-sorting/
# albeit highly modified
class ModuleDepGraph:
    def __init__(self, modules, dependencies: OrderedDict):
        self.graph = dict()

        v = []

        for m in modules:
            v.append(m[0])

        self.vertices = v

        # construct the edges
        for name, deps in dependencies.items():
            self.graph[name] = []
            for dep_name in deps:
                self.graph[name].append(dep_name)

    # A recursive function used by dependency_sort
    def topological_sort_util(self, v, visited, stack):

        # Mark the current node as visited.
        visited[v] = True

        # Recur for all the vertices adjacent to this vertex
        for i in self.graph[v]:
            if i in visited and visited[i] == False:
                self.topological_sort_util(i, visited, stack)

        # Push current vertex to stack which stores result
        stack.insert(0, v)

    # The function to performs a topological sort, and then reverses it to obtain the dependency sort.
    def dependency_sort(self):# -> []:
        # Mark all the vertices as not visited
        visited = dict()
        for v in self.vertices:
            visited[v] = False

        stack = []

        # Call the recursive helper function to store Topological
        # Sort starting from all vertices one by one
        for v in self.vertices:
            if visited[v] == False:
                self.topological_sort_util(v, visited, stack)

        # reverse the topological sort
        stack.reverse()

        return stack

def sort_modules_dependencies(modules):
    deps = {}
    for mod in modules:
        name = mod[0]
        path = mod[1]

        sys.path.insert(0, path)
        import detect

        try:
            deps[name] = detect.get_module_dependencies()
        except AttributeError:
            deps[name] = {}
        sys.path.remove(path)
        sys.modules.pop("detect")

    graph = ModuleDepGraph(modules, deps)
    dep_sorted_names = graph.dependency_sort()

    for n in dep_sorted_names:
        for i in range(len(modules)):
            if modules[i][0] == n:

                mt = modules[i]

                for j in range(i + 1, len(modules)):
                    modules[j - 1] = modules[j]

                modules[len(modules) - 1] = mt

                break

    #return modules

env_base = Environment()

env_base.__class__.add_source_files = add_source_files
env_base.__class__.add_library = add_library
env_base.__class__.add_program = add_program

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

opts = Variables([], ARGUMENTS)

opts.Add(EnumVariable("target", "Compilation target", "debug", ("debug", "release_debug", "release")))

opts.Add("folders", "App folders to compile", "")
opts.Add("main_file", "The main file", "")
opts.Add("module_folders", "App module folders to compile", "")
opts.Add("databases", "Whether to have database support", False)

# Compilation environment setup
opts.Add("CXX", "C++ compiler")
opts.Add("CC", "C compiler")
opts.Add("LINK", "Linker")
opts.Add("CCFLAGS", "Custom flags for both the C and C++ compilers")
opts.Add("CFLAGS", "Custom flags for the C compiler")
opts.Add("CXXFLAGS", "Custom flags for the C++ compiler")
opts.Add("LINKFLAGS", "Custom flags for the linker")

opts.Update(env_base)

# add default include paths
env_base.Prepend(CPPPATH=["#"])
env_base.Prepend(CPPPATH=["#libs"])


env_base.Prepend(LINKFLAGS=["-lpthread"])

#env_base.Append(CXX=["-o3"])
env_base.Append(CXX=["-g"])
#env_base.Append(CXX=["-g2"])
#env_base.Append(CXX=["-fno-rtti"])

# Compilation DB requires SCons 3.1.1+.
from SCons import __version__ as scons_raw_version

scons_ver = env_base._get_major_minor_revision(scons_raw_version)

if scons_ver >= (4, 0, 0):
    env_base.Tool("compilation_db")

database_list = []

if env_base["databases"]:
    env_base.Append(CPPDEFINES=["DATABASES_ENABLED"])

    for x in sorted(glob.glob("database/*")):
        if not os.path.isdir(x) or not os.path.exists(x + "/detect.py"):
            continue
        tmppath = "./" + x

        sys.path.insert(0, tmppath)
        import detect

        if detect.is_active() and detect.can_build():
            x = x.replace("database/", "")  # rest of world
            x = x.replace("database\\", "")  # win32
            database_list += [x]

        sys.path.remove(tmppath)
        sys.modules.pop("detect")


modfol = env_base["module_folders"].split(";")
modfol.append("modules")
modfol.append("web_modules")
#temporarily, these should be handled in a different pass
modfol.append("backends")
module_folders = list()

for fol in modfol:
    folt = fol.strip()

    if folt == "":
        continue

    module_folders.append(os.path.abspath(folt) + "/*")

module_list = []

for mf in module_folders:
    for x in sorted(glob.glob(mf)):
        if not os.path.isdir(x) or not os.path.exists(x + "/detect.py"):
            continue

        tmppath = os.path.realpath(os.path.expanduser(os.path.expandvars(x))) + "/"

        sys.path.insert(0, tmppath)
        import detect

        if detect.is_active() and detect.can_build():
            #x = x.replace("/", "")  # rest of world
            x = x.replace("\\", "/")  # win32

            tx = x.split("/")

            module_list.append([ tx[len(tx) - 1], tmppath ])

        sys.path.remove(tmppath)
        sys.modules.pop("detect")

# Sort modules dependencies
sort_modules_dependencies(module_list)

env = env_base.Clone()

if scons_ver >= (4, 0, 0):
    env.Tool("compilation_db")
    env.Alias("compiledb", env_base.CompilationDatabase())

Export("env")

SConscript("core/SCsub")
SConscript("web/SCsub")
SConscript("platform/SCsub")

if env_base["databases"]:
    for d in database_list:
        tmppath = "./database/" + d
        sys.path.insert(0, tmppath)

        import detect

        env_db = env_base.Clone()

        if scons_ver >= (4, 0, 0):
            env_db.Tool("compilation_db")
            env_db.Alias("compiledb", env_base.CompilationDatabase())

        detect.configure(env_db)
        detect.configure(env)

        Export("env_db")

        SConscript("database/" + d + "/SCsub")

        sys.path.remove(tmppath)
        sys.modules.pop("detect")

for m in module_list:
    tmppath = m[1]
    sys.path.insert(0, tmppath)

    import detect

    env_mod = env_base.Clone()

    # Compilation DB requires SCons 3.1.1+.
    from SCons import __version__ as scons_raw_version

    scons_ver = env_mod._get_major_minor_revision(scons_raw_version)

    if scons_ver >= (4, 0, 0):
        env_mod.Tool("compilation_db")
        env_mod.Alias("compiledb", env_base.CompilationDatabase())

    detect.configure(env_mod)
    detect.configure(env)

    Export("env_mod")

    SConscript(m[1] + "/SCsub")

    sys.path.remove(tmppath)
    sys.modules.pop("detect")


folders = env_base["folders"].split(";")

files = []

for fol in folders:
    folt = fol.strip()

    if folt == "":
        continue

    ff = os.listdir(folt)

    for f in ff:
        if f.endswith("cpp"):
            files.append(os.path.abspath(fol + "/" + f))
            #files.append(fol + "/" + f)

env.prg_sources = files
libapp = env.add_library("application", env.prg_sources)
env.Prepend(LIBS=[libapp])


mfp = os.path.abspath(env_base["main_file"])
prog = env.add_program("#bin/server", [ mfp ])

