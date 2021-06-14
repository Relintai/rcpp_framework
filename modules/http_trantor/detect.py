import os
import platform
import sys


def is_active():
    return True


def get_name():
    return "http_trantor"


def can_build():
    return True


def get_opts():
    return []

def get_flags():

    return []


def configure(env):
    env.Prepend(CPPPATH=["#modules/http_trantor"])
    env.Prepend(CPPPATH=["#modules/http_trantor/trantor/net"])
    env.Prepend(CPPPATH=["#modules/http_trantor/trantor/net/inner"])
    env.Prepend(CPPPATH=["#modules/http_trantor/trantor/utils"])

    
