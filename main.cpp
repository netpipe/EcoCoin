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
#include <irrlicht.h>
#include <irrNet.h>
#include "md5sum.h"


//create 5 databas scale to 50 later divide the ammount of coins by databases and search for coin address
//fill with coin addresses and main wallet info

//int system(const char *command); //md5 file

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


void handleConnection();
bool doit;
bool connected,authorized;

vector3df tmpvect;
//	u32 address = netManager->getClientAddress(playerId);

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



int main()
{
#ifndef headlessGUI
                    std::cout << "Create IrrDevice " ;
	IrrlichtDevice *device = createDevice( video::EDT_BURNINGSVIDEO, dimension2d<s32>(640, 480), 16,
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
#endif
    //log << std::string("loop");
		//  The default port that clients can connect to is set to 45000.
		net::INetManager* netManager = net::createIrrNetServer(0);
		ServerNetCallback* serverCallback = new ServerNetCallback(netManager);
		netManager->setNetCallback(serverCallback);
          netManager->setVerbose(true);


    //! TEST AREA FOR SQL
//#define noSQL
#ifndef noSQL
std::string a;
    sqlCon *sq =new sqlCon("ha.db");
    //  sq->execute(".dump");
     // needs error management if table exists it crashse
          //  sq->execute("PRAGMA table_info(Coins);"); //testing
	//	sq->execute("CREATE TABLE Coins (a integer, b integer,y string);");
    //  sq->execute("insert into Coins (a,b,y) VALUES(33 , 34,'ssssnipples');");
      //  sq->execute("INSERT INTO Coins (a,b) VALUES(33 , 34);");
        sq->execute("SELECT * FROM Coins where b like '34%';");
	//a = sq->execute("SELECT a FROM Coins where y like '%nameo%';");
    //sq->execute("FLOOR(DATEDIFF(CURRENT_DATE, hire_date) / 365) b)";
    //DATEDIFF(year, hire_date,CURRENT_TIMESTAMP)
    //  sq->execute("SELECT * FROM learn");
    //  sq->execute("PRAGMA table_info(Coins)");
    //  sq->execute("SELECT * FROM sqlite_master WHERE tbl_name = 'Coins' AND type = 'table'");

//    cout << a << endl;
#endif


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
#endif
    delete netManager;
    delete serverCallback;
    return 0;
}