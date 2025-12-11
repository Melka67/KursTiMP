/**
 * @file database.h
 * @brief Заголовочный файл класса базы данных пользователей
 * @author Мелькаев Евгений
 * @date 2025
 */

#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <unordered_map>

/**
 * @brief Класс для работы с базой данных пользователей
 * 
 * Хранит логины и пароли пользователей в памяти.
 * Обеспечивает загрузку из файла и проверку учетных данных.
 * 
 * @note Все методы класса статические - используется как синглтон
 */
class Database {
public:
    /**
     * @brief Загружает базу пользователей из файла
     * @param filename Путь к файлу конфигурации
     * @return true Файл успешно загружен
     * @return false Ошибка загрузки файла
     * 
     * @details Формат файла:
     * - Каждая строка: логин:пароль
     * - Пробелы вокруг разделителя игнорируются
     * - Строки, начинающиеся с #, игнорируются
     * - Пустые строки игнорируются
     */
    static bool load(const std::string& filename);
    
    /**
     * @brief Получает пароль пользователя по логину
     * @param login Логин пользователя
     * @return std::string Пароль пользователя или пустая строка если не найден
     */
    static std::string getPassword(const std::string& login);
    
    /**
     * @brief Проверяет учетные данные пользователя
     * @param login Логин пользователя
     * @param salt Соль для хэширования
     * @param hash Предоставленный хэш пароля
     * @return true Учетные данные верны
     * @return false Неверный логин или пароль
     * 
     * @details Сравнивает SHA-224(salt + password) с предоставленным хэшем
     * Сравнение регистронезависимое для hex-строк
     */
    static bool checkUser(const std::string& login, 
                         const std::string& salt, 
                         const std::string& hash);
    
private:
    static std::unordered_map<std::string, std::string> users; ///< Хранилище пользователей
};

#endif