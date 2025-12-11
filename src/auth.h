/**
 * @file auth.h
 * @brief Заголовочный файл класса аутентификации
 * @author Мелькаев Евгений
 * @date 2025
 */

#ifndef AUTH_H
#define AUTH_H

#include <string>

/**
 * @brief Класс для аутентификации клиентов
 * 
 * Обеспечивает проверку учетных данных клиентов,
 * используя механизм соли и хэширования SHA-224.
 * 
 * @note Все методы статические - не требует создания экземпляра
 */
class Auth {
public:
    /**
     * @brief Выполняет аутентификацию клиента
     * @param clientSocket Дескриптор сокета клиента
     * @return true Аутентификация успешна
     * @return false Аутентификация не удалась
     * 
     * @details Протокол аутентификации:
     * 1. Клиент отправляет: LOGIN(4) + SALT(16 hex) + HASH(56 hex)
     * 2. Сервер проверяет формат
     * 3. Сервер вычисляет и сравнивает хэш
     * 4. Сервер отправляет OK или ERR
     */
    static bool authenticate(int clientSocket);
    
// Делаем методы публичными для тестов
#ifdef UNIT_TESTS
public:
#else
private:
#endif
    /**
     * @brief Проверяет формат аутентификационных данных
     * @param login Логин пользователя
     * @param salt Соль для хэширования
     * @param hash Хэш пароля
     * @return true Формат корректен
     * @return false Формат некорректен
     * 
     * Проверяет:
     * - Логин должен быть "user"
     * - Длина соли: 16 hex символов
     * - Длина хэша: 56 hex символов
     * - Содержит только hex символы
     */
    static bool validateFormat(const std::string& login, 
                               const std::string& salt, 
                               const std::string& hash);
    
    /**
     * @brief Проверяет учетные данные
     * @param login Логин пользователя
     * @param salt Соль для хэширования
     * @param receivedHash Хэш от клиента
     * @return true Учетные данные верны
     * @return false Неверные учетные данные
     * 
     * Сравнивает вычисленный хэш SHA-224(salt + password) с полученным
     */
    static bool verifyCredentials(const std::string& login,
                                 const std::string& salt,
                                 const std::string& receivedHash);
};

#endif