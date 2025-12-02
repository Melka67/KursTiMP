#ifndef SHA224_H
#define SHA224_H

#include <string>
#include <vector>

class SHA224 {
public:
    // Вычисление SHA-224 от данных
    static std::string hash(const std::string& data);
    
    // Вычисление SHA-224(salt || password)
    static std::string hashWithSalt(const std::string& salt, const std::string& password);
    
    // Конвертация бинарных данных в hex строку
    static std::string toHex(const unsigned char* data, size_t length);
    
    // Проверка hex строки
    static bool isValidHex(const std::string& str);
};

#endif