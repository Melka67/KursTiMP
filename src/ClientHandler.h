#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string>
#include "AuthManager.h"
#include "DataProcessor.h"
#include "Logger.h"

class ClientHandler {
private:
    int clientSocket;
    AuthManager& authManager;
    Logger& logger;
    
    bool authenticateClient();
    bool receiveVectors(std::vector<std::vector<double>>& vectors);
    bool sendResults(const std::vector<double>& results);
    
    bool sendMessage(const std::string& message);
    bool receiveMessage(std::string& message, size_t length);
    bool receiveTextMessage(std::string& message); // Добавьте эту строку
    
    template<typename T>
    bool receiveBinary(T& value);
    
    template<typename T>
    bool sendBinary(const T& value);

public:
    ClientHandler(int socket, AuthManager& auth, Logger& log);
    void handleClient();
};

#endif