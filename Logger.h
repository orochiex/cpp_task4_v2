#pragma once
#include <fstream>
#include <string>
#include <ctime>
#include <iomanip>

class Logger {
public:
    Logger(const std::string& filename);
    ~Logger();
    void log(const std::string& message, const std::string& importance);

private:
    std::ofstream logfile;
};
