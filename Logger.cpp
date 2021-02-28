#include "Logger.hpp"

#include <errno.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <iomanip>


std::string GetDateAndTime()
{
    auto now = std::chrono::system_clock::now();
    time_t time = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    struct tm* timeinfo;

    timeinfo = localtime(&time);
    ss << std::put_time(timeinfo, "%Y-%m-%d %X");
    return ss.str();
}

std::string HexDump(std::vector<uint8_t>& data) {
    std::stringstream ss;
    ss << std::endl << std::hex;
    for(size_t i = 0; i < ((data.size() / 10) + 1); i++) {
        ss << "\t| ";
        for(size_t j = 0; (j < 10) && ((j + (10 * i)) < data.size()); j++) {
               ss << unsigned(data.at(j + (10 * i))) << " ";
        }
        ss << "|" << std::endl;
    }
    ss << std::dec;
    return ss.str();
}

std::string DecDump(std::vector<uint8_t>& data) {
    std::stringstream ss;
    ss << std::endl;
    for(size_t i = 0; i < ((data.size() / 10) + 1); i++) {
        ss << "\t| ";
        for(size_t j = 0; (j < 10) && ((j + (10 * i)) < data.size()); j++) {
            ss << unsigned(data.at(j + (10 * i))) << " ";
        }
        ss << "|" << std::endl;
    }
    return ss.str();
}

Logger::~Logger()
{
    message << std::endl;
    if(terminate) {
        message << "Assertion Failed. Terminating Program" << std::endl;
    }

    // Log to console
    fprintf(stdout, "%s", message.str().c_str());
    fflush(stdout);

    // Log to file
    FILE *log_file;
    log_file = fopen(LOF_FILE_NAME, "a");
    if(log_file == NULL)
    {
        std::string error_msg = "[ERROR] Could not open log file to write to.\n";
        fprintf(stdout, "%s", error_msg.c_str());
        fflush(stdout);  
    } else {
        fprintf(log_file, "%s", message.str().c_str());
        fclose(log_file);
    }

    if(terminate) {
        std::abort();
    }
}

std::ostringstream& Logger::SetMessage(LogLevel type,std::string file, int line) {
    if(type == LogLevel::LOG_ASSERT) {
        terminate = true;
    }
    message << LevelToString(type) << "   " << GetDateAndTime() << "   " << file << ":" << line <<"   ";
    return message;
}

std::string Logger::LevelToString(LogLevel level)
{
    switch (level)
    {
    case LOG_DEBUG:  { return "[DEBUG]"; } break;
    case LOG_INFO:  { return "[INFO]"; } break;
    case LOG_WARN: { return "[WARN]"; } break;
    case LOG_ERROR: { return "[ERROR]"; } break;
    case LOG_ASSERT: { return "[ASSERT]"; } break;
    default: { throw std::invalid_argument("Bad Log LogLevel Enum"); } break;
    }
}