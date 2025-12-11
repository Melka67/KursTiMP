/**
 * @file server.cpp
 * @brief Реализация класса сервера
 * @author Мелькаев Евгений
 * @date 2025
 */

#include "server.h"
#include "auth.h"
#include "database.h"
#include "processor.h"
#include "logger.h"
#include <iostream>
#include <cstring>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

/**
 * @brief Запускает сервер и начинает прослушивание порта
 * 
 * @param port Порт для прослушивания
 * @param configFile Путь к файлу с базой пользователей
 * @return true Сервер успешно запущен
 * @return false Ошибка при запуске сервера
 * 
 * @details Метод выполняет полную инициализацию сервера:
 * 1. Загружает базу данных пользователей
 * 2. Создает сокет сервера
 * 3. Настраивает и привязывает сокет
 * 4. Начинает прослушивание порта
 * 5. Входит в бесконечный цикл обработки клиентов
 * 
 * @note Использует TCP сокеты с адресом INADDR_ANY (все интерфейсы)
 * @note Включает опцию SO_REUSEADDR для быстрого перезапуска
 * 
 * @see Database::load
 * @see handleClient
 */
bool Server::start(int port, const std::string& configFile) {
    // Загружаем базу
    if (!Database::load(configFile)) {
        Logger::getInstance().log("Failed to load database: " + configFile, true);
        std::cerr << "Failed to load database" << std::endl;
        return false;
    }
    
    Logger::getInstance().log("Database loaded successfully: " + configFile);
    
    // Создаем сокет
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        Logger::getInstance().log("Socket error: " + std::string(strerror(errno)), true);
        std::cerr << "Socket error: " << strerror(errno) << std::endl;
        return false;
    }
    
    // Настройки сокета
    int opt = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        Logger::getInstance().log("Setsockopt error: " + std::string(strerror(errno)), false);
    }
    
    // Биндим
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
    
    if (bind(serverSocket, (sockaddr*)&addr, sizeof(addr)) < 0) {
        Logger::getInstance().log("Bind error on port " + std::to_string(port) + 
                                 ": " + strerror(errno), true);
        std::cerr << "Bind error: " << strerror(errno) << std::endl;
        close(serverSocket);
        return false;
    }
    
    // Слушаем
    if (listen(serverSocket, 10) < 0) {
        Logger::getInstance().log("Listen error: " + std::string(strerror(errno)), true);
        std::cerr << "Listen error: " << strerror(errno) << std::endl;
        close(serverSocket);
        return false;
    }
    
    std::cout << "Server started on port " << port << std::endl;
    std::cout << "Press Ctrl+C to stop" << std::endl;
    Logger::getInstance().log("Server started successfully on port " + std::to_string(port));
    
    // Главный цикл
    while (true) {
        sockaddr_in clientAddr;
        socklen_t clientLen = sizeof(clientAddr);
        
        int clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientLen);
        if (clientSocket < 0) {
            Logger::getInstance().log("Accept error: " + std::string(strerror(errno)), false);
            continue;
        }
        
        char clientIP[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);
        
        Logger::getInstance().log("New connection from " + std::string(clientIP));
        std::cout << "New connection from " << clientIP << std::endl;
        
        handleClient(clientSocket);
        close(clientSocket);
        
        Logger::getInstance().log("Connection closed: " + std::string(clientIP));
    }
    
    close(serverSocket);
    return true;
}

/**
 * @brief Обрабатывает отдельное клиентское подключение
 * 
 * @param clientSocket Дескриптор сокета клиента
 * 
 * @details Последовательность обработки:
 * 1. Аутентификация клиента через Auth::authenticate()
 * 2. Если аутентификация успешна - обработка векторных данных
 * 3. Закрытие соединения после завершения обработки
 * 
 * @see Auth::authenticate
 * @see Processor::processVectors
 */
void Server::handleClient(int clientSocket) {
    // Аутентификация
    if (!Auth::authenticate(clientSocket)) {
        Logger::getInstance().log("Authentication failed");
        return;
    }
    
    Logger::getInstance().log("Authentication successful");
    
    // Обработка векторов
    if (!Processor::processVectors(clientSocket)) {
        Logger::getInstance().log("Vector processing failed", false);
    } else {
        Logger::getInstance().log("Vector processing completed successfully");
    }
}