#include "ClientHandler.h"
#include <cstring>
#include <vector>
#include <arpa/inet.h>
#include <iostream> // для отладки

ClientHandler::ClientHandler(int socket, AuthManager& auth, Logger& log) 
    : clientSocket(socket), authManager(auth), logger(log) {}

template<typename T>
bool ClientHandler::receiveBinary(T& value) {
    return recv(clientSocket, &value, sizeof(T), 0) == sizeof(T);
}

template<typename T>
bool ClientHandler::sendBinary(const T& value) {
    return send(clientSocket, &value, sizeof(T), 0) == sizeof(T);
}

bool ClientHandler::sendMessage(const std::string& message) {
    return send(clientSocket, message.c_str(), message.length(), 0) == static_cast<ssize_t>(message.length());
}

bool ClientHandler::receiveMessage(std::string& message, size_t length) {
    char* buffer = new char[length + 1];
    ssize_t bytesReceived = recv(clientSocket, buffer, length, 0);
    
    if (bytesReceived == static_cast<ssize_t>(length)) {
        buffer[length] = '\0';
        message = std::string(buffer, length);
        delete[] buffer;
        return true;
    }
    
    delete[] buffer;
    return false;
}

bool ClientHandler::receiveTextMessage(std::string& message) {
    char buffer[1024];
    ssize_t bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        message = std::string(buffer, bytesReceived);
        return true;
    }
    
    return false;
}

bool ClientHandler::authenticateClient() {
    // Согласно ТЗ: аутентификация в текстовой форме
    // Клиент отправляет: LOGIN || SALT_16 || HASH_SHA-224 одной строкой
    
    std::string authMessage;
    if (!receiveTextMessage(authMessage)) {
        logger.log(LogLevel::ERROR, "Failed to receive authentication message");
        return false;
    }
    
    // Отладочный вывод
    std::cout << "DEBUG: Received auth message: " << authMessage << std::endl;
    std::cout << "DEBUG: Auth message length: " << authMessage.length() << std::endl;
    
    // Проверяем минимальную длину: "user"(4) + SALT(16) + HASH(56) = 76 символов
    if (authMessage.length() < 76) {
        logger.log(LogLevel::ERROR, "Authentication message too short: " + std::to_string(authMessage.length()));
        sendMessage("ERR");
        return false;
    }
    
    // Извлекаем компоненты
    std::string login = authMessage.substr(0, 4);
    std::string salt = authMessage.substr(4, 16);
    std::string clientHash = authMessage.substr(20, 56);
    
    // Проверяем логин
    if (login != "user") {
        logger.log(LogLevel::ERROR, "Invalid login: " + login);
        sendMessage("ERR");
        return false;
    }
    
    // Валидация формата соли (16 шестнадцатеричных цифр)
    if (salt.find_first_not_of("0123456789ABCDEFabcdef") != std::string::npos) {
        logger.log(LogLevel::ERROR, "Invalid salt format: " + salt);
        sendMessage("ERR");
        return false;
    }
    
    // Валидация формата хэша (56 шестнадцатеричных цифр)
    if (clientHash.find_first_not_of("0123456789ABCDEFabcdef") != std::string::npos) {
        logger.log(LogLevel::ERROR, "Invalid hash format: " + clientHash);
        sendMessage("ERR");
        return false;
    }
    
    logger.log(LogLevel::INFO, "Authentication attempt", 
               "login=" + login + " salt=" + salt + " hash=" + clientHash.substr(0, 16) + "...");
    
    if (authManager.authenticate(login, salt, clientHash)) {
        logger.log(LogLevel::INFO, "Client authenticated successfully");
        return sendMessage("OK");
    } else {
        logger.log(LogLevel::WARNING, "Authentication failed");
        sendMessage("ERR");
        return false;
    }
}

bool ClientHandler::receiveVectors(std::vector<std::vector<double>>& vectors) {
    // Согласно ТЗ: начиная с шага 6 обмен в двоичном формате
    
    uint32_t numVectors;
    if (!receiveBinary(numVectors)) {
        logger.log(LogLevel::ERROR, "Failed to receive number of vectors");
        return false;
    }
    
    numVectors = ntohl(numVectors);
    std::cout << "DEBUG: Number of vectors: " << numVectors << std::endl;
    
    vectors.resize(numVectors);
    
    for (uint32_t i = 0; i < numVectors; ++i) {
        uint32_t vectorSize;
        if (!receiveBinary(vectorSize)) {
            logger.log(LogLevel::ERROR, "Failed to receive vector size");
            return false;
        }
        
        vectorSize = ntohl(vectorSize);
        std::cout << "DEBUG: Vector " << i << " size: " << vectorSize << std::endl;
        
        std::vector<double> vector(vectorSize);
        
        // Receive all values
        for (uint32_t j = 0; j < vectorSize; ++j) {
            uint64_t temp;
            if (!receiveBinary(temp)) {
                logger.log(LogLevel::ERROR, "Failed to receive vector value");
                return false;
            }
            
            // Convert network to host byte order
            temp = be64toh(temp);
            
            // Convert to double using memcpy to avoid strict-aliasing
            double value;
            memcpy(&value, &temp, sizeof(double));
            vector[j] = value;
            
            std::cout << "DEBUG: Vector[" << i << "][" << j << "] = " << value << std::endl;
        }
        
        vectors[i] = vector;
    }
    
    return true;
}

bool ClientHandler::sendResults(const std::vector<double>& results) {
    uint32_t numResults = htonl(static_cast<uint32_t>(results.size()));
    if (!sendBinary(numResults)) {
        logger.log(LogLevel::ERROR, "Failed to send number of results");
        return false;
    }
    
    for (double result : results) {
        // Convert double to network byte order
        uint64_t temp;
        memcpy(&temp, &result, sizeof(double));
        temp = htobe64(temp);
        
        if (!sendBinary(temp)) {
            logger.log(LogLevel::ERROR, "Failed to send result");
            return false;
        }
        
        std::cout << "DEBUG: Sending result: " << result << std::endl;
    }
    
    return true;
}

void ClientHandler::handleClient() {
    struct sockaddr_in clientAddr;
    socklen_t addrLen = sizeof(clientAddr);
    getpeername(clientSocket, (struct sockaddr*)&clientAddr, &addrLen);
    std::string clientIP = inet_ntoa(clientAddr.sin_addr);
    
    logger.log(LogLevel::INFO, "New client connection", "ip=" + clientIP);
    
    // Step 1-5: Authentication (text format)
    if (!authenticateClient()) {
        logger.log(LogLevel::INFO, "Client authentication failed, closing connection", "ip=" + clientIP);
        close(clientSocket);
        return;
    }
    
    // Step 6-10: Data processing (binary format)
    std::vector<std::vector<double>> vectors;
    if (!receiveVectors(vectors)) {
        logger.log(LogLevel::ERROR, "Failed to receive vectors from client", "ip=" + clientIP);
        close(clientSocket);
        return;
    }
    
    std::vector<double> results = DataProcessor::processVectors(vectors);
    
    if (!sendResults(results)) {
        logger.log(LogLevel::ERROR, "Failed to send results to client", "ip=" + clientIP);
        close(clientSocket);
        return;
    }
    
    logger.log(LogLevel::INFO, "Successfully processed vectors", 
               "ip=" + clientIP + " vectors=" + std::to_string(vectors.size()));
    
    close(clientSocket);
}