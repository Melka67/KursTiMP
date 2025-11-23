#include "Server.h"
#include <iostream>
#include <csignal>
#include <unistd.h>
#include "ClientHandler.h"

Server::Server(const Config& config) 
    : authManager(config.clientDbFile), logger(config.logFile), running(false) {
    
    if (!setupSocket(config.port)) {
        throw std::runtime_error("Failed to setup server socket");
    }
    
    logger.log(LogLevel::INFO, "Server initialized", 
               "port=" + std::to_string(config.port) + 
               " client_db=" + config.clientDbFile +
               " log_file=" + config.logFile);
}

Server::~Server() {
    cleanup();
}

bool Server::setupSocket(int port) {
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        logger.log(LogLevel::CRITICAL, "Failed to create socket");
        return false;
    }
    
    int opt = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        logger.log(LogLevel::ERROR, "Failed to set socket options");
        return false;
    }
    
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);
    
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        logger.log(LogLevel::CRITICAL, "Failed to bind socket");
        return false;
    }
    
    if (listen(serverSocket, 10) < 0) {
        logger.log(LogLevel::CRITICAL, "Failed to listen on socket");
        return false;
    }
    
    return true;
}

void Server::cleanup() {
    if (serverSocket >= 0) {
        close(serverSocket);
    }
}

bool Server::start() {
    running = true;
    logger.log(LogLevel::INFO, "Server started and listening for connections");
    
    while (running) {
        struct sockaddr_in clientAddr;
        socklen_t clientLen = sizeof(clientAddr);
        
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);
        if (clientSocket < 0) {
            if (running) {
                logger.log(LogLevel::ERROR, "Failed to accept client connection");
            }
            continue;
        }
        
        // Handle client in single-threaded mode (as per requirements)
        ClientHandler clientHandler(clientSocket, authManager, logger);
        clientHandler.handleClient();
    }
    
    return true;
}

void Server::stop() {
    running = false;
    logger.log(LogLevel::INFO, "Server stopping");
    // Close the socket to break out of accept()
    shutdown(serverSocket, SHUT_RDWR);
}