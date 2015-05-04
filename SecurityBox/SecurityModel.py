# Security Model.
import os
import subprocess
import base64


def is_windows():
    os_name = os.name
    return True if os_name == "nt" else False


def aes_encrypt(message):
    command = './AES -e {}'
    if is_windows():
        command = "AES -e {}"

    message_base64 = base64.b64encode(message)
    command = command.format(message_base64)
    return subprocess.Popen(command, shell=True, stdout=subprocess.PIPE).stdout.read()


def aes_decrypt(cipher_base64):
    command = "./AES -d {}"
    if is_windows():
        command = "AES -d {}"

    command = command.format(cipher_base64)
    message_base64 = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE).stdout.read()
    return base64.b64decode(message_base64)


def hmac_make(message):
    command = "./HMAC -m {}"
    if is_windows():
        command = "HMAC -m {}"

    message_base64 = base64.b64encode(message)
    command = command.format(message_base64)
    return subprocess.Popen(command, shell=True, stdout=subprocess.PIPE).stdout.read()


def hmac_verify(message, mac_base64):
    command = "./HMAC -v {} {}"
    if is_windows():
        command = "HMAC -v {} {}"

    message_base64 = base64.b64encode(message)
    command = command.format(message_base64, mac_base64)
    is_valid = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE).stdout.read()
    return True if is_valid == "true" else False
