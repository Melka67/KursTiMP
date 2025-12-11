/**
 * @file sha224.h
 * @brief Заголовочный файл для работы с SHA-224 хэшированием
 * @author Мелькаев Евгений
 * @date 2025
 */

#ifndef SHA224_H
#define SHA224_H

#include <string>
#include <vector>

/**
 * @brief Класс для вычисления SHA-224 хэшей
 * 
 * Предоставляет статические методы для:
 * - Вычисления хэшей от произвольных данных
 * - Вычисления хэшей с солью
 * - Преобразования в hex строки
 * - Валидации hex строк
 * 
 * @note Использует OpenSSL EVP API (рекомендовано для OpenSSL 3.0+)
 */
class SHA224 {
public:
    /**
     * @brief Вычисляет SHA-224 хэш от данных
     * @param data Входные данные для хэширования
     * @return std::string Hex строка хэша (56 символов)
     * 
     * @warning Возвращает пустую строку при ошибке инициализации контекста
     */
    static std::string hash(const std::string& data);
    
    /**
     * @brief Вычисляет SHA-224 от конкатенации соли и пароля
     * @param salt Соль для хэширования
     * @param password Пароль пользователя
     * @return std::string Hex строка хэша (56 символов)
     * 
     * @details Вычисляет SHA-224(salt || password)
     * Используется для безопасного хранения и проверки паролей
     */
    static std::string hashWithSalt(const std::string& salt, const std::string& password);
    
    /**
     * @brief Конвертирует бинарные данные в hex строку
     * @param data Указатель на бинарные данные
     * @param length Длина данных в байтах
     * @return std::string Hex представление данных
     * 
     * @note Каждый байт представляется двумя hex символами
     */
    static std::string toHex(const unsigned char* data, size_t length);
    
    /**
     * @brief Проверяет, является ли строка корректной hex строкой
     * @param str Строка для проверки
     * @return true Строка содержит только hex символы (0-9, a-f, A-F)
     * @return false Строка содержит не-hex символы
     */
    static bool isValidHex(const std::string& str);
};

#endif