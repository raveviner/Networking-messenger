/**
 * this class handles the users
 */

#ifndef CPPPROJECT_COPY_USERS_H
#define CPPPROJECT_COPY_USERS_H


#include <iostream>
#include <string.h>
#include <string>
#include <map>
#include <fstream>
#include <sstream>


using namespace std;

namespace npl{

class Users {

    private:
        map<string,string> usersAndPasswords;
        string filePath;

    public:
    Users(string path);
    map<string,string> getUsersAndPasswords();
    map<string,string> loadAllUserFromFile();
    bool addNewUser(string user,string password);

    };
}

#endif //CPPPROJECT_COPY_USERS_H
