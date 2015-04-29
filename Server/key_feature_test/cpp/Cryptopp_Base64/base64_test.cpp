// This cpp file is for testing base64 encryption and decryption.
#include <iostream>
#include <string>

#include <cryptopp/base64.h>

int main()
{
    std::string plaintext = "Hello world.";
    std::string encoded_text;
    std::string decoded_text;

    // Dump plaintext.
    std::cout << "The plaintext length = " << plaintext.size() << std::endl;
    std::cout << "The plaintext = " << plaintext << std::endl;


    // Encrypt.
    CryptoPP::StringSource str_source (
        plaintext,
        true,
        new CryptoPP::Base64Encoder(
            new CryptoPP::StringSink(encoded_text),
            false
        )
    );

    std::cout << "The encoded_text length = " << encoded_text.size() << std::endl;
    std::cout << "The encoded_text = " << encoded_text << std::endl;

    // Decrypt.
    CryptoPP::StringSource(
        encoded_text,
        true,
        new CryptoPP::Base64Decoder(
            new CryptoPP::StringSink(decoded_text)
        )
    );

    std::cout << "The decoded_text length = " << decoded_text.size() << std::endl;
    std::cout << "The decoded_text = " << decoded_text << std::endl;
}
