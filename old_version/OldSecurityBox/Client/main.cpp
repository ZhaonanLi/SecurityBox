
#include "CipherSuite.h"

int main(int argc, char* argv[])
{
	//test message
	string msg = "MESSAGEmessage";

	//test files
	string ofilename = "plain.txt";
	string efilename = "cipher.txt";
	string rfilename = "recovered.txt";

	CCipherSuite suite;

	//RSA
	suite.GenerateRSAkey();
	suite.SetServerRSApublicKey(suite.GetRSApublicKey());
	suite.RSA_Encrypt(msg);
	suite.RSA_Decrypt(suite.GetRSAcipher());


	//AES encrypt file
	string userpassword="123456";
	string serversalt="xxxxxxxxxxx";
	suite.GenerateFileKey(userpassword,serversalt);
	suite.AES_EncryptFile(ofilename,efilename);
	suite.AES_DecryptFile(efilename,rfilename);

	//AES message
	suite.GenerateMessageKey();
	suite.AES_EncryptMessage(msg);
	suite.AES_DecryptMessage(suite.GetCipherMessage());


	//HMAC
	string plain = "HMAC Test";
	cout << "plain text: " << plain << endl;
	suite.GenerateSHAkey();
	suite.SHA_HMAC(plain);
	suite.SHA_Verify(plain,suite.GetHmac(),suite.GetSHAkey());

	//DSA
	suite.GenerateDSAkey();
	suite.DSA_Sign(suite.GetHmac());
	suite.DSA_Verify(suite.GetHmac(),suite.GetSignature());



	return 0;
}


