# AES PY.
import subprocess
import base64


def encrypt(message):
    message_base64 = base64.b64encode(message)
    command = "./AES -e {}".format(message_base64)
    return subprocess.Popen(command, shell=True, stdout=subprocess.PIPE).stdout.read()


def decrypt(cipher_base64):
    command = "./AES -d {}".format(cipher_base64)
    message_base64 = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE).stdout.read()
    return base64.b64decode(message_base64)


if __name__ == "__main__":
    m = "Hello world"
    c_base64 = encrypt(m)
    decrypted_m = decrypt(c_base64)
    print "message = " + str(m)
    print "cipher_base64 = " + str(c_base64)
    print "decrypted message = " + str(decrypted_m)
