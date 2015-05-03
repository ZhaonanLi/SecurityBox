# Test security model
import security_model

if __name__ == "__main__":
    print "testing..."
    m = "hello World!"
    m_encrypted = security_model.aes_encrypt(m)
    mac = security_model.hmac_make(m_encrypted)
    print "m = ", m
    print "m_encrypted = ", m_encrypted
    print "mac = ", mac

    return_message_encrypt = security_model.aes_encrypt("Hello world!")
    mac_2 = mac
    print "mac_2 = ", mac_2
    is_valid = security_model.hmac_verify(return_message_encrypt, mac_2)
    if is_valid:
        print "verify"
    else:
        print "not verify"

    m_decrypted = security_model.aes_decrypt(return_message_encrypt)
    print "m_decrypted = ", m_decrypted
