/**
 * @file sha224.cpp
 * @brief Реализация SHA-224 хэширования
 * @author Мелькаев Евгений
 * @date 2025
 */

#include "sha224.h"
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>
#include <cctype>

/**
 * @brief Вычисляет SHA-224 хэш от данных
 * 
 * @param data Входные данные для хэширования
 * @return std::string Hex строка хэша длиной 56 символов
 * @return Пустая строка в случае ошибки создания контекста
 * 
 * @details Использует EVP API из OpenSSL, что является
 * рекомендуемым способом в OpenSSL 3.0 и выше.
 * Длина выходного хэша - 28 байт (224 бита), что соответствует
 * 56 hex символам.
 * 
 * @note Требуется линковка с libcrypto (-lcrypto)
 */
std::string SHA224::hash(const std::string& data) {
    unsigned char digest[SHA224_DIGEST_LENGTH];
    
    // Используем EVP API (рекомендуется в OpenSSL 3.0)
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    if (!ctx) return "";
    
    EVP_DigestInit_ex(ctx, EVP_sha224(), NULL);
    EVP_DigestUpdate(ctx, data.c_str(), data.length());
    EVP_DigestFinal_ex(ctx, digest, NULL);
    EVP_MD_CTX_free(ctx);
    
    return toHex(digest, SHA224_DIGEST_LENGTH);
}

/**
 * @brief Вычисляет SHA-224 от конкатенации соли и пароля
 * 
 * @param salt Соль для добавления к паролю
 * @param password Пароль пользователя
 * @return std::string Hex строка хэша
 * 
 * @details Конкатенирует соль и пароль, затем вычисляет хэш.
 * Это защищает от атак с использованием радужных таблиц.
 */
std::string SHA224::hashWithSalt(const std::string& salt, const std::string& password) {
    std::string data = salt + password;
    return hash(data);
}

/**
 * @param data Указатель на бинарные данные
 * @param length Длина данных в байтах
 * @return std::string Hex представление (длина = length * 2)
 * 
 * @example toHex({0xAB, 0xCD}, 2) вернет "abcd"
 * 
 * @note Использует std::hex и std::setfill('0') для гарантии
 * двух символов на каждый байт.
 */
std::string SHA224::toHex(const unsigned char* data, size_t length) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    
    for (size_t i = 0; i < length; i++) {
        ss << std::setw(2) << static_cast<unsigned>(data[i]);
    }
    
    return ss.str();
}

/**
 * @brief Проверяет корректность hex строки
 * 
 * @param str Строка для проверки
 * @return true Строка содержит только hex символы (0-9, a-f, A-F)
 * @return false Строка пуста или содержит не-hex символы
 * 
 * @details Использует std::isxdigit для проверки каждого символа.
 * Пустая строка считается некорректной.
 */
bool SHA224::isValidHex(const std::string& str) {
    if (str.empty()) return false;
    
    for (char c : str) {
        if (!std::isxdigit(static_cast<unsigned char>(c))) {
            return false;
        }
    }
    return true;
}