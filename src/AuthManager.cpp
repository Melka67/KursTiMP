#include "AuthManager.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <openssl/sha.h>
#include <stdexcept>
#include <iostream>

AuthManager::AuthManager(const std::string& clientDbFile) : filename(clientDbFile) {
    loadClients();
}

bool AuthManager::loadClients() {
    clients.clear();
    
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        size_t pos = line.find(':');
        if (pos != std::string::npos) {
            std::string login = line.substr(0, pos);
            std::string password = line.substr(pos + 1);
            clients[login] = password;
        }
    }
    
    file.close();
    return true;
}

std::string AuthManager::computeSHA224(const std::string& data) {
    unsigned char hash[SHA224_DIGEST_LENGTH];
    SHA224(reinterpret_cast<const unsigned char*>(data.c_str()), data.length(), hash);
    
    std::stringstream ss;
    for (int i = 0; i < SHA224_DIGEST_LENGTH; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }
    
    return ss.str();
}

std::string AuthManager::hexToBinary(const std::string& hex) {
    std::string binary;
    for (size_t i = 0; i < hex.length(); i += 2) {
        std::string byteString = hex.substr(i, 2);
        char byte = static_cast<char>(std::stoul(byteString, nullptr, 16));
        binary += byte;
    }
    return binary;
}

bool AuthManager::authenticate(const std::string& login, const std::string& salt, const std::string& clientHash) {
    auto it = clients.find(login);
    if (it == clients.end()) {
        std::cout << "DEBUG: User not found: " << login << std::endl;
        return false;
    }
    
    std::string password = it->second;
    
    std::cout << "DEBUG AUTH:" << std::endl;
    std::cout << "  Login: " << login << std::endl;
    std::cout << "  Password: " << password << std::endl;
    std::cout << "  Salt (hex): " << salt << std::endl;
    
    // Преобразуем hex-соль в бинарный формат
    std::string saltBinary = hexToBinary(salt);
    
    // Вычисляем хэш: SHA-224(salt_binary + password)
    std::string data = saltBinary + password;
    std::string serverHash = computeSHA224(data);
    
    // Сравниваем хэши (регистронезависимо)
    std::string clientHashUpper = clientHash;
    std::string serverHashUpper = serverHash;
    
    for (char& c : clientHashUpper) c = std::toupper(c);
    for (char& c : serverHashUpper) c = std::toupper(c);
    
    std::cout << "  Client hash: " << clientHashUpper << std::endl;
    std::cout << "  Server hash: " << serverHashUpper << std::endl;
    std::cout << "  Match: " << (clientHashUpper == serverHashUpper ? "YES" : "NO") << std::endl;
    
    return (clientHashUpper == serverHashUpper);
}

void AuthManager::setClientDbFile(const std::string& filename) {
    this->filename = filename;
    loadClients();
}