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
//#include "address.h"

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

class CryptoBank {
public:
    CryptoBank() {
        initDatabase();
    }

    void createAccount(const std::string& username, const std::string& publicKey) {
        std::string sql = "INSERT INTO accounts (username, public_key, balance) VALUES ('" + username + "', '" + publicKey + "', 0);";
        executeSQL(sql);
    }

    void deposit(const std::string& username, double amount) {
        std::string sql = "UPDATE accounts SET balance = balance + " + std::to_string(amount) + " WHERE username = '" + username + "';";
        executeSQL(sql);
    }

    void transfer(const std::string& fromUser, const std::string& toUser, double amount) {
        std::string sql = "UPDATE accounts SET balance = balance - " + std::to_string(amount) + " WHERE username = '" + fromUser + "';";
        executeSQL(sql);
        sql = "UPDATE accounts SET balance = balance + " + std::to_string(amount) + " WHERE username = '" + toUser + "';";
        executeSQL(sql);
    }

    void printBalance(const std::string& username) {
        std::string sql = "SELECT balance FROM accounts WHERE username = '" + username + "';";
        executeSQL(sql);
    }

private:
    sqlite3* db;

    void initDatabase() {
        if (sqlite3_open("bank.db", &db)) {
            std::cerr << "Error opening database" << std::endl;
            return;
        }
        std::string sql = "CREATE TABLE IF NOT EXISTS accounts (id INTEGER PRIMARY KEY, username TEXT UNIQUE, public_key TEXT, balance REAL);";
        executeSQL(sql);
    }

    void executeSQL(const std::string& sql) {
        char* errMsg = nullptr;
        if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
            std::cerr << "SQL Error: " << errMsg << std::endl;
            sqlite3_free(errMsg);
        }
    }
};

int main()
{
    CryptoBank bank;
    bank.createAccount("Alice", "AlicePublicKey");
    bank.createAccount("Bob", "BobPublicKey");
    bank.deposit("Alice", 100);
    bank.transfer("Alice", "Bob", 50);
    bank.printBalance("Alice");
    bank.printBalance("Bob");



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
