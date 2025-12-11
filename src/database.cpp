/**
 * @file database.cpp
 * @brief Реализация класса базы данных пользователей
 * @author Мелькаев Евгений
 * @date 2025
 */

#include "database.h"
#include "sha224.h"
#include <iostream>
#include <fstream>
#include <sstream>

std::unordered_map<std::string, std::string> Database::users; ///< Статическое хранилище

/**
 * @brief Загружает пользователей из текстового файла
 * 
 * @param filename Путь к файлу конфигурации
 * @return true Файл успешно загружен
 * @return false Ошибка открытия или чтения файла
 * 
 * @details Пример содержимого файла:
 * @code
 * user1:password1
 * user2:password2
 * # комментарий
 * user3:password3
 * @endcode
 * 
 * @note Тримит пробелы и табуляции вокруг логина и пароля
 * @note Очищает предыдущие данные перед загрузкой
 */
bool Database::load(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Cannot open: " << filename << std::endl;
        return false;
    }
    
    users.clear();
    std::string line;
    int count = 0;
    
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        
        size_t pos = line.find(':');
        if (pos == std::string::npos) continue;
        
        std::string login = line.substr(0, pos);
        std::string password = line.substr(pos + 1);
        
        // Trim
        login.erase(0, login.find_first_not_of(" \t"));
        login.erase(login.find_last_not_of(" \t") + 1);
        password.erase(0, password.find_first_not_of(" \t"));
        password.erase(password.find_last_not_of(" \t") + 1);
        
        users[login] = password;
        count++;
    }
    
    return true;
}

/**
 * @brief Получает пароль пользователя
 * 
 * @param login Логин пользователя для поиска
 * @return std::string Пароль или пустая строка если пользователь не найден
 * 
 * @warning Возвращаемая строка может быть пустой если пользователь не существует
 */
std::string Database::getPassword(const std::string& login) {
    auto it = users.find(login);
    if (it == users.end()) {
        return "";
    }
    return it->second;
}

/**
 * @brief Проверяет учетные данные пользователя
 * 
 * @param login Логин пользователя
 * @param salt Соль для хэширования (16 hex символов)
 * @param hash Хэш для проверки (56 hex символов)
 * @return true Хэш совпадает с ожидаемым
 * @return false Логин не найден или хэш не совпадает
 * 
 * @details Алгоритм проверки:
 * 1. Получение пароля из базы
 * 2. Вычисление SHA-224(salt + password)
 * 3. Сравнение с предоставленным хэшем (регистронезависимо)
 * 
 * @see SHA224::hashWithSalt
 */
bool Database::checkUser(const std::string& login, 
                        const std::string& salt, 
                        const std::string& hash) {
    std::string password = getPassword(login);
    if (password.empty()) return false;
    
    std::string expectedHash = SHA224::hashWithSalt(salt, password);
    
    // Сравниваем без учета регистра
    std::string receivedUpper = hash;
    std::string expectedUpper = expectedHash;
    
    for (char& c : receivedUpper) c = std::toupper(c);
    for (char& c : expectedUpper) c = std::toupper(c);
    
    return receivedUpper == expectedUpper;
}