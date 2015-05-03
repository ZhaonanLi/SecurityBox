// HMAC.
#include <iostream>
#include <string>
#include <fstream>

#include <cryptopp/hmac.h>
#include <cryptopp/sha.h>
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

void generate_key()
{
    int key_length = 16;
    byte key[key_length];

    AutoSeededRandomPool rng;
    rng.GenerateBlock(key, key_length);
    
    std::string key_filename = "./hmac_key.txt";
    
    // Write key into file.
    std::ofstream key_file(key_filename, std::ios::binary|std::ios::out);
    key_file.write((char*)&key, sizeof(key));
    key_file.close();
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

std::string make_hmac(std::string message, byte key[])
{
    std::string mac;
    SecByteBlock key_block(16);
    for (int i = 0; i < 16; i ++)
    {
        key_block[i] = key[i];
    }

    HMAC<SHA256> hmac(key_block, key_block.size());
    StringSource ss(
        message,
        true,
        new HashFilter(
            hmac,
            new StringSink(mac))
    );

    return mac;
}

bool verify(std::string message, std::string mac, byte key[])
{
    try
    {
        SecByteBlock key_block(16);
        for (int i = 0; i < 16; i ++)
        {
            key_block[i] = key[i];
        }

        HMAC<SHA256> hmac(key_block, key_block.size());
        const int flags = HashVerificationFilter::THROW_EXCEPTION |
                          HashVerificationFilter::HASH_AT_END;
        StringSource(
            message + mac,
            true,
            new HashVerificationFilter(hmac, NULL, flags)
        );

        return true;
    }
    catch(const Exception& e)
    {
        return false;
    }
}

int main(int argc, char** argv)
{
    if (argc <= 1) return 0;
    
    std::string key_filename = "./hmac_key.txt";
    int key_length = 16;
    
    if (strcmp(argv[1], "-m") == 0)
    {
        byte key[key_length];
        load_key(key_filename, key);
        
        std::string message_base64 = argv[2];
        std::string message = base64_decode(message_base64);
        
        std::string mac = make_hmac(message, key);
        std::string mac_base64 = base64_encode(mac);
        
        std::cout << mac_base64;
    }
    else if (strcmp(argv[1], "-v") == 0)
    {
        byte key[key_length];
        load_key(key_filename, key);
        
        std::string message_base64 = argv[2];
        std::string message = base64_decode(message_base64);
        
        std::string mac_base64 = argv[3];
        std::string mac = base64_decode(mac_base64);
        
        bool is_valid = verify(message, mac, key);
        
        if (is_valid) std::cout << "true";
        else std::cout << "false";
    }
    else if (strcmp(argv[1], "-g") == 0)
    {
        generate_key();
    }
    
    return 0;
}
