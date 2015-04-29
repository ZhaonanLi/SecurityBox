// cryptopp::AES test.
#include <iostream>
#include <string>

#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>

int main()
{
    std::string plaintext = "Hello world.";
    std::string ciphertext;
    std::string decrypted_text;

    // Key and IV setup.
    byte key[CryptoPP::AES::DEFAULT_KEYLENGTH];
    byte iv[CryptoPP::AES::BLOCKSIZE];

    memset(key, 0x00, CryptoPP::AES::DEFAULT_KEYLENGTH);
    memset(iv, 0x00, CryptoPP::AES::BLOCKSIZE);

    // Dump plaintext test.
    std::cout << "The plaintext size = " << plaintext.size() << std::endl;
    std::cout << "The plaintext = " << plaintext << std::endl;

    // Encrypt.
    CryptoPP::AES::Encryption aes_encryption (key, CryptoPP::AES::DEFAULT_KEYLENGTH);
    CryptoPP::CBC_Mode_ExternalCipher::Encryption cbc_cipher_encryption(aes_encryption, iv);

    CryptoPP::StreamTransformationFilter stf_encryptor(cbc_cipher_encryption,
                                                       new CryptoPP::StringSink(ciphertext));
    stf_encryptor.Put(reinterpret_cast<const unsigned char*>(plaintext.c_str()),
                      plaintext.length());
    stf_encryptor.MessageEnd();

    // Dump ciphertext.
    std::cout << "The ciphertext size = " << ciphertext.size() << std::endl;
    std::cout << "The ciphertext is:" << std::endl;
    for (int i = 0; i < ciphertext.size(); i ++)
    {
        std::cout << "0x" << std::hex << (0xFF & static_cast<byte>(ciphertext[i])) << " ";
    }
    std::cout << std::endl;

    // Decrypt.
    CryptoPP::AES::Decryption aes_decryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
    CryptoPP::CBC_Mode_ExternalCipher::Decryption cbc_cipher_decryption(aes_decryption, iv);

    CryptoPP::StreamTransformationFilter stf_decryptor(cbc_cipher_decryption,
                                                       new CryptoPP::StringSink(decrypted_text));
    stf_decryptor.Put(reinterpret_cast<const unsigned char*>(ciphertext.c_str()),
                      ciphertext.length());
    stf_decryptor.MessageEnd();

    // Dump decrypted_text.
    std::cout << "The decrypted_text size = " << std::to_string(decrypted_text.size()) << std::endl;
    std::cout << "The decrypted_text = " << decrypted_text << std::endl;

    return 0;
}
