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


void CCipherSuite::AES_EncryptFile(string ofilename, string efilename)
{

	ofile=ofilename;
	efile=efilename;
//	FileAESkey=key;
//	memcpy(FileAESiv,iv,AES::BLOCKSIZE*sizeof(byte));

	string encoded;
		// Pretty print key
	encoded.erase();
	StringSource(FileAESkey, sizeof(FileAESkey), true,
		new HexEncoder(
			new StringSink(encoded)
		) // HexEncoder
	); // StringSource
	cout << "key: " << encoded << endl;


	// Pretty print iv
	encoded.erase();
	StringSource(FileAESiv, sizeof(FileAESiv), true,
		new HexEncoder(
			new StringSink(encoded)
		) // HexEncoder
	); // StringSource
	cout << "iv: " << encoded << endl;


	try
{
    CBC_Mode< AES >::Encryption e;
    e.SetKeyWithIV( FileAESkey, FileAESkey.size(), FileAESiv );

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



void CCipherSuite::AES_DecryptFile(string efilename, string rfilename, SecByteBlock key,  byte iv[])
{

	efile=efilename;
	rfile=rfilename;
	FileAESkey=key;
	memcpy(FileAESiv,iv,AES::BLOCKSIZE*sizeof(byte));

	string encoded;
		// Pretty print key
	encoded.erase();
	StringSource(FileAESkey, sizeof(FileAESkey), true,
		new HexEncoder(
			new StringSink(encoded)
		) // HexEncoder
	); // StringSource
	cout << "key: " << encoded << endl;

	// Pretty print iv
	encoded.erase();
	StringSource(FileAESiv, sizeof(FileAESiv), true,
		new HexEncoder(
			new StringSink(encoded)
		) // HexEncoder
	); // StringSource
	cout << "iv: " << encoded << endl;



try
{
    CBC_Mode< AES >::Decryption d;
    d.SetKeyWithIV( key, key.size(), iv );

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



void CCipherSuite::AES_EncryptMessage(string plain)
{
	
try
{
    cout << "plain text: " << plain << endl;

    CBC_Mode< AES >::Encryption e;
    e.SetKeyWithIV( MessageAESkey, MessageAESkey.size(), MessageAESiv );

    // The StreamTransformationFilter adds padding
    //  as required. ECB and CBC Mode must be padded
    //  to the block size of the cipher.
   StringSource ss( plain, true, 
        new StreamTransformationFilter( e,
            new StringSink( ciphermessage ),BlockPaddingSchemeDef::BlockPaddingScheme::ONE_AND_ZEROS_PADDING,
						true)
         // StreamTransformationFilter     
    ); // StringSource
}
catch( const CryptoPP::Exception& e )
{
    cerr << e.what() << endl;
    exit(1);
}

string encoded;
// Pretty print cipher text
StringSource ss( ciphermessage, true,
    new HexEncoder(
        new StringSink( encoded )
    ) // HexEncoder
); // StringSource
cout << "cipher text: " << encoded << endl;

return;
}


void CCipherSuite::AES_DecryptMessage(string cipher)
{
	string encoded;
	// Pretty print cipher text
StringSource ss( ciphermessage, true,
    new HexEncoder(
        new StringSink( encoded )
    ) // HexEncoder
); // StringSource

	cout << "cipher text: " << encoded << endl;

	try
{
    CBC_Mode< AES >::Decryption d;
    d.SetKeyWithIV( MessageAESkey, MessageAESkey.size(), MessageAESiv );

    // The StreamTransformationFilter removes
    //  padding as required.
    StringSource ss( cipher, true, 
        new StreamTransformationFilter( d,
            new StringSink( plainmessage ),BlockPaddingSchemeDef::BlockPaddingScheme::ONE_AND_ZEROS_PADDING,
						true)
         // StreamTransformationFilter
	
    ); // StringSource



    cout << "recovered text: " << plainmessage << endl;
}
catch( const CryptoPP::Exception& e )
{
    cerr << e.what() << endl;
    exit(1);
}




return;
}





void CCipherSuite::SHA_HMAC(string message)
{

string plain=message;
string mac, encoded;

/*********************************\
\*********************************/


try
{
    HMAC< SHA256 > hmac(SHAkey, SHAkey.size());

    StringSource ss2(plain, true, 
        new HashFilter(hmac,
            new StringSink(mac)
        ) // HashFilter      
    ); // StringSource
}
catch(const CryptoPP::Exception& e)
{
    cerr << e.what() << endl;
    exit(1);
}

/*********************************\
\*********************************/

SHAhmac=mac;
// Pretty print
encoded.erase();
StringSource ss3(mac, true,
    new HexEncoder(
        new StringSink(encoded)
    ) // HexEncoder
); // StringSource

cout << "hmac: " << encoded << endl;


return;
}


bool CCipherSuite::SHA_Verify(string message, string mac, SecByteBlock key)
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

void CCipherSuite::DSA_Sign(string message)
{
	 try
    {

	AutoSeededRandomPool prng;


	string signature;
DSA::Signer signer( UserDSAprivateKey );
StringSource( message, true, 
    new SignerFilter( prng, signer,
        new StringSink( signature )
    ) // SignerFilter
); // StringSource


//signature[0] |= 0x0F;
          //   message[0] |= 0x0F;



DSAsignature=signature;

    }
    catch( CryptoPP::Exception& e )
    {

        cerr << e.what() << endl;
    }



return;
}



void CCipherSuite::GenerateDSAkey()
{
		 try
    {

	AutoSeededRandomPool prng;

	
   UserDSAprivateKey.GenerateRandomWithKeySize(prng, 1024);
   
   // Generate Public Key   
   
   UserDSApublicKey.AssignFrom(UserDSAprivateKey);
   if (!UserDSAprivateKey.Validate(prng, 3) || !UserDSApublicKey.Validate(prng, 3))
   {
      throw runtime_error("DSA key generation failed");
   }

    }
    catch( CryptoPP::Exception& e )
    {
        cerr << e.what() << endl;
    }




	return;
}

	bool CCipherSuite::DSA_Verify(string message, string signature)
	{

			 try
    {
ServerDSApublicKey=UserDSApublicKey;

DSA::Verifier verifier( ServerDSApublicKey );
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





	void CCipherSuite::GenerateFileKey()
	{

		
AutoSeededRandomPool prng;

//AES

FileAESkey=SecByteBlock(AES::DEFAULT_KEYLENGTH);
prng.GenerateBlock( FileAESkey, FileAESkey.size() );
prng.GenerateBlock( FileAESiv, sizeof(FileAESiv) );




return;
	}


	void CCipherSuite::GenerateSHAkey()
		{

AutoSeededRandomPool prng;
		SHAkey=SecByteBlock(16);
prng.GenerateBlock(SHAkey, SHAkey.size());

string encoded;
// Pretty print key
encoded.erase();
StringSource ss1(SHAkey, SHAkey.size(), true,
    new HexEncoder(
        new StringSink(encoded)
    ) // HexEncoder
); // StringSource

cout << "key: " << encoded << endl;




		return;
		}



void CCipherSuite::GenerateMessageKey()
{

		
AutoSeededRandomPool prng;

//AES

MessageAESkey=SecByteBlock(AES::DEFAULT_KEYLENGTH);
prng.GenerateBlock( MessageAESkey, MessageAESkey.size() );
prng.GenerateBlock( MessageAESiv, sizeof(MessageAESiv) );



return;
}