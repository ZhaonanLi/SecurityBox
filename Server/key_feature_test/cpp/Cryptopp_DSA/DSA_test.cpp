// This file for test DSA.
#include <iostream>
#include <string>

#include <cryptopp/dsa.h>
#include <cryptopp/osrng.h>
#include <cryptopp/files.h>
#include <cryptopp/base64.h>

using namespace CryptoPP;

void generate_key(std::string public_key_filename,
                  std::string private_key_filename)
{
    AutoSeededRandomPool rng;
    DSA::PrivateKey private_key;
    private_key.GenerateRandomWithKeySize(rng, 1024);

    DSA::PublicKey public_key;
    public_key.AssignFrom(private_key);

    // Write private key and public key into file.
    Base64Encoder private_key_sink(new FileSink(private_key_filename.c_str()));
    private_key.DEREncode(private_key_sink);
    private_key_sink.MessageEnd();

    Base64Encoder public_key_sink(new FileSink(public_key_filename.c_str()));
    public_key.DEREncode(public_key_sink);
    public_key_sink.MessageEnd();
}

std::string sign(std::string mac, std::string private_key_filename)
{
    std::string signature;

    // Load private key from file.
    FileSource privFile(private_key_filename.c_str(), true, new Base64Decoder);
    AutoSeededRandomPool rng;

    DSA::Signer signer(privFile);
    StringSource(
        mac,
        true,
        new SignerFilter(
            rng,
            signer,
            new StringSink(signature)
        )
    );

    return signature;
}

bool verify(std::string mac, std::string signature,
            std::string public_key_filename)
{
    // Load public key from file.
    FileSource pubFile(public_key_filename.c_str(), true, new Base64Decoder);
    AutoSeededRandomPool rng;

    DSA::Verifier verifier(pubFile);

    try
    {
        StringSource(
            mac + signature,
            true,
            new SignatureVerificationFilter(
                verifier,
                NULL,
                SignatureVerificationFilter::THROW_EXCEPTION
            )
        );

        return true;
    }
    catch(Exception& e)
    {
        return false;
    }
}

int main()
{
    std::string public_key_filename = "./dsa_public_key.txt";
    std::string private_key_filename = "./dsa_private_key.txt";

    std::cout << "Generate public and private key." << std::endl;
    generate_key(public_key_filename, private_key_filename);

    // Sign.
    std::string mac = "this is a mac.";
    std::string signature = sign(mac, private_key_filename);
    std::cout << "mac = " << mac << ", length = " << mac.size() << std::endl;
    std::cout << "signature = " << signature << ", length = " << signature.size() << std::endl;

    // Encrypt.
    std::string mac_base64;
    StringSource(
        mac,
        true,
        new Base64Encoder(
            new StringSink(mac_base64),
            false
        )
    );
    std::string signature_base64;
    StringSource(
        signature,
        true,
        new Base64Encoder(
            new StringSink(signature_base64),
            false
        )
    );
    std::cout << "mac base64 = " << mac_base64 << std::endl;
    std::cout << "signature base64 = " << signature_base64 << std::endl;

    // Decrypt.
    std::string raw_mac;
    StringSource(
        mac_base64,
        true,
        new Base64Decoder(
            new StringSink(raw_mac)
        )
    );
    std::string raw_signature;
    StringSource(
        signature_base64,
        true,
        new Base64Decoder(
            new StringSink(raw_signature)
        )
    );
    mac = raw_mac;
    signature = raw_signature;

    // Verify.
    bool is_valid = verify(mac, signature, public_key_filename);
    if (is_valid) std::cout << "verified." << std::endl;
    else std::cout << "not verified." << std::endl;

    return 0;
}
