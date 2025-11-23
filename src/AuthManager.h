#ifndef AUTHMANAGER_H
#define AUTHMANAGER_H

#include <string>
#include <unordered_map>
#include <vector>

class AuthManager {
private:
    std::unordered_map<std::string, std::string> clients;
    std::string filename;
    
    std::string computeSHA224(const std::string& data);
    std::string hexToBinary(const std::string& hex);

public:
    AuthManager(const std::string& clientDbFile = "/etc/vcalc.conf");
    bool loadClients();
    bool authenticate(const std::string& login, const std::string& salt, const std::string& clientHash);
    void setClientDbFile(const std::string& filename);
};

#endif