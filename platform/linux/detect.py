import os
import platform
import sys


def is_active():
    return True


def get_name():
    return "platform_linux"


def can_build():
    return True


def get_opts():
    return []

def get_flags():

    return []


def configure(env):
    pass
