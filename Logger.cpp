#include "Logger.hpp"

#include <errno.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <algorithm>

std::string s_filename = "results.log";

std::string LevelToString(Logger::Level level)
{
    switch (level)
    {
    case Logger::Level::Debug:  { return "[DEBUG]"; } break;
    case Logger::Level::Info:  { return "[INFO]"; } break;
    case Logger::Level::Warn: { return "[WARN]"; } break;
    case Logger::Level::Error: { return "[ERROR]"; } break;
    case Logger::Level::Assertion: { return "[ASSERT]"; } break;
    default: { throw std::invalid_argument("Bad Log::Level Enum"); } break;
    }
}

void Logger::Init() {

    #ifdef COSMIC_DEBUG_BUILD
        auto now = std::chrono::system_clock::now();
        time_t time = std::chrono::system_clock::to_time_t(now);
        struct tm* timeinfo = localtime(&time);

        std::ostringstream filename_builder;
        filename_builder << "results_" << std::put_time(timeinfo, "%Y-%m-%d_%X") << ".log";
        s_filename = filename_builder.str();
        std::replace( s_filename.begin(), s_filename.end(), ':', ';');
    #endif

    FILE* log_file;
    //Create an empty file for output operations. If a file with the same name already exists, its contents are discarded and the file is treated as a new empty file.
    log_file = fopen(s_filename.c_str(), "w");
    if(log_file == NULL)
    {
        std::string error_msg = "[ERROR] Could not open log file to write to.\n";
        fprintf(stdout, "%s", error_msg.c_str());
        fflush(stdout);  
    } else {
        fclose(log_file);
    }
}

Logger::Logger(Level type,std::string file, int line)
{
    if(type == Level::Assertion) {
        m_terminate = true;
    }
    m_level = type;

    auto now = std::chrono::system_clock::now();
    time_t time = std::chrono::system_clock::to_time_t(now);
    struct tm* timeinfo = localtime(&time);

    m_message << LevelToString(type) << "   " <<  std::put_time(timeinfo, "%Y-%m-%d %X") << "   " << file << ":" << line <<"   ";
}

Logger::~Logger()
{
    m_message << std::endl;
    if(m_terminate) {
        m_message << "Assertion Failed. Terminating Program" << std::endl;
    }

    // Log to console
    const char* color_strings[5] = {"1;34", "1;32", "1;33", "1;31", "1;41"};
    std::cout << "\033[" << color_strings[(uint8_t) m_level] << "m" << m_message.str() << "\033[0m";


    fflush(stdout);

    // Log to file
    FILE *log_file;
    log_file = fopen(s_filename.c_str(), "a");
    if(log_file == NULL)
    {
        std::string error_msg = "[ERROR] Could not open log file to write to.\n";
        fprintf(stdout, "%s", error_msg.c_str());
        fflush(stdout);  
    } else {
        fprintf(log_file, "%s", m_message.str().c_str());
        fclose(log_file);
    }

    if(m_terminate) {
        //__debugbreak();
        std::abort();
    }
}

void Logger::operator<< (const char* str)                        { m_message << str; }
void Logger::operator<< (std::string str)                        { m_message << str; }
void Logger::operator<< (bool val)                               { m_message << val; }
void Logger::operator<< (short val)                              { m_message << val; }
void Logger::operator<< (unsigned short val)                     { m_message << val; }
void Logger::operator<< (int val)                                { m_message << val; }
void Logger::operator<< (unsigned int val)                       { m_message << val; }
void Logger::operator<< (long val)                               { m_message << val; }
void Logger::operator<< (unsigned long val)                      { m_message << val; }
void Logger::operator<< (long long val)                          { m_message << val; }
void Logger::operator<< (unsigned long long val)                 { m_message << val; }
void Logger::operator<< (float val)                              { m_message << val; }
void Logger::operator<< (double val)                             { m_message << val; }
void Logger::operator<< (long double val)                        { m_message << val; }
void Logger::operator<< (void* val)                              { m_message << val; }
void Logger::operator<< (std::streambuf* sb )                    { m_message << sb; }
void Logger::operator<< (std::ostream& (*pf)(std::ostream&))     { m_message << pf; }
void Logger::operator<< (std::ios& (*pf)(std::ios&))             { m_message << pf; }
void Logger::operator<< (std::ios_base& (*pf)(std::ios_base&))   { m_message << pf; }
