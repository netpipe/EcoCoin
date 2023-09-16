#include <math.h>

void brutegen(){
//basic demo-bruteforce algorithm in C++ from hacksenkessel.com
    std::string alphabet="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890"; // can randomize string and record as part of the unique coin signature ?
    const int min_pw_len = 1;
    const int max_pw_len = 3;
    char pw[max_pw_len + 2];
    int n, pos = min_pw_len -1, count=0;
    long double max_perms = 0;
    string final;
    /* create last possible password based on max. password length
     * and alphabet,  i.e. 'zzzz' for an alphabet [a-z] with 4 chars
     */
    for (int k=0; k<max_pw_len; k++) final+=alphabet[alphabet.length()-1];
    /* calculate number of permutations to run by interpreting the
     * final password as polynomial. Be careful, this is where an integer
     * overflow can occur by exceeding the max. password length and/or the alphabet
     */
    for (int n=0; n < (int) final.length(); n++)
     max_perms += (long double) ( (long double) (alphabet.find_first_of(string(1, final[n]))+1) * powl(alphabet.length(), final.length()-n-1));
    /* Print out calculated information and set password
     * to first char in alphabet
     */
    cout << "Last password: " << final << endl << "Permutations:  " << max_perms << endl << endl;
    cout << "Starting bruteforce:" << endl;
    memset(pw, '\0', sizeof(pw));
    pw[0] = alphabet[0];
    /* bruteforce until previously calculated max. number
    * of permutations is exceeded */
    while (count < max_perms) {
    /* iterate last password index separately, as we know its number
     * of iterations matches the alphabet's length
     */
         for (int a=0;  a < (int) alphabet.length(); a++) {
            pw[pos] = alphabet[a];
            cout << pw << endl;
            if (count++ >= max_perms) break;
         }
  /* iterate remaining indexes in descending order, as in
   * all indexes except for the last index we already iterated
   */
  for (n = pos; n>=0; n--) {
    if (pw[n] != alphabet[alphabet.length() -1]) {
      pw[n]++;
      break;
    }
    else {
      if (n == 0) {
        /* increase password length */
        memset(pw, (int) alphabet[0], ++pos + 1);
        break;
      }
      /* re-initialize current password index */
      pw[n] = alphabet[0];
    }
  }
}
}



//void MainWindow::GenerateCoins1() //not used
//{   //bruteforce algo by Author: Neo_Fr


//    //bruteforce generate coin numbers
//    //randomize indexs after backing up into coinlist.db into another db called freecoins.db
//    //stop after set ammount of coins have been generated

//    int LongMin;
//    int LongMax;
//    unsigned long nbMDP = 0;

//    //const char ALL[105] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvwxyz1234567890<>,?;.:/!§*µù%$£¤¨+=})]à@ç^\\_`è|-[({'#\"é~&";
//    const char ALL[105] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890"; // can randomize string and record as part of the unique coin signature ?
//    int Lg = LongMin - 1;
//    int i, x, y;

//    char* Buff = (char*) malloc(Lg); // Creer le Buff de Lg octet
//    while(Lg != LongMax)
//    {
//        realloc(Buff, Lg); // Augmente la taille du buff
//        int Nchar[Lg];
//        for(i=0; i<=Lg; i++)
//        {
//            Buff[i] = ALL[0]; // Remplie le Buff du premier char
//            Nchar[i] = 0; // Initialise le tableau de int a 0
//        }
//        while(Nchar[0] != 105)
//        {
//             for(x=0; x<=105; x++)
//             {
//                     Buff[Lg] = ALL[x];
//                     Nchar[Lg] = x;
//                     printf("%s \n", Buff);
//                     nbMDP++;
//             }
//             for(y=Lg; y>=0; y--)
//             {
//                 if((Nchar[y] == 105)&&(Nchar[0]!= 105))
//                 {
//                     Nchar[y] = 0;
//                     Nchar[y-1]++;
//                 }
//                 Buff[y] = ALL[Nchar[y]];
//             }
//       }
//       Lg++;
//    }
//    free(Buff);// Libere la memoire
//}

