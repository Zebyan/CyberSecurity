#include <iostream>
#include <list>
#include <cstring>
#include <crypt.h>
#include <unistd.h>
#include <algorithm>

//The changes i make:
//1.Change the Max_LEN
//2.Rewrite the for that adds one character at the time to instead add 1 word at the time
//3.Change charset into an array of strings
//4.Use a variable word as an index for the words
//5.Change the way passwords were generated,instead of adding characters,we add words(array of chars)
//The password that i recovered was: blueredgreenpinkorange


using namespace std;

//this is an example line from the shadow file:
//$6$Iy/hHRfM$gC.Fw7CbqG.Qc9p9X59Tmo5uEHCf0ZAKCsPZuiYUKcejrsGuZtES1VQiusSTen0NRUPYN0v1z76PwX2G2.v1l1:15001:0:99999:7:::
// the salt and password values are extracted as

string target_salt = "$6$9kfonWC7$";
string target_pw_hash = "$6$9kfonWC7$gzqmM9xD7V3zzZDo.3Fb5mAdM0GbIR2DYTtjYpcGkXVWatTC0pa/XVvKTXLb1ZP0NG9cinGRZF7gPLdhJsHDM/";

// define a null string which is returned in case of failure to find the password
char null[] = {'\0'};

// define the maximum length for the password to be searched
#define MAX_LEN 50//i asummed the maximum lenght of the password is 50,the current charset has 25 characters

list<char*> pwlist;

// check if the pw and salt are matching the hash
int check_password(char* pw, char* salt, char* hash)
{
    char* res = crypt (pw, salt);
    cout << "password " << pw << "\n";
    cout << "hashes to " << res << "\n";
    for (int i = 0; i<strlen(hash); i++)
        if (res[i]!=hash[i])
            return 0;
    cout << "match !!!" << "\n";
    return 1;
}

// builds passwords from the given character set
// and verifies if they match the target
char* exhaustive_search(const char* charset[],int word, char* salt, char* target)
{
    char* current_password;
    char* new_password;
    int i, current_len;
    
    // begin by adding each word
    for (i = 0; i<word; i++)
    {
        new_password = new char [strlen(charset[i])+1];//add the size of the word plus null at the end
        strcpy(new_password,charset[i]);//new password becomes the word coresponding to the index i
        pwlist.push_back(new_password);
    }
    
    while(true){
        
        // test if queue is not empty and return null if so
        if (pwlist.empty())
            return null;
        
        // get the current current_password from queue
        current_password = pwlist.front();
        current_len = strlen(current_password);
        
        //  check  if  current  password  is  the  target  password,  if  yes  return  the current_password
        if (check_password(current_password, salt, target))
            return current_password;
        
        if(current_len < MAX_LEN){
            for (i = 0; i < word; i++){
                new_password = new char[current_len + strlen(charset[i]) + 1];//add the coresponding size
                strcpy(new_password, current_password);
                strcat(new_password, charset[i]);//add the otherwords in the password
                pwlist.push_back(new_password);
            }
        }
        // now remove the front element as it didn't match the password
        pwlist.pop_front();
    }
}

main()
{
    char* salt;
    char* target;
    char* password;
    
    // define the character set from which the password will be built
    const char* charset[] = {"red","green","blue","orange","pink"};//change charset to an array of strings
    int word = sizeof(charset)/sizeof(charset[0]);//create an int that will point at the worlds
    //convert the salt from string to char*
    salt = new char[target_salt.length()+1];
    copy(target_salt.begin(), target_salt.end(), salt);
    //convert the hash from string to char*
    target = new char[target_pw_hash.length()+1];
    copy(target_pw_hash.begin(), target_pw_hash.end(), target);
    //start the search
    password = exhaustive_search(charset,word, salt, target);
    if  (strlen(password)!= 0)
        cout  <<  "Password  successfuly  recovered:  "  << password << "  \n";
    else
        cout << "Failure to find password, try distinct character set of size \n";
}
