#include <iostream>
#include "Logger.h"
#include "StringTools.h"
#include "Timing.h"
#include "FileSystem.h"
INIT_LOGGING
INIT_TIMING

using namespace std;
int main(){
    // 测试 Logger
    Utils::logger.addSink(unique_ptr<Utils::ConsoleSink>(new Utils::ConsoleSink(Utils::LogLevel::INFO)));
	Utils::logger.addSink(shared_ptr<Utils::BufferSink>(new Utils::BufferSink(Utils::LogLevel::DEBUG)));
    LOG_INFO << "Number of triangles: ";
    // 测试 StringTool
    std::string input_str = "Hello woeld! This is a test string.";
    std::vector<std::string> tokens;
    Utils::StringTools::tokenize(input_str,tokens);

    for ( auto token:tokens)
    {
        std::cout << token << std::endl;
    }
    
    START_TIMING("00")
    for (size_t i = 0; i < 100000000; i++)
    {}
    STOP_TIMING_PRINT

    START_TIMING("01")
    for (size_t i = 0; i < 100000000; i++)
    {}
    STOP_TIMING_PRINT

    START_TIMING("02")
    for (size_t i = 0; i < 100000000; i++)
    {}
    STOP_TIMING_PRINT

    START_TIMING("03")
    for (size_t i = 0; i < 100000000; i++)
    {}
    STOP_TIMING_PRINT

    START_TIMING("04")
    for (size_t i = 0; i < 100000000; i++)
    {}
    STOP_TIMING_PRINT

    STOP_TIMING_AVG_PRINT
    Utils::Timing::printAverageTimes();

    std::string filePath = "E:\\Project\\GPBD\\test.txt";
    std::string fileDir = "E:\\Project\\GPBD";
    std::string filter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
    std::string directoryPath = Utils::FileSystem::getFilePath(filePath);
    std::string getFileName = Utils::FileSystem::getFileName(filePath);
    std::string getFileNameWithExt = Utils::FileSystem::getFileNameWithExt(filePath);
    std::string getFileExt = Utils::FileSystem::getFileExt(filePath);
    std::string normalizePath = Utils::FileSystem::normalizePath(filePath);
    bool isRelativePath = Utils::FileSystem::isRelativePath(filePath);
    int makeDir = Utils::FileSystem::makeDir(filePath);
    int makeDirs = Utils::FileSystem::makeDirs(filePath);
    bool fileExists = Utils::FileSystem::fileExists(filePath);
    std::cout << "directoryPath " << directoryPath << std::endl;
    std::cout << "getFileName " << getFileName << std::endl;
    std::cout << "getFileNameWithExt " << getFileNameWithExt << std::endl;
    std::cout << "getFileExt " << getFileExt << std::endl;
    std::cout << "normalizePath " << normalizePath << std::endl;
    std::cout << "isRelativePath " << isRelativePath << std::endl;
    std::cout << "makeDir " << makeDir << std::endl;
    std::cout << "makeDirs " << makeDirs << std::endl;


    std::string dir = Utils::FileSystem::fileDialog(0,fileDir,filter);
    std::cout << dir << std::endl;
    system("pause");
    return 0;

}