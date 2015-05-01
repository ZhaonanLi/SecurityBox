// Base64.
#include <iostream>
#include <string>

#include <cryptopp/base64.h>

using namespace CryptoPP;

std::string encode_base64(std::string message)
{
    std::string encoded_str;
    CryptoPP::StringSource str_source (
        message,
        true,
        new CryptoPP::Base64Encoder(
            new CryptoPP::StringSink(encoded_str),
            false
        )
    );

    return encoded_str;
}

std::string decode_base64(std::string encoded_str)
{
    std::string message;
    CryptoPP::StringSource(
        encoded_str,
        true,
        new CryptoPP::Base64Decoder(
            new CryptoPP::StringSink(message)
        )
    );

    return message;
}
