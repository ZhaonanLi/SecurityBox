// RSA.
#include <iostream>
#include <string>
#include <fstream>

#include <cryptopp/rsa.h>
#include <cryptopp/osrng.h>
#include <cryptopp/base64.h>
#include <cryptopp/files.h>

#include "Base64.cpp"

using namespace CryptoPP;

bool file_exist(std::string filename)
{
    std::ifstream file(filename);
    if (file.good())
    {
        file.close();
        return true;
    }
    else
    {
        file.close();
        return false;
    }
}

void generate_rsa_private_and_public_key(std::string private_key_filename,
                                         std::string public_key_filename
)
{
    if (file_exist(private_key_filename) && file_exist(public_key_filename))
    {
        return;
    }
    AutoSeededRandomPool rng;
    InvertibleRSAFunction private_key;
    private_key.Initialize(rng, 1024);

    Base64Encoder private_key_sink(new FileSink(private_key_filename.c_str()));
    private_key.DEREncode(private_key_sink);
    private_key_sink.MessageEnd();

    RSAFunction public_key(private_key);
    Base64Encoder public_key_sink(new FileSink(public_key_filename.c_str()));
    public_key.DEREncode(public_key_sink);
    public_key_sink.MessageEnd();
}

std::string encrypt(std::string plaintext, std::string public_key_filename)
{
    FileSource pubFile(public_key_filename.c_str(), true, new Base64Decoder);

    AutoSeededRandomPool rng;

    RSAES_OAEP_SHA_Encryptor pub(pubFile);
    std::string cipher;
    StringSource(
        plaintext,
        true,
        new PK_EncryptorFilter(
            rng,
            pub,
            new Base64Encoder(new StringSink(cipher))
        )
    );
    return cipher;
}

std::string decrypt(std::string cipher, std::string private_key_filename)
{
    FileSource privFile(private_key_filename.c_str(), true, new Base64Decoder);

    AutoSeededRandomPool rng;

    RSAES_OAEP_SHA_Decryptor priv(privFile);
    std::string plaintext;
    StringSource(
        cipher,
        true,
        new Base64Decoder(
            new PK_DecryptorFilter(
                rng,
                priv,
                new StringSink(plaintext)
            )
        )
    );
    return plaintext;
}

std::string retrieve_key(std::string key_filename)
{
    if (!file_exist(key_filename))
    {
        std::string server_rsa_public_key_filename = "./security_file/";
        server_rsa_public_key_filename += "server_rsa_public_key.txt";

        std::string server_rsa_private_key_filename = "./security_file/";
        server_rsa_private_key_filename += "server_rsa_private_key.txt";
        generate_rsa_private_and_public_key(
            server_rsa_private_key_filename,
            server_rsa_public_key_filename
        );
    }

    // Load key from file.
    std::ifstream key_file(key_filename, std::ios::binary);
    key_file.seekg(0, std::ios::end);
    size_t key_arr_len = key_file.tellg();
    char *key_char_arr = new char[key_arr_len];
    key_file.seekg(0, std::ios::beg);
    key_file.read(key_char_arr, key_arr_len);
    key_file.close();

    // Buid key string.
    std::string key_str;
    for (int i = 0; i < key_arr_len; i ++)
    {
        key_str += key_char_arr[i];
    }
    return key_str;
}

int main(int argc, char** argv)
{
    if (argc <= 1) return 0;
    if (strcmp(argv[1], "--retrieve_pub_k") == 0)
    {
        std::string key_filename = std::string(argv[2]);
        std::string key_str = retrieve_key(key_filename);
        std::string base64_key_str = encode_base64(key_str);
        std::cout << base64_key_str;
    }

    return 0;
}
