/**
 * @file logger.h
 * @brief Заголовочный файл класса логгера
 * @author Мелькаев Евгений
 * @date 2025
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>

/**
 * @brief Класс логгера (синглтон) для записи событий сервера
 * 
 * Обеспечивает потокобезопасное логирование в файл с метками времени.
 * Реализован как синглтон - только один экземпляр на программу.
 * 
 * @note Для использования вызовите Logger::getInstance()
 */
class Logger {
public:
    /**
     * @brief Возвращает единственный экземпляр логгера
     * @return Logger& Ссылка на экземпляр логгера
     * 
     * @details Реализовано через статическую локальную переменную
     * (Meyer's singleton), что обеспечивает потокобезопасность в C++11+
     */
    static Logger& getInstance();
    
    /**
     * @brief Инициализирует логгер с указанным файлом
     * @param filename Путь к файлу логов
     * @return true Файл успешно открыт
     * @return false Ошибка открытия файла
     * 
     * @note Открывает файл в режиме добавления (append)
     * @note Если файл не открывается, логи выводятся в консоль
     */
    bool init(const std::string& filename);
    
    /**
     * @brief Записывает сообщение в лог
     * @param message Текст сообщения
     * @param isCritical Флаг критичности сообщения
     * 
     * @details Формат записи: "ВРЕМЯ | УРОВЕНЬ | СООБЩЕНИЕ"
     * Уровень: INFO или CRITICAL в зависимости от isCritical
     */
    void log(const std::string& message, bool isCritical = false);
    
    /**
     * @brief Закрывает файл логов
     * 
     * Вызывается автоматически в деструкторе
     */
    void close();
    
private:
    Logger() = default; ///< Приватный конструктор
    ~Logger();          ///< Деструктор
    Logger(const Logger&) = delete; ///< Запрет копирования
    Logger& operator=(const Logger&) = delete; ///< Запрет присваивания
    
    std::ofstream logFile; ///< Поток для записи в файл
};

#endif