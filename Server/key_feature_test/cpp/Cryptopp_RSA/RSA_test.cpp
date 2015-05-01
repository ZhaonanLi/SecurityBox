// This file is for testing RSA under Crypto++.
#include <iostream>
#include <string>

#include <cryptopp/rsa.h>
#include <cryptopp/osrng.h>
#include <cryptopp/base64.h>
#include <cryptopp/files.h>

using namespace CryptoPP;

void generate_rsa_private_public_key_pair()
{
    AutoSeededRandomPool rng;
    InvertibleRSAFunction private_key;
    private_key.Initialize(rng, 1024);

    std::string private_key_filename = "./rsa_private_key.txt";
    Base64Encoder private_key_sink(new FileSink(private_key_filename.c_str()));
    private_key.DEREncode(private_key_sink);
    private_key_sink.MessageEnd();

    RSAFunction public_key(private_key);
    std::string public_key_filename = "./rsa_public_key.txt";
    Base64Encoder public_key_sink(new FileSink(public_key_filename.c_str()));
    public_key.DEREncode(public_key_sink);
    public_key_sink.MessageEnd();
}

std::string encrypt(std::string message)
{
    std::string public_key_filename = "./rsa_public_key.txt";
    FileSource pubFile(public_key_filename.c_str(), true, new Base64Decoder);

    AutoSeededRandomPool rng;

    RSAES_OAEP_SHA_Encryptor pub(pubFile);
    std::string cipher;
    StringSource(message, true,
            new PK_EncryptorFilter(rng, pub,
                    new Base64Encoder(new StringSink(cipher))));
    return cipher;
}

std::string decrypt(std::string cipher)
{
    std::string private_key_filename = "./rsa_private_key.txt";
    FileSource privFile(private_key_filename.c_str(), true, new Base64Decoder);

    AutoSeededRandomPool rng;

    RSAES_OAEP_SHA_Decryptor priv(privFile);
    std::string plaintext;
    StringSource(cipher, true,
            new Base64Decoder(
                    new PK_DecryptorFilter(rng, priv,
                            new StringSink(plaintext))));
    return plaintext;
}

int main()
{
    // std::cout << "Generate key pair and write them into files." << std::endl;
    // generate_rsa_private_public_key_pair();
    //
    // std::string plaintext = "This is a RSA test.";
    // std::string cipher = encrypt(plaintext);
    // std::cout << "The plaintext = " << plaintext << ", length = " << plaintext.size() << std::endl;
    // std::cout << "The cipher = " << cipher << ", length = " << cipher.size() << std::endl;

    std::string cipher = "O0MQRIC3W7c4jh75tvZC1sibL8GBwLbhlt8sy1HlXrh+HrJgyHpUBKNHXGIV/AXXgm2B4z7b\n";
    cipher += "YSO3/txC07u19sz4H25BQUZz6j1osvNb09CB4deMrE3lI0nzMu34985M029DwligHW0Wz2nv\n";
    cipher += "cS6ZAxUdEno2WpGcvNONY21TVWQ=\n";
    std::string decoded_plaintext = decrypt(cipher);
    std::cout << "The decoded_plaintext = " << decoded_plaintext << ", length = " << decoded_plaintext.size() << std::endl;

    return 0;
}
