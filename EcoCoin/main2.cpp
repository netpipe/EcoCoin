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
#define headlessGUI //define for console mode only

#include "sqlCon.h"
//#include <irrlicht.h>
//#include <irrNet.h>
#include "md5sum.h"
#include "address.h"

#include "crypto/TestHelper.hpp"
#include "crypto/Ecdsa.hpp"
#include "crypto/Sha256Hash.hpp"
#include "crypto/Uint256.hpp"
#include "coingen.h"

static int numTestCases = 0;



//create 5 databas scale to 50 later divide the ammount of coins by databases and search for coin address
//fill with coin addresses and main wallet info

// sign txid with private key use public key(wallet address) to decode it and verify
//hash tx with coin addresses to produce a txid file

//txid file has value, coins (random generated addresses ? or just coin numbers) hash of file , publickey, timestamp , fee, send to walletid

//int system(const char *command); //md5 file

//using namespace irr;
//using namespace core;
//using namespace scene;
//using namespace video;
//using namespace io;
//using namespace gui;


void handleConnection();
bool doit;
bool connected,authorized;

//vector3df tmpvect;
//	u32 address = netManager->getClientAddress(playerId);
#ifdef irrlicht
class ServerNetCallback : public net::INetCallback
{
 public:
	ServerNetCallback(net::INetManager* netManagerIn) : netManager(netManagerIn) {}

	// Override the "onConnect" function, don't forget to get the method
	// signature correct. Whenever a fresh client connects, their brand
	// spanking new player id will be passed to this function. A "u16"
	// is a typedef for an unsigned short, incase you were wondering.
	virtual void onConnect(const u16 playerId)
	{
	    connected=1;
        printf("connected - sending handshake \n");
			net::SOutPacket packet;
		//	core::stringc message;
		//	message = "first";
			//message += playerId;
			//message += " has just connected.";
            packet << "first";
            netManager->sendOutPacket(packet);
		//	netManager->sendOutPacket(packet);
	}

	virtual void onDisconnect(const u16 playerId)
	{
	    doit=0;
	    connected=0;
		net::SOutPacket packet;
		core::stringc message;
		message = "Client number ";
		message += playerId;
		message += " has just left the building.";
		packet << message;
		netManager->sendOutPacket(packet);
		doit,connected = false;
	}

	virtual void handlePacket(net::SInPacket& packet)
	{
        core::stringc str;
		packet >> str;
        printf("handling packet \n");
     //   core::vector3df yes;
	//	packet >> yes;

            if (str == "first"){
                connected=true; // do stuff were authed
                std::cout << "CONNECTED "  << str.c_str() <<std::endl;
            }

            if (str == "second"){
                connected=true; // do stuff were authed
                 doit=true; // do stuff were authed
                std::cout << "second2connECTED "  << str.c_str() <<std::endl;
            }
            if (str == "userLogin"){
                doit=true; // do stuff were authed

                std::cout << "AUTHORIZED with " << str.c_str() <<std::endl;
            }
            if (str.c_str() == "actPack2"){
                               doit=true;
                std::cout <<"Auth Communications 2" << str.c_str() <<std::endl;
            }else{
                std::cout <<"random string" << str.c_str() <<std::endl;
            }

        //    tmpvect = vector3df(yes);

	}
private:
	core::array<u32> banList;
	net::INetManager* netManager;
};
#endif
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
#ifndef headlessGUI
                    std::cout << "Create IrrDevice " ;
	IrrlichtDevice *device = createDevice( video::EDT_OPENGL, dimension2d<s32>(640, 480), 16,
                                            false, false, false, 0);

                                            if (!device)return 1;

	device->setWindowCaption(L"Hello World! - Irrlicht Engine Demo");

	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();
	guienv->addStaticText(L"Server!",
		rect<s32>(10,10,260,22), true);

	IAnimatedMesh* mesh = smgr->getMesh(".media/sydney.md2");
	if (!mesh)return 1;

	IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode( mesh );
	if (node)
	{   node->setMaterialFlag(EMF_LIGHTING, false);
		node->setMD2Animation(scene::EMAT_STAND);
		node->setMaterialTexture( 0, driver->getTexture("./media/sydney.bmp") );
	}
	smgr->addCameraSceneNode(0, vector3df(0,30,-40), vector3df(0,5,0));

    //log << std::string("loop");
		//  The default port that clients can connect to is set to 45000.
		net::INetManager* netManager = net::createIrrNetServer(0);
		ServerNetCallback* serverCallback = new ServerNetCallback(netManager);
		netManager->setNetCallback(serverCallback);
          netManager->setVerbose(true);
#endif

    //! TEST AREA FOR SQL
std::string a;
    sqlCon *sq =new sqlCon("ha.db");
    //  sq->execute(".dump");
          //  sq->execute("PRAGMA table_info(Coins);"); //testing

		int createcoins=0;
		if ( createcoins==1 ){
			sq->execute("CREATE TABLE Coins (coin integer, wallet string);");
			for (int coin=1;coin < 100 ;++coin){
				std::string test = "insert into Coins (coin,wallet) VALUES(" + std::to_string(coin) + " ,'ssssnipples');";
				//printf("%s",(char*)chr);
				const char *chr = test.c_str();
				sq->execute((char*)chr);
			}
		}

		// either scan through db to build coin list or remember what coins belong to your wallet
		// search several dabatase ranges
		  sq->execute("SELECT * FROM Coins where wallet like '%ssssnipples%';");
		//a=   sq->search("SELECT * FROM Coins where wallet like '%ssssnipples%';");

//	sq->execute("insert into Coins (coin,wallet) VALUES(35,'nameo');");
       // sq->execute("INSERT INTO Coins (a,b) VALUES(35 , 'nameo');");
    //  a=   sq->execute("SELECT * FROM Coins where coin like '35%';");

	//a = sq->execute("SELECT a FROM Coins where y like '%nameo%';");
    //sq->execute("FLOOR(DATEDIFF(CURRENT_DATE, hire_date) / 365) b)";
    //DATEDIFF(year, hire_date,CURRENT_TIMESTAMP)
    //  sq->execute("SELECT * FROM learn");
    //  sq->execute("PRAGMA table_info(Coins)");
    //  sq->execute("SELECT * FROM sqlite_master WHERE tbl_name = 'Coins' AND type = 'table'");

    //cout << a << endl;


    generateAddress();

	testEcdsaSignAndVerify();
	testEcdsaVerify();
	std::printf("All %d test cases passed\n", numTestCases);

	//brutegen();
#ifndef headlessGUI

    while(device->run())
    {
        // std::cout << "Sleeping for 150 units\n " ;
        // device->sleep(150,0);
#endif
#ifdef net
while(1){
    if(netManager->getConnectionStatus() != net::EICS_FAILED)
    {
                net::SOutPacket packet;// To send a packet, first you create an SOutPacket object.
        if (connected==true)
        {
            if (!authorized){
                if (!doit)
                {
                    std::cout << "tell client to authorize \n " ;
                    packet << "second";
                       //    doit = false; // send only one
                     //    netManager->sendOutPacket(packet);
                }
                else
                {
                    std::cout << "handshake \n " ;
                    packet << "actPack2";
                                     doit = true;
    ////tmpvect.X = 5;
    ////            packet << vector3df(tmpvect);
    ////                      std::cout << "sending X=" << tmpvect.X << "Y=" << tmpvect.Y << "Z=" << tmpvect.Z ;
    ////
    ////            packet << vector3df(tmpvect);
    ////                      std::cout << "sending X=" << tmpvect.X << "Y=" << tmpvect.Y << "Z=" << tmpvect.Z ;
                }

    ////doit = false;
            }else{
            // take some authenticated data
            }
                   netManager->sendOutPacket(packet);
        } else {
           sleep(5);    //   device->sleep(50,0);
        };

            netManager->update(1000); //update(delay);
    }
}
#endif

#ifndef headlessGUI
        driver->beginScene(true, true, SColor(255,100,101,140));
        smgr->drawAll();
        guienv->drawAll();
        driver->endScene();


	device->drop();

    delete netManager;
    delete serverCallback;
    #endif
    return 0;
}
