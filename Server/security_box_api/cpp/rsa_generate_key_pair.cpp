// RSA generate public key and private key pair.
#include <iostream>
#include <string>

#include <cryptopp/rsa.h>
#include <cryptopp/osrng.h>

using namespace CryptoPP;
using namespace std;

void gen_rsa_public_key()
{
    AutoSeededRandomPool prng;
	InvertibleRSAFunction params;
	params.GenerateRandomWithKeySize( prng, 1536 );
	RSA::PrivateKey server_rsa_private_key =RSA::PrivateKey( params );
	RSA::PublicKey server_rsa_public_key =RSA::PublicKey( params );

    cout << "The private key = " << string(server_rsa_private_key) << endl;
    cout << "The public key = " << server_rsa_public_key << endl;
}
