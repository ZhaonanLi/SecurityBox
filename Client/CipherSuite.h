// CipherSuite.h: interface for the CCipherSuite class.
//
//////////////////////////////////////////////////////////////////////
#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;

#include <osrng.h>
#include <cryptlib.h>
#include <hex.h>
#include <files.h>
#include <filters.h>
#include <aes.h>
#include <modes.h>
#include <hmac.h>
#include <sha.h>
#include <secblock.h>
#include <dsa.h>
#include <rsa.h>
using namespace CryptoPP;

#ifdef _DEBUG
#  pragma comment ( lib, "cryptlibd" )
#else
#  pragma comment ( lib, "cryptlib" )
#endif


#if !defined(AFX_CIPHERSUITE_H__97623BCF_BA57_4252_A385_73A0A383E44E__INCLUDED_)
#define AFX_CIPHERSUITE_H__97623BCF_BA57_4252_A385_73A0A383E44E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCipherSuite  
{
public:
	CCipherSuite();
	virtual ~CCipherSuite();

public:
	void GenerateRSAkey();
	RSA::PublicKey GetRSApublicKey();
	void SetServerRSApublicKey(RSA::PublicKey publickey);
	void RSA_Encrypt(string plain);
	void RSA_Decrypt(string cipher);
	string GetRSAplain();
	string GetRSAcipher();

	void GenerateFileKey(string userpassword, string serversalt);   /////////
	void AES_EncryptFile(string ofilename, string efilename);
	void AES_DecryptFile(string efilename, string rfilename);

	
	void GenerateMessageKey();
	void SetMessageKey(SecByteBlock messagekey, byte MessageAESiv[]);
	void AES_EncryptMessage(string plain);
	void AES_DecryptMessage(string cipher);
	string GetCipherMessage();
	string GetPlainMessage();

	void GenerateSHAkey();
	void SHA_HMAC(string message);
	bool SHA_Verify(string message, string mac, SecByteBlock key);
	SecByteBlock GetSHAkey();
	string GetHmac();

	void GenerateDSAkey();
	void DSA_Sign(string message);
	bool DSA_Verify(string message, string signature);
	void SetServerDSApublicKey(DSA::PublicKey key);
	DSA::PublicKey GetDSApublicKey();
	string GetSignature();

private:
	string Encoded(string str);

	string original_file_; 
	string encrypted_file_; 
	string recovered_file_; 

	string cipher_message_;
	string plain_message_;

	RSA::PrivateKey user_rsa_private_key_;
	RSA::PublicKey user_rsa_public_key_;
	RSA::PublicKey server_rsa_public_key_;
	
	string rsa_cipher_;
	string rsa_plain_;

	SecByteBlock file_aes_key_;
	byte file_aes_iv_[ AES::BLOCKSIZE ];

	SecByteBlock message_aes_key_;
	byte message_aes_iv_[ AES::BLOCKSIZE ];
	
	SecByteBlock sha_key_;
	string sha_hmac_;

	DSA::PrivateKey user_dsa_private_key_;
	DSA::PublicKey user_dsa_public_key_;
	DSA::PublicKey server_dsa_public_key_;
	string dsa_signature_;
};

#endif // !defined(AFX_CIPHERSUITE_H__97623BCF_BA57_4252_A385_73A0A383E44E__INCLUDED_)
