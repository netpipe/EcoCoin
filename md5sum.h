//https://stackoverflow.com/questions/1220046/how-to-get-the-md5-hash-of-a-file-in-c
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

#include <openssl/md5.h>

using namespace std;

unsigned char result[MD5_DIGEST_LENGTH];

// function to print MD5 correctly
void printMD5(unsigned char* md, long size = MD5_DIGEST_LENGTH) {
    for (int i=0; i<size; i++) {
        cout<< hex << setw(2) << setfill('0') << (int) md[i];
    }
}

int md5(char *pfile) {

//  if(argc != 2) {
//      cout << "Specify the file..." << endl;
//      return 0;
//  }

  ifstream::pos_type fileSize;
  char * memBlock;

  ifstream file (pfile, ios::ate);

  //check if opened
  if (file.is_open() ) { cout<< "Using file\t"<< pfile <<endl; }
  else {
      cout<< "Unnable to open\t"<< pfile <<endl;
      return 0;
  }

  //get file size & copy file to memory
  //~ file.seekg(-1,ios::end); // exludes EOF
  fileSize = file.tellg();
  cout << "File size \t"<< fileSize << endl;
  memBlock = new char[fileSize];
  file.seekg(0,ios::beg);
  file.read(memBlock, fileSize);
  file.close();

  //get md5 sum
  MD5((unsigned char*) memBlock, fileSize, result);

  //~ cout << "MD5_DIGEST_LENGTH = "<< MD5_DIGEST_LENGTH << endl;
  printMD5(result);
  cout<<endl;

return 0;
}



