/**
 * @file logger.cpp
 * @brief Реализация класса логгера
 * @author Мелькаев Евгений
 * @date 2025
 */

#include "logger.h"
#include <iostream>
#include <ctime>
#include <cstring>

/**
 * @brief Возвращает единственный экземпляр логгера
 * 
 * @return Logger& Ссылка на статический экземпляр
 * 
 * @note Использует идиому "Meyer's singleton"
 * Потокобезопасна в стандарте C++11 и выше
 */
Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

/**
 * @brief Инициализирует логгер с указанным файлом
 * 
 * @param filename Путь к файлу для записи логов
 * @return true Файл успешно открыт
 * @return false Не удалось открыть файл
 * 
 * @details Открывает файл в режиме std::ios::app (добавление в конец).
 * Если файл не открывается, логи будут выводиться в std::cout.
 */
bool Logger::init(const std::string& filename) {
    logFile.open(filename, std::ios::app);
    if (!logFile.is_open()) {
        std::cerr << "Cannot open log file: " << filename << std::endl;
        return false;
    }
    return true;
}

/**
 * @brief Записывает сообщение в лог
 * 
 * @param message Текст сообщения для записи
 * @param isCritical Флаг, указывающий на критичность сообщения
 * 
 * @details Формат записи:
 * [YYYY-MM-DD HH:MM:SS | LEVEL | message]
 * 
 * Уровни:
 * - INFO (isCritical = false)
 * - CRITICAL (isCritical = true)
 * 
 * Если файл не открыт, выводит в консоль с префиксом [LOG]
 */
void Logger::log(const std::string& message, bool isCritical) {
    if (!logFile.is_open()) {
        // Если файл не открыт, выводим в консоль
        std::cout << "[LOG] " << message << std::endl;
        return;
    }
    
    std::time_t now = std::time(nullptr);
    char timeStr[100];
    std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    
    logFile << timeStr << " | ";
    logFile << (isCritical ? "CRITICAL" : "INFO") << " | ";
    logFile << message << std::endl;
    logFile.flush();
}

/**
 * @brief Закрывает файл логов
 * 
 * Вызывается при завершении работы программы или
 * при повторной инициализации логгера.
 */
void Logger::close() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

/**
 * @brief Деструктор логгера
 * 
 * Автоматически закрывает файл логов при уничтожении объекта.
 */
Logger::~Logger() {
    close();
}