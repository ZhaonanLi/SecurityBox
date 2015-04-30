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
	void AES_EncryptFile(string ofilename, string efilename);
	void AES_DecryptFile(string efilename, string rfilename, SecByteBlock key,  byte iv[]);
	void SHA_HMAC(string message);
	bool SHA_Verify(string message, string mac, SecByteBlock key);
	void DSA_Sign(string message);
	bool DSA_Verify(string message, string signature);
	void GenerateDSAkey();
	void GenerateFileKey();
	void GenerateSHAkey();
	void GenerateMessageKey();
	void AES_EncryptMessage(string plain);
	void AES_DecryptMessage(string cipher);

public:

	string ofile; 
	string efile; 
	string rfile; 
	string ciphermessage;
	string plainmessage;
	SecByteBlock FileAESkey;
	SecByteBlock MessageAESkey;
	byte MessageAESiv[ AES::BLOCKSIZE ];
	SecByteBlock SHAkey;
	byte FileAESiv[ AES::BLOCKSIZE ];
	string SHAhmac;
	string DSAsignature;
	DSA::PrivateKey UserDSAprivateKey;
	DSA::PublicKey UserDSApublicKey;
	DSA::PublicKey ServerDSApublicKey;




};

#endif // !defined(AFX_CIPHERSUITE_H__97623BCF_BA57_4252_A385_73A0A383E44E__INCLUDED_)
