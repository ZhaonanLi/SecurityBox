// CipherSuite.cpp: implementation of the CCipherSuite class.
//
//////////////////////////////////////////////////////////////////////

#include "CipherSuite.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCipherSuite::CCipherSuite()
{

}

CCipherSuite::~CCipherSuite()
{


}


void CCipherSuite::GenerateRSAkey()
{
	AutoSeededRandomPool prng;
	InvertibleRSAFunction params;
	params.GenerateRandomWithKeySize( prng, 1536 );
	user_rsa_private_key_=RSA::PrivateKey( params );
	user_rsa_public_key_=RSA::PublicKey( params );
	return;
}


void CCipherSuite::RSA_Encrypt( string plain )
{
	AutoSeededRandomPool prng;
	RSAES_OAEP_SHA_Encryptor e( server_rsa_public_key_ ); 
	
	StringSource( plain, true,
		new PK_EncryptorFilter( prng, e, new StringSink( rsa_cipher_ )) // PK_EncryptorFilter
         ); // StringSource
	cout << "RSA cipher text: " << Encoded( rsa_cipher_ ) << endl;
	return;
}


void CCipherSuite::RSA_Decrypt( string cipher )
{
	AutoSeededRandomPool prng;
	RSAES_OAEP_SHA_Decryptor d( user_rsa_private_key_ );
	
	StringSource( cipher, true,
            new PK_DecryptorFilter( prng, d, new StringSink( rsa_plain_ )) // PK_EncryptorFilter
			); // StringSource
	cout << "RSA plain text: " << rsa_plain_ << endl;
	return;
}

	
void CCipherSuite::GenerateFileKey( string userpassword, string serversalt )
{		
	AutoSeededRandomPool prng;
	file_aes_key_=SecByteBlock(AES::DEFAULT_KEYLENGTH);
	prng.GenerateBlock( file_aes_key_,file_aes_key_.size() );
	prng.GenerateBlock( file_aes_iv_, sizeof(file_aes_iv_) );
	
	string encoded;
	//  print key
	encoded.erase();
	StringSource( file_aes_key_, sizeof( file_aes_key_ ), true,
		new HexEncoder( new StringSink(encoded)) // HexEncoder
		); // StringSource
	cout<< "AES file key: "<< encoded <<endl;

	//  print iv
	encoded.erase();
	StringSource( file_aes_iv_, sizeof( file_aes_iv_ ), true,
		new HexEncoder( new StringSink(encoded)) // HexEncoder
		); // StringSource
	cout<< "AES file iv: "<< encoded <<endl;
	return;
}


void CCipherSuite::AES_EncryptFile( string ofilename, string efilename )
{
	original_file_=ofilename;
	encrypted_file_=efilename;
	
	try
	{
		CBC_Mode< AES >::Encryption e;
		e.SetKeyWithIV( file_aes_key_, file_aes_key_.size(), file_aes_iv_ );
		FileSource fs1(ofilename.c_str(), true,
			new StreamTransformationFilter( e,
            new FileSink(efilename.c_str()),BlockPaddingSchemeDef::BlockPaddingScheme::ONE_AND_ZEROS_PADDING,
			true)// StreamTransformationFilter     
			); // FileSource
	}
	catch( const CryptoPP::Exception& e )
	{
		cerr << e.what() << endl;
		exit(1);
	}

return;
}


void CCipherSuite::AES_DecryptFile( string efilename, string rfilename )
{

	encrypted_file_=efilename;
	recovered_file_=rfilename;

	try
	{
		CBC_Mode< AES >::Decryption d;
		d.SetKeyWithIV( file_aes_key_, file_aes_key_.size(), file_aes_iv_ );

		FileSource fs1(efilename.c_str(), true,
                   new StreamTransformationFilter( d,
				   new FileSink(rfilename.c_str()),BlockPaddingSchemeDef::BlockPaddingScheme::ONE_AND_ZEROS_PADDING,
				   true)// StreamTransformationFilter     
				   ); // StringSource
	}
	catch( const CryptoPP::Exception& e )
	{
		cerr << e.what() << endl;
		exit(1);
	}

return;
}

void CCipherSuite::GenerateMessageKey()
{		
	AutoSeededRandomPool prng;

	prng.GenerateBlock( message_aes_key_, sizeof(message_aes_key_) );
	prng.GenerateBlock( message_aes_iv_, sizeof(message_aes_iv_) );
	
	string encoded;
	//  print key
	encoded.erase();
	StringSource( message_aes_key_, sizeof( message_aes_key_ ), true,
		new HexEncoder( new StringSink(encoded)) // HexEncoder
		); // StringSource
	cout<< "AES message key: "<< encoded <<endl;

	//  print iv
	encoded.erase();
	StringSource( message_aes_iv_, sizeof( message_aes_iv_ ), true,
		new HexEncoder( new StringSink(encoded)) // HexEncoder
		); // StringSource
	cout<< "AES message iv: "<< encoded <<endl;
	return;
}

void CCipherSuite::AES_EncryptMessage( string plain )
{	
	try
	{
		CBC_Mode< AES >::Encryption e;

		e.SetKeyWithIV( message_aes_key_, sizeof(message_aes_key_), message_aes_iv_ );
		
		StringSource ss( plain, true, 
			new StreamTransformationFilter( e,
            new StringSink( cipher_message_ ),BlockPaddingSchemeDef::BlockPaddingScheme::ONE_AND_ZEROS_PADDING,
			true)// StreamTransformationFilter     
			); // StringSource
	}
	catch( const CryptoPP::Exception& e )
	{
		cerr << e.what() << endl;
		exit(1);
	}

	cout << "cipher text: " << Encoded( cipher_message_ ) << endl;
	return;
}


void CCipherSuite::AES_DecryptMessage(string cipher)
{
	try
	{
		CBC_Mode< AES >::Decryption d;
		d.SetKeyWithIV( message_aes_key_, sizeof(message_aes_key_), message_aes_iv_ );
		
		StringSource ss( cipher, true, 
			new StreamTransformationFilter( d,
            new StringSink( plain_message_ ),BlockPaddingSchemeDef::BlockPaddingScheme::ONE_AND_ZEROS_PADDING,
			true)// StreamTransformationFilter
			); // StringSource
	}
	catch( const CryptoPP::Exception& e )
	{
		cerr << e.what() << endl;
		exit(1);
	}
	cout << "recovered text: " << plain_message_ << endl;
	return;
}

void CCipherSuite::GetMessageKey( byte key[])
{

	memcpy(key,message_aes_key_,AES::DEFAULT_KEYLENGTH);
	return;

}

void CCipherSuite::GetMessageIv(byte iv[])
{
	memcpy(iv,message_aes_iv_,AES::BLOCKSIZE);
	return;

}


void CCipherSuite::GenerateSHAkey()
{
	AutoSeededRandomPool prng;
	sha_key_=SecByteBlock(16);
	prng.GenerateBlock(sha_key_, sha_key_.size());

	string encoded;
	// print key
	encoded.erase();
	StringSource ss1(sha_key_, sha_key_.size(), true,
		new HexEncoder(
        new StringSink(encoded)
		) // HexEncoder
		); // StringSource

cout << "SHA key: " << encoded << endl;
return;
}


void CCipherSuite::SHA_HMAC( string message )
{
	try
	{
		HMAC< SHA256 > hmac(sha_key_, sha_key_.size());
	
		StringSource ss2(message, true, 
			new HashFilter(hmac,
			new StringSink(sha_hmac_)
			) // HashFilter      
			); // StringSource
	}
	catch( const CryptoPP::Exception& e )
	{
		cerr << e.what() << endl;
		exit(1);
	}

	string encoded;
	// print
	encoded.erase();
	StringSource ss3(sha_hmac_, true,
		new HexEncoder(
		new StringSink(encoded)) // HexEncoder
		); // StringSource
	cout << "hmac: " << encoded << endl;
	return;
}


bool CCipherSuite::SHA_Verify( string message, string mac, SecByteBlock key )
{
	try
	{
		HMAC< SHA256 > hmac(key, key.size());
		const int flags = HashVerificationFilter::THROW_EXCEPTION | HashVerificationFilter::HASH_AT_END;
		StringSource(message + mac, true, 
			new HashVerificationFilter(hmac, NULL, flags)
			); // StringSource
		cout << "Verified message" << endl;
		return true;
}
	catch(const CryptoPP::Exception& e)
	{
    cerr << e.what() << endl;
	return false;
	}
}

void CCipherSuite::DSA_Sign( string message )
{
	try
	{
		AutoSeededRandomPool prng;

		
		DSA::Signer signer( user_dsa_private_key_ );
		
		StringSource( message, true, 
			new SignerFilter( prng, signer,
			new StringSink( dsa_signature_ )
			) // SignerFilter
			); // StringSource

		//signature[0] |= 0x0F;
        //message[0] |= 0x0F;
	}
	catch( CryptoPP::Exception& e )
	{
		cerr << e.what() << endl;
		exit(1);
    }
return;
}


void CCipherSuite::GenerateDSAkey()
{
	try
	{
		AutoSeededRandomPool prng;
		user_dsa_private_key_.GenerateRandomWithKeySize(prng, 1024);
   
		user_dsa_public_key_.AssignFrom(user_dsa_private_key_);
		
		if (!user_dsa_private_key_.Validate(prng, 3) || !user_dsa_public_key_.Validate(prng, 3))
		{
			throw runtime_error("DSA key generation failed");
		}
	}
    catch( CryptoPP::Exception& e )
    {
        cerr << e.what() << endl;
		exit(1);
    }
	return;
}

	
bool CCipherSuite::DSA_Verify(string message, string signature)
{
	try
	{
		server_dsa_public_key_=user_dsa_public_key_;

		DSA::Verifier verifier( server_dsa_public_key_ );

		StringSource( message+signature, true,
			new SignatureVerificationFilter(
			verifier, NULL, SignatureVerificationFilter::THROW_EXCEPTION
        /* SIGNATURE_AT_END */
		)
		);
		cout << "Verified signature on message" << endl;
		return true;
	}
	catch( CryptoPP::Exception& e )
	{
        cerr << e.what() << endl;
		return false;
    }
}

string CCipherSuite::GetCipherMessage()
{
	return cipher_message_;
}

string CCipherSuite::GetPlainMessage()
{
	return plain_message_;
}

string CCipherSuite::GetRSAplain()
{
	return rsa_plain_;
}

string CCipherSuite::GetRSAcipher()
{
	return rsa_cipher_;
}
	
RSA::PublicKey CCipherSuite::GetRSApublicKey()
{
	return  user_rsa_public_key_;
}
	
void CCipherSuite::SetServerRSApublicKey(RSA::PublicKey publickey)
{
	server_rsa_public_key_=publickey;
	return;
}
	
void CCipherSuite::SetMessageKey(byte messagekey[], byte messageiv[])
{

	memcpy(message_aes_key_,messagekey,AES::DEFAULT_KEYLENGTH);
	memcpy(message_aes_iv_,messageiv,AES::BLOCKSIZE);
	return;
}
	
string CCipherSuite::GetSHAkey()
{
	string encoded;
	encoded.erase();
	StringSource ss1(sha_key_, sha_key_.size(), true,
		new HexEncoder(
        new StringSink(encoded)
		) // HexEncoder
		); // StringSource
	return encoded;
}
	
string CCipherSuite::GetHmac()
{
	return sha_hmac_;
}
	
void CCipherSuite::SetServerDSApublicKey(DSA::PublicKey key)
{
	server_dsa_public_key_=key;
	return;
}

DSA::PublicKey CCipherSuite::GetDSApublicKey()
{
	return user_dsa_public_key_;
}
	
string CCipherSuite::GetSignature()
{
	return dsa_signature_;
}

string CCipherSuite::Encoded(string str)
{
	string encoded;
	StringSource ss( str, true,
		new HexEncoder(
		new StringSink( encoded )) // HexEncoder
		); // StringSource
	return encoded;
}



