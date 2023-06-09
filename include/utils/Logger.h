#pragma once
#include <sstream>
#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <vector>
#include <memory>

namespace Utils{
    // 日志等级
    enum LogLevel{
        DEBUG = 0,
        INFO,
        WARN,
        ERR
    };

    class LogSink
    {
    protected:
        LogLevel m_minLevel;
    public:
        LogSink(const LogLevel minLevel):m_minLevel(minLevel){}
        virtual ~LogSink(){}
        virtual void write(const LogLevel level,const std::string &str)=0;
        void setMinLevel(LogLevel level){
            m_minLevel = level;
        }
    };
    // 输出
    class ConsoleSink:public LogSink{
    public:
        ConsoleSink(const LogLevel minLevel):LogSink(minLevel){}

        virtual void write(const LogLevel level,const std::string& str){
            if (level < m_minLevel)
                return;
            if (level == LogLevel::WARN)
                std::cout << "Warning: ";
            else if(level == LogLevel::ERR)
                std::cerr << "Error: ";
            
            std::cout << str << std::endl;
        }
    };
    // 文件
    class FileSink:public LogSink{
    protected:
        std::ofstream m_file;
    public:
        FileSink(const LogLevel minLevel,const std::string& fileName)
            : LogSink(minLevel)
        {
            m_file.open(fileName.c_str(),std::ios::out);
            if(m_file.fail())
                std::cerr << "Failed to open file: " << fileName << "\n";
        }

        virtual ~FileSink(){
            m_file.close();
        }

        virtual void write(const LogLevel level,const std::string& str )
        {
            if(level < m_minLevel)
                return;
            
            time_t t = time(0);
            struct tm* now= localtime(&t);
            m_file << "[" << (now->tm_year + 1900) << '-'
                    << std::setfill('0') << std::setw(2)
                    << (now->tm_mon + 1) << '-'
                    << std::setfill('0') << std::setw(2)
                    << now->tm_mday <<" "
                    << std::setfill('0') << std::setw(2)
                    << now->tm_hour <<":"
                    << std::setfill('0') << std::setw(2)
                    << now->tm_min <<":"
                    << std::setfill('0') 
                    << std::setw(2) << now->tm_sec << "] ";

            if(level == LogLevel::DEBUG)
                m_file << "Debug:   ";
            else if(level == LogLevel::INFO)
                m_file << "Info:    ";
            else if(level == LogLevel::WARN)
                m_file << "Warning:     ";
            else if(level == LogLevel::ERR)
                m_file << "Error:   ";

            m_file << str << std::endl;
            m_file.flush();
        }
    };
    // 缓冲
    class BufferSink:public Utils::LogSink
    {
    protected:
        std::vector<std::pair<Utils::LogLevel,std::string>> m_buffer;

    public:
        BufferSink(const LogLevel minLevel):LogSink(minLevel)
        {
            m_buffer.reserve(10000);
        }

        virtual void write(const Utils::LogLevel level,const std::string& str)
        {
            if(level < m_minLevel)
                return;
            m_buffer.push_back({level,str});
        }

        std::vector<std::pair<Utils::LogLevel,std::string>>& getBuffer()
        {
            return m_buffer;
        }

        void clearBuffer()
        {
            m_buffer.clear();
        }
    };


    class Logger{
    public:
        Logger(){
            m_active = true;
        }
        ~Logger(){
            m_sinks.clear();
        }
    protected:
        // 创建一个数组用来存放Log 指针
        std::vector<std::shared_ptr<LogSink>> m_sinks;
        bool m_active;
    
    public:
        void addSink(std::shared_ptr<LogSink> sink){
            m_sinks.push_back(sink);
        }

        void removeSink(std::shared_ptr<LogSink> sink){
            for (auto it = m_sinks.begin(); it != m_sinks.end(); ++it)
            {
                if(*it == sink)
                {
                    m_sinks.erase(it);
                    break;
                }
            }
        }

        std::vector<std::shared_ptr<LogSink>>& getSinks(){
            return m_sinks;
        }

        void write(const LogLevel level,const std::string& str){
            if(!m_active)
                return;
            for (unsigned int i = 0; i < m_sinks.size(); ++i)
                m_sinks[i]->write(level,str);
        }

        void activate(const bool b){
            m_active = b;
        }
    };

    class LogStream{
    public:
        LogStream(Logger *logger,const LogLevel level)
        :m_logger(logger),m_level(level)
        {}

        template<typename T>
        LogStream& operator<<(T const& value){
            m_buffer << value;
            return *this;
        }
        ~LogStream()
        {
            m_logger->write(m_level,m_buffer.str());
        }
    protected:
        LogLevel m_level;
        Logger *m_logger;
        std::ostringstream m_buffer;
    };

    extern Utils::Logger logger;

    #define LOG_DEBUG Utils::LogStream(&Utils::logger, Utils::LogLevel::DEBUG)
	#define LOG_INFO Utils::LogStream(&Utils::logger, Utils::LogLevel::INFO)
	#define LOG_WARN Utils::LogStream(&Utils::logger, Utils::LogLevel::WARN)
	#define LOG_ERR Utils::LogStream(&Utils::logger, Utils::LogLevel::ERR)
    // 声明一下该类中的变量
	#define INIT_LOGGING Utils::Logger Utils::logger;

}