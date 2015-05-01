// SHA256.
#include <iostream>
#include <string>

#include <cryptopp/sha.h>
#include <cryptopp/base64.h>

using namespace CryptoPP;

std::string SHA256_Hash(std::string message){
    std::string digest;
    CryptoPP::SHA256 sha;

    StringSource(
        message,
        true,
        new HashFilter(
            sha,
            new Base64Encoder (
                new StringSink(digest)
            )
        )
    );

    return digest;
}

int main()
{
    std::string message = "This is a SHA256 test.";
    std::string hash = SHA256_Hash(message);

    std::cout << "The message = " << message << std::endl;
    std::cout << "The hash = " << hash << std::endl;

    return 0;
}
