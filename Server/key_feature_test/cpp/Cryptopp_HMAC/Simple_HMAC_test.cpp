// This file for test HMAC.
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include <cryptopp/osrng.h>
#include <cryptopp/hmac.h>
#include <cryptopp/sha.h>
#include <cryptopp/base64.h>

using namespace CryptoPP;

void generate_key(byte key[16])
{
    AutoSeededRandomPool rng;
    rng.GenerateBlock(key, 16);
}


std::string generate_HMAC(std::string message, byte key[])
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

bool verify_HMAC(std::string message, byte* key, std::string mac)
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


int main()
{
    // byte key[16];
    // generate_key(key);

    byte key[16] = {182,85,26,72,32,197,251,106,81,48,156,159,67,196,111,107};

    std::stringstream key_ss;
    std::string key_s;
    for (int i = 0; i < sizeof(key); i ++)
    {
        key_ss << (int)key[i];
        if (i != sizeof(key) - 1)
        {
            key_ss << ",";
        }
    }
    key_s = key_ss.str();
    std::cout << "The key s = " << key_s << std::endl;

    // Generate HMAC.
    std::string message = "This is a HAMC test.";
    std::string mac = generate_HMAC(message, key);
    std::cout << "message = " << message << std::endl;
    std::cout << "mac = " << mac << std::endl;

    // Encode base64.
    std::string mac_base64;
    CryptoPP::StringSource str_source (
        mac,
        true,
        new Base64Encoder(
            new StringSink(mac_base64),
            false
        )
    );
    std::cout << "The mac_base64 = " << mac_base64 << std::endl;

    // Decode base64.
    std::string raw_mac;
    CryptoPP::StringSource(
        mac_base64,
        true,
        new Base64Decoder(
            new StringSink(raw_mac)
        )
    );

    // Verify HMAC.
    bool is_valid = verify_HMAC(message, key, raw_mac);
    if (is_valid) std::cout << "verified." << std::endl;
    else std::cout << "not verified." << std::endl;

}
