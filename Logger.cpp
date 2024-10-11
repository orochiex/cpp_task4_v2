#include "Logger.h"

Logger::Logger(const std::string& filename) {
    logfile.open(filename, std::ios_base::app);
    if (!logfile.is_open()) {
        throw std::runtime_error("Unable to open log file");
    }
}

Logger::~Logger() {
    if (logfile.is_open()) {
        logfile.close();
    }
}

void Logger::log(const std::string& message, const std::string& importance) {
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    logfile << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << " [" << importance << "] " << message << std::endl;
}
