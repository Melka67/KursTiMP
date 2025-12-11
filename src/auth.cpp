/**
 * @file auth.cpp
 * @brief Реализация класса аутентификации
 * @author Мелькаев Евгений
 * @date 2025
 */

#include "auth.h"
#include "database.h"
#include "sha224.h"
#include "logger.h"
#include <iostream>
#include <cstring>
#include <string>
#include <unistd.h>
#include <sys/socket.h>  

/**
 * @brief Выполняет полный процесс аутентификации клиента
 * 
 * @param clientSocket Дескриптор сокета подключенного клиента
 * @return true Клиент успешно аутентифицирован
 * @return false Ошибка аутентификации
 * 
 * @details Процесс аутентификации:
 * 1. Получение данных от клиента (76 байт)
 * 2. Разбор на компоненты: логин, соль, хэш
 * 3. Валидация формата
 * 4. Проверка учетных данных
 * 5. Отправка результата клиенту
 * 
 * @note Формат сообщения: LOGIN(4) + SALT(16 hex) + HASH(56 hex)
 * @note При ошибке отправляет "ERR", при успехе - "OK"
 */
bool Auth::authenticate(int clientSocket) {
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    
    ssize_t len = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (len <= 0) {
        Logger::getInstance().log("Failed to receive authentication data", false);
        return false;
    }
    
    std::string msg(buffer, len);
    
    // Формат: LOGIN (4 символа) + SALT (16 hex) + HASH (56 hex)
    if (msg.length() != 76) {  // 4 + 16 + 56 = 76
        Logger::getInstance().log("Invalid auth message length: " + std::to_string(msg.length()), false);
        send(clientSocket, "ERR", 3, 0);
        return false;
    }
    
    std::string login = msg.substr(0, 4);
    std::string salt = msg.substr(4, 16);
    std::string receivedHash = msg.substr(20, 56);
    
    if (!validateFormat(login, salt, receivedHash)) {
        send(clientSocket, "ERR", 3, 0);
        return false;
    }
    
    if (!verifyCredentials(login, salt, receivedHash)) {
        Logger::getInstance().log("Authentication failed for: " + login);
        send(clientSocket, "ERR", 3, 0);
        return false;
    }
    
    send(clientSocket, "OK", 2, 0);
    Logger::getInstance().log("User authenticated: " + login);
    return true;
}

/**
 * @brief Проверяет корректность формата аутентификационных данных
 * 
 * @param login Логин пользователя (должен быть "user")
 * @param salt Соль в hex формате
 * @param hash Хэш в hex формате
 * @return true Все параметры корректны
 * @return false Обнаружена ошибка в формате
 * 
 * @see SHA224::isValidHex
 */
bool Auth::validateFormat(const std::string& login, 
                         const std::string& salt, 
                         const std::string& hash) {
    // Проверяем логин
    if (login != "user") {
        Logger::getInstance().log("Invalid login: " + login, false);
        return false;
    }
    
    // Проверяем длину соли
    if (salt.length() != 16) {
        Logger::getInstance().log("Invalid salt length: " + std::to_string(salt.length()), false);
        return false;
    }
    
    // Проверяем hex символы соли
    if (!SHA224::isValidHex(salt)) {
        Logger::getInstance().log("Invalid salt format (not hex)", false);
        return false;
    }
    
    // Проверяем длину хэша
    if (hash.length() != 56) {  // SHA-224 produces 56 hex characters
        Logger::getInstance().log("Invalid hash length: " + std::to_string(hash.length()), false);
        return false;
    }
    
    // Проверяем hex символы хэша
    if (!SHA224::isValidHex(hash)) {
        Logger::getInstance().log("Invalid hash format (not hex)", false);
        return false;
    }
    
    return true;
}

/**
 * @brief Проверяет соответствие учетных данных
 * 
 * @param login Логин пользователя
 * @param salt Соль использованная клиентом
 * @param receivedHash Хэш от клиента
 * @return true Хэши совпадают
 * @return false Хэши не совпадают или пользователь не найден
 * 
 * @details Получает пароль из базы, вычисляет SHA-224(salt + password)
 * и сравнивает с полученным хэшем (регистронезависимо).
 */
bool Auth::verifyCredentials(const std::string& login,
                            const std::string& salt,
                            const std::string& receivedHash) {
    // Получаем пароль из базы данных
    std::string password = Database::getPassword(login);
    if (password.empty()) {
        Logger::getInstance().log("User not found in database: " + login, false);
        return false;
    }
    
    // Вычисляем ожидаемый хэш: SHA-224(salt || password)
    std::string expectedHash = SHA224::hashWithSalt(salt, password);
    
    // Сравниваем (регистронезависимо для hex)
    std::string receivedUpper = receivedHash;
    std::string expectedUpper = expectedHash;
    
    // Конвертируем в верхний регистр для сравнения
    for (char& c : receivedUpper) c = std::toupper(c);
    for (char& c : expectedUpper) c = std::toupper(c);
    
    bool match = (receivedUpper == expectedUpper);
    
    if (!match) {
        std::cout << "Hash mismatch!\n"
                  << "Expected: " << expectedHash << "\n"
                  << "Received: " << receivedHash << std::endl;
    }
    
    return match;
}