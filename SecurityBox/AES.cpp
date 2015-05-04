// AES.
#include <iostream>
#include <string>
#include <fstream>

#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include <cryptopp/base64.h>
#include <cryptopp/osrng.h>

using namespace CryptoPP;

typedef unsigned char byte;

std::string base64_encode(std::string message)
{
    std::string message_base64;
    CryptoPP::StringSource str_source (
        message,
        true,
        new CryptoPP::Base64Encoder(
            new CryptoPP::StringSink(message_base64),
            false
        )
    );
    return message_base64;
}

std::string base64_decode(std::string message_base64)
{
    std::string message;
    CryptoPP::StringSource(
        message_base64,
        true,
        new CryptoPP::Base64Decoder(
            new CryptoPP::StringSink(message)
        )
    );
    return message;
}

void generate_key_and_iv()
{
    byte key[AES::DEFAULT_KEYLENGTH];
    byte iv[AES::DEFAULT_KEYLENGTH];

    AutoSeededRandomPool rng;
    rng.GenerateBlock(key, AES::DEFAULT_KEYLENGTH);
    rng.GenerateBlock(iv, AES::DEFAULT_KEYLENGTH);
    
    std::string key_filename = "./aes_key.txt";
    
    // Write key into file.
    std::ofstream key_file(key_filename, std::ios::binary|std::ios::out);
    key_file.write((char*)&key, sizeof(key));
    key_file.close();
    
    std::string iv_filename = "./aes_iv.txt";
    
    // Write iv into file.
    std::ofstream iv_file(iv_filename, std::ios::binary|std::ios::out);
    iv_file.write((char*)&key, sizeof(key));
    iv_file.close();
}

void load_key(std::string key_filename, byte key[])
{
    std::ifstream key_file(key_filename, std::ios::binary);
    key_file.seekg(0, std::ios::end);
    size_t key_arr_len = key_file.tellg();
    char *key_char_arr = new char[key_arr_len];
    key_file.seekg(0, std::ios::beg);
    key_file.read(key_char_arr, key_arr_len);
    key_file.close();

    for (int i = 0; i < key_arr_len; i ++)
    {
        key[i] = (byte)key_char_arr[i];
    }
}

std::string encrypt(std::string plaintext, byte key[], byte iv[])
{
    std::string cipher;

    AES::Encryption aes_encryption (key, AES::DEFAULT_KEYLENGTH);
    CBC_Mode_ExternalCipher::Encryption cbc_cipher_encryption(aes_encryption, iv);

    StreamTransformationFilter stf_encryptor(
        cbc_cipher_encryption,
        new StringSink(cipher)
    );

    stf_encryptor.Put(
        reinterpret_cast<const unsigned char*>(plaintext.c_str()),
        plaintext.length()
        );

    stf_encryptor.MessageEnd();

    return cipher;
}

std::string decrypt(std::string cipher, byte key[], byte iv[])
{
    std::string plaintext;

    AES::Decryption aes_decryption(key, AES::DEFAULT_KEYLENGTH);
    CBC_Mode_ExternalCipher::Decryption cbc_cipher_decryption(aes_decryption, iv);

    StreamTransformationFilter stf_decryptor(
        cbc_cipher_decryption,
        new StringSink(plaintext)
    );

    stf_decryptor.Put(
        reinterpret_cast<const unsigned char*>(cipher.c_str()),
        cipher.length()
        );

    stf_decryptor.MessageEnd();

    return plaintext;
}

int main(int argc, char** argv)
{    
    if (argc <= 1) return 0;
    
    std::string key_filename = "./aes_key.txt";
    std::string iv_filename = "./aes_iv.txt";

    if (strcmp(argv[1], "-e") == 0)
    {
        byte key[AES::DEFAULT_KEYLENGTH];
        byte iv[AES::DEFAULT_KEYLENGTH];
        
        load_key(key_filename, key);
        load_key(iv_filename, iv);
        
        std::string m_b = argv[2];
        std::string m = base64_decode(m_b);
        
        std::string c = encrypt(m, key, iv);
        std::cout << base64_encode(c);
    }
    else if (strcmp(argv[1], "-d") == 0)
    {
        byte key[AES::DEFAULT_KEYLENGTH];
        byte iv[AES::DEFAULT_KEYLENGTH];
        
        load_key(key_filename, key);
        load_key(iv_filename, iv);
        
        std::string c_b = argv[2];
        std::string c 
        = base64_decode(c_b);
        std::string m = decrypt(c, key, iv);
        std::cout << base64_encode(m);
    }
    else if (strcmp(argv[1], "-g") == 0)
    {
        generate_key_and_iv();
    }
        
    return 0;
}
