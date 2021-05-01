import os
import platform
import sys


def is_active():
    return True


def get_name():
    return "mqtt_server"


def can_build():
    if os.name == "posix" or sys.platform == "darwin":
        x11_error = os.system("pkg-config --version > /dev/null")
        if x11_error:
            return False

        libevent_err = os.system("pkg-config libevent --modversion --silence-errors > /dev/null ")

        if libevent_err:
            print("libevent not found! MQTT server will not be available!")
            return False

        print("libevent found! MQTT server will be available!")

        return True

    return False


def get_opts():
    return []

def get_flags():

    return []


def configure(env):
    env.ParseConfig("pkg-config libevent --cflags --libs")
    
    env.Append(CPPDEFINES=["MQTT_SERVER_PRESENT"])


