#include <unistd.h>
#include <cstdio>
#include <string>
#include <stdio.h>
#include <iostream>
#include <memory.h>

#ifdef WIN32
#include <windows.h>
#endif

#include <iostream>

#include "sqlCon.h"
#include "md5sum.h"
#include "address.h"

#include "crypto/TestHelper.hpp"
#include "crypto/Ecdsa.hpp"
#include "crypto/Sha256Hash.hpp"
#include "crypto/Uint256.hpp"

static int numTestCases = 0;



//create 5 databas scale to 50 later divide the ammount of coins by databases and search for coin address
//fill with coin addresses and main wallet info

// sign txid with private key use public key(wallet address) to decode it and verify
//hash tx with coin addresses to produce a txid file

//txid file has value, coins (random generated addresses ? or just coin numbers) hash of file , publickey, timestamp , fee, send to walletid

//int system(const char *command); //md5 file


//	u32 address = netManager->getClientAddress(playerId);

static void testEcdsaSignAndVerify() {
	// Define test cases
	struct SignCase {
		bool matches;
		const char *privateKey;
		const char *msgHash;  // Byte-reversed
		const char *nonce;    // Can be null
		const char *expectedR;
		const char *expectedS;
	};
	const vector<SignCase> cases{
		// Hand-crafted cases
		{true, "0000000000000000000000000000000000000000000000000000000000000123",
		"8900000000000000000000000000000000000000000000000000000000000000",
		"0000000000000000000000000000000000000000000000000000000000000457",
		"28B7F3A019749CCE6FC677AFA8FAE72EC10E811ED4B04E1963143CEF87654B75",
		"04719F34FE9A47F2C9A22045485F3654DC3AC4A910A7B0B4C7A318F41DB65C9B"}//,
//		{true, "8B46893E711C8948B28E7637BFBED61666E0118ED4D361BED1F18058214C69B8",
//		"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF",
//		"D9063703D9F719739FF645C77BA2F9D1DD2B4254DC7B001F8FC77C3B05AEF5B1",
//		"B4508AF745210F6702C687682FD5E8C8D99CD1C6A7AD450AB4640458E14474BA",
//		"421ED1256C6056D50A481D76B77CF5AA74A692556682E584A4872E8D8BBBCEAC"},
//		{true, "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364140",
//		"B8EF4E4640FBBD526166FF260EB65EC2B3B60064CCE2DA9747201BA201E90F7F",
//		"E2EF043987542CD685482E71E57D01A93B701C2610FB03253871DC2958AD3D73",
//		"38C2AB97F778D0E1E468B3A7EBEBD2FB1C45678B62DD01587CF54E298C71EC43",
//		"7A6D055110F4296A85E8122B32F87AD32A58CF9BB73435088102638D7DECE1F2"}
	};

	// Test runner
	for (const SignCase &tc : cases) {
		Uint256 privateKey(tc.privateKey);
		const Sha256Hash msgHash(tc.msgHash);
		Uint256 expectedR(tc.expectedR);
		Uint256 expectedS(tc.expectedS);

		Uint256 r, s;
		bool ok;
		if (tc.nonce != nullptr) {
			Uint256 nonce(tc.nonce);
			ok = Ecdsa::sign(privateKey, msgHash, nonce, r, s);
		} else {
			ok = Ecdsa::signWithHmacNonce(privateKey, msgHash, r, s);
		}
		bool actualMatch = r == expectedR && s == expectedS;
		assert(ok && actualMatch == tc.matches);

		if (Uint256::ZERO < privateKey && privateKey < CurvePoint::ORDER) {
			CurvePoint publicKey = CurvePoint::privateExponentToPublicPoint(privateKey);
			assert(Ecdsa::verify(publicKey, msgHash, r, s));
		}

		numTestCases++;
	}
}

static void testEcdsaVerify() {
	struct VerifyCase {
		bool answer;
		const char *pubPointX;
		const char *pubPointY;
		const char *msgHash;  // Byte-reversed
		const char *rValue;
		const char *sValue;
	};
	const vector<VerifyCase> cases{
		{false, "77D9ECB1D22A45C107EE36FC6D62A4D32BAB6689A50F0FAE587E0B95A795E833",
		"9BB5CF3051C7FCD5B69CB80A59B052D75BB6C6090B28C1E5AC0C6502B04BE63B",
		"EF54D03E7453CED1A0A9529ADFBE46CE7440E40E3457CA1C040B6CAC9E3209E4",
		"EB4E0C2C1723EFE8192F2F8743D343F45B5B8A9A12012EE71743247B0F65DAD8",
		"08F4E06799E5919F72EE39D3473EB473BD8ADC672694D895734E8AE4D049E038"},
	};

	for (const VerifyCase &tc : cases) {
		CurvePoint publicKey(tc.pubPointX, tc.pubPointY);
		const Sha256Hash msgHash(tc.msgHash);
		Uint256 r(tc.rValue);
		Uint256 s(tc.sValue);
		assert(Ecdsa::verify(publicKey, msgHash, r, s) == tc.answer);
		numTestCases++;
	}
}

int main()
{

    //! TEST AREA FOR SQL
    std::string a;
    sqlCon *sq =new sqlCon("coins.db");
    //  sq->execute(".dump");
          //  sq->execute("PRAGMA table_info(Coins);"); //testing

		int createcoins=1;
		if ( createcoins==1 ){
			sq->execute("CREATE TABLE Coins (coin integer, wallet string);");
			for (int coin=1;coin < 20 ;++coin){
				std::string test = "insert into Coins (coin,wallet) VALUES(" + std::to_string(coin) + " ,'ssssnipples');";
//				printf("%s",(char*)chr);
				const char *chr = test.c_str();
				sq->execute((char*)chr);
			}
		}

		// either scan through db to build coin list or remember what coins belong to your wallet
		// search several dabatase ranges
		//  sq->execute("SELECT * FROM Coins where wallet like '%ssssnipples%';");
		a=   sq->search("SELECT * FROM Coins where wallet like '%ssssnipples%';");

//	sq->execute("insert into Coins (coin,wallet) VALUES(35,'nameo');");
       // sq->execute("INSERT INTO Coins (a,b) VALUES(35 , 'nameo');");
    //  a=   sq->execute("SELECT * FROM Coins where coin like '35%';");

	//a = sq->execute("SELECT a FROM Coins where y like '%nameo%';");
    //sq->execute("FLOOR(DATEDIFF(CURRENT_DATE, hire_date) / 365) b)";
    //DATEDIFF(year, hire_date,CURRENT_TIMESTAMP)
    //  sq->execute("SELECT * FROM learn");
    //  sq->execute("PRAGMA table_info(Coins)");
    //  sq->execute("SELECT * FROM sqlite_master WHERE tbl_name = 'Coins' AND type = 'table'");

    cout << a << endl;


   // generateAddress();

	//testEcdsaSignAndVerify();
	//testEcdsaVerify();
	std::printf("All %d test cases passed\n", numTestCases);

    return 0;
}
