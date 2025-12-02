#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <unordered_map>

class Database {
public:
    static bool load(const std::string& filename);
    static std::string getPassword(const std::string& login);
    static bool checkUser(const std::string& login, 
                         const std::string& salt, 
                         const std::string& hash);
    
private:
    static std::unordered_map<std::string, std::string> users;
};

#endif