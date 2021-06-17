import os
import platform
import sys


def is_active():
    return True


def get_name():
    return "trantor"


def can_build():
    return False


def get_opts():
    return []

def get_flags():

    return []


def configure(env):
    #env.Append(CXX=["-std=c++17"])

    env.Prepend(CPPPATH=["#modules/trantor"])
    env.Prepend(CPPPATH=["#modules/trantor/trantor/net"])
    env.Prepend(CPPPATH=["#modules/trantor/trantor/net/inner"])
    env.Prepend(CPPPATH=["#modules/trantor/trantor/utils"])

    
