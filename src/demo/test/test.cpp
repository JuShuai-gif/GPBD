#include <iostream>
#include "Logger.h"
#include "StringTools.h"
#include "Timing.h"
#include "FileSystem.h"
#include "AABB.h"
#include "kdTree.h"
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
/*
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
*/
    PBD::AABB aabb1;
    Vector3r v00(1.0f,2.0f,3.0f);
    Vector3r v01(2.0f,3.0f,4.0f);
    aabb1.m_p[0] = v00;
    aabb1.m_p[1] = v01;


    PBD::AABB aabb2;
    Vector3r v10(1.5f,2.5f,3.5f);
    Vector3r v11(3.0f,3.0f,5.0f);
    std::cout << "v10  " << v10 << std::endl;
    std::cout << "v11  " << v11 << std::endl;
    aabb2.m_p[0] = v10;
    aabb2.m_p[1] = v11;

    if (PBD::AABB::pointInAABB(aabb1,Vector3r(1.5f,1.5f,1.5f)))
        std::cout << "in inter" << std::endl;
    else
        std::cout << "no in inter" << std::endl;
    
    if (PBD::AABB::interSection(aabb1,aabb2))
        std::cout << "intersection" << std::endl;
    
    PBD::AABB::getEdge(aabb1,'2',v00,v01);
    std::cout << "v0" << v00 << std::endl;
    std::cout << "v1" << v01 << std::endl;
    

    PBD::kdTree kd;
    
    //system("pause");
    return 0;

}