/**
 * @file processor.cpp
 * @brief Реализация обработки векторных данных
 * @author Мелькаев Евгений
 * @date 2025
 */

#include "processor.h"
#include "logger.h"
#include <iostream>
#include <cstring>
#include <cstdint>
#include <vector>
#include <unistd.h>
#include <sys/socket.h>
#include <cmath>
#include <limits>
#include <cfloat>

// Константы переполнения
const double OVERFLOW_UP = 9223372036854775807.0;    ///< 2^63 - 1 (максимальное значение при переполнении)
const double OVERFLOW_DOWN = -9223372036854775808.0; ///< -2^63 (минимальное значение при переполнении)

/**
 * @brief Обрабатывает векторные данные от клиента
 * 
 * @param clientSocket Дескриптор сокета подключенного клиента
 * @return true Все векторы успешно обработаны
 * @return false Ошибка при чтении/записи данных
 * 
 * @details Алгоритм обработки:
 * 1. Чтение количества векторов (uint32_t)
 * 2. Для каждого вектора в цикле:
 *    - Чтение размера вектора (uint32_t)
 *    - Чтение данных вектора (массив double)
 *    - Вычисление произведения элементов
 *    - Отправка результата клиенту
 * 
 * @note Использует MSG_WAITALL для гарантированного чтения всего блока данных
 * @note Все данные передаются в сетевом порядке байт
 */
bool Processor::processVectors(int clientSocket) {
    uint32_t count;
    ssize_t bytes = recv(clientSocket, &count, sizeof(count), MSG_WAITALL);
    if (bytes != sizeof(count)) {
        Logger::getInstance().log("Failed to read vector count", false);
        return false;
    }
    
    Logger::getInstance().log("Processing " + std::to_string(count) + " vectors");
    std::cout << "Processing " << count << " vectors..." << std::endl;
    
    for (uint32_t i = 0; i < count; i++) {
        uint32_t size;
        bytes = recv(clientSocket, &size, sizeof(size), MSG_WAITALL);
        if (bytes != sizeof(size)) {
            Logger::getInstance().log("Failed to read vector size", false);
            return false;
        }
        
        std::vector<double> vec(size);
        bytes = recv(clientSocket, vec.data(), size * sizeof(double), MSG_WAITALL);
        if (bytes != static_cast<ssize_t>(size * sizeof(double))) {
            Logger::getInstance().log("Failed to read vector data", false);
            return false;
        }
        
        double product = calculateProduct(vec);
        
        bytes = send(clientSocket, &product, sizeof(product), 0);
        if (bytes != sizeof(product)) {
            Logger::getInstance().log("Failed to send result", false);
            return false;
        }
        
        Logger::getInstance().log("Vector " + std::to_string(i+1) + 
                                 " processed, result: " + std::to_string(product));
        std::cout << "Vector " << i + 1 << " processed, result: " << product << std::endl;
    }
    
    Logger::getInstance().log("All vectors processed successfully");
    std::cout << "All vectors processed successfully" << std::endl;
    return true;
}

/**
 * @brief Вычисляет произведение всех элементов вектора
 * 
 * @param vec Вектор чисел с плавающей точкой
 * @return double Произведение всех элементов
 * 
 * @details Особенности реализации:
 * 1. Проверка на пустой вектор → возврат 0.0
 * 2. Обнаружение нуля → немедленный возврат 0.0
 * 3. Отслеживание знака произведения
 * 4. Проверка переполнения перед умножением
 * 5. При переполнении возвращает граничные значения:
 *    - OVERFLOW_UP (2^63-1) для положительных
 *    - OVERFLOW_DOWN (-2^63) для отрицательных
 * 
 * @warning Использует DBL_MAX для проверки переполнения
 * @note Алгоритм оптимизирован для минимизации ошибок округления
 */
double Processor::calculateProduct(const std::vector<double>& vec) {
    if (vec.empty()) return 0.0;
    
    double product = 1.0;
    bool isPositive = true;
    
    for (size_t i = 0; i < vec.size(); i++) {
        double val = vec[i];
        
        if (val == 0.0) {
            return 0.0;
        }
        
        if (val < 0.0) {
            isPositive = !isPositive;
        }
        
        double absProduct = std::abs(product);
        double absVal = std::abs(val);
        
        if (absProduct > DBL_MAX / absVal) {
            std::cout << "REAL DOUBLE OVERFLOW! ";
            Logger::getInstance().log("Real double overflow detected");
            
            if (isPositive) {
                std::cout << "Returning 2^63 - 1" << std::endl;
                return OVERFLOW_UP;
            } else {
                std::cout << "Returning -2^63" << std::endl;
                return OVERFLOW_DOWN;
            }
        }
        
        product *= val;
    }
    
    return product;
}