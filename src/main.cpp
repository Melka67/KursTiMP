/**
 * @file main.cpp
 * @brief Главный файл сервера аутентификации и обработки векторов
 * @mainpage Сервер аутентификации и векторных вычислений
 * 
 * Сервер принимает подключения клиентов, проводит аутентификацию
 * и выполняет вычисления над векторами.
 * 
 * @section usage_sec Использование
 * @code{.sh}
 * ./server [параметры]
 * ./server -p 33333 -c vcalc.conf -l vcalc.log
 * @endcode
 */

#include "server.h"
#include "logger.h"
#include "args_parser.h"
#include <iostream>
#include <csignal>

Server server; ///< Глобальный экземпляр сервера

/**
 * @brief Обработчик сигналов завершения работы
 * @param signum Номер сигнала
 * 
 * Обрабатывает сигналы SIGINT (Ctrl+C) для корректного завершения сервера.
 * Записывает сообщение в лог и выводит информацию в консоль.
 */
void signalHandler(int signum) {
    Logger::getInstance().log("Server stopped by signal", true);
    std::cout << "\nInterrupt signal received. Stopping server..." << std::endl;
    exit(signum);
}

/**
 * @brief Точка входа в программу
 * @param argc Количество аргументов командной строки
 * @param argv Массив аргументов командной строки
 * @return Код завершения программы (0 - успешно, 1 - ошибка)
 * 
 * Выполняет:
 * 1. Парсинг аргументов командной строки
 * 2. Инициализацию логгера
 * 3. Установку обработчика сигналов
 * 4. Запуск сервера
 * 
 * @exception std::exception При ошибках парсинга аргументов
 */
int main(int argc, char* argv[]) {
    try {
        ServerConfig config = ArgsParser::parse(argc, argv);
        
        if (config.showHelp) {
            ArgsParser::printHelp();
            return 0;
        }
        
        // Инициализируем логгер
        if (!Logger::getInstance().init(config.logFile)) {
            std::cerr << "Cannot open log file: " << config.logFile << std::endl;
            return 1;
        }
        
        signal(SIGINT, signalHandler);
        
        std::cout << "Starting server with parameters:\n"
                  << "  Port: " << config.port << "\n"
                  << "  Config file: " << config.configFile 
                  << "\n  Log file: " << config.logFile << std::endl;
        
        Logger::getInstance().log("Server starting on port " + std::to_string(config.port));
        
        if (!server.start(config.port, config.configFile)) {
            Logger::getInstance().log("Failed to start server", true);
            std::cerr << "Failed to start server" << std::endl;
            return 1;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        ArgsParser::printHelp();
        return 1;
    }
    
    return 0;
}