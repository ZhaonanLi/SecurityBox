# HMAC_PY.
import subprocess
import base64


def make_hmac(message):
    message_base64 = base64.b64encode(message)
    command = "./HMAC -m {}".format(message_base64)
    return subprocess.Popen(command, shell=True, stdout=subprocess.PIPE).stdout.read()


def verify(message, mac_base64):
    message_base64 = base64.b64encode(message)
    command = "./HMAC -v {} {}".format(message_base64, mac_base64)
    is_valid = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE).stdout.read()
    return True if is_valid == "true" else False


if __name__ == "__main__":
    m = "Hello world"
    mac_base64 = make_hmac(m)
    
    m2 = "hello world"
    mac2_base64 = make_hmac(m2)
    
    is_valid = verify(m, mac_base64)
    
    if is_valid:
        print "verify"
    else:
        print "no verify"
