#include <iostream>
#include <list>
#include <cstring>
#include <crypt.h>

using namespace std;

//this is an example line from the shadow file:
//$6$Iy/hHRfM$gC.Fw7CbqG.Qc9p9X59Tmo5uEHCf0ZAKCsPZuiYUKcejrsGuZtES1VQiusSTen0NRUPYN0v1z76PwX2G2.v1l1:15001:0:99999:7:::
// the salt and password values are extracted as

string target_salt = "$6$Iy/hHRfM$";
string target_pw_hash = "$6$Iy/hHRfM$gC.Fw7CbqG.Qc9p9X59Tmo5uEHCf0ZAKCsPZuiYUKcejrsGuZtES1VQiusSTen0NRUPYN0v1z76PwX2G2.v1l1";

// define a null string which is returned in case of failure to find the password
char null[] = {'\0'};

// define the maximum length for the password to be searched
#define MAX_LEN 6

list<char*> pwlist;

// check if the pw and salt are matching the hash
int check_password(char* pw, char* salt, char* hash)
{
    char* res = crypt(pw, salt);
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
char* exhaustive_search(char* charset, char* salt, char* target)
{
    char* current_password;
    char* new_password;
    int i, current_len;
    
    // begin by adding each character as a potential 1 character password
    for (i = 0; i<strlen(charset); i++)
    {
        new_password = new char[2];
        new_password[0] = charset[i];
        new_password[1] = '\0';
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
        //  else  generates  new  passwords  from  the  current  one  by  appending each character from the charlist
        // only if the current length is less than the maxlength
        if(current_len < MAX_LEN){
            for (i = 0; i < strlen(charset); i++){
                new_password = new char[current_len + 2];
                memcpy(new_password, current_password, current_len);
                new_password[current_len] = charset[i];
                new_password[current_len+1] = '\0';
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
    char charset[] = {'b', 'o', 'g', 'd', 'a', 'n', '\0'};
    //convert the salt from string to char*
    salt = new char[target_salt.length()+1];
    copy(target_salt.begin(), target_salt.end(), salt);
    //convert the hash from string to char*
    target = new char[target_pw_hash.length()+1];
    copy(target_pw_hash.begin(), target_pw_hash.end(), target);
    //start the search
    password = exhaustive_search(charset, salt, target);
    if  (strlen(password)!= 0)
        cout  <<  "Password  successfuly  recovered:  "  << password << "  \n";
    else
        cout << "Failure to find password, try distinct character set of size \n";
}
