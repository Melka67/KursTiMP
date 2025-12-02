#ifndef AUTH_H
#define AUTH_H

#include <string>

class Auth {
public:
    static bool authenticate(int clientSocket);
    
private:
    static bool validateFormat(const std::string& login, 
                               const std::string& salt, 
                               const std::string& hash);
    static bool verifyCredentials(const std::string& login,
                                 const std::string& salt,
                                 const std::string& receivedHash);
};

#endif