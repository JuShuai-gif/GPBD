file(GLOB ALL_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_SOURCE_DIR}/include/Simulation/*.h"
)

# 静态链接库的名字为 PBD
add_library(Simulation
		 ${PROJECT_PATH}/Common/Common.h
         ${ALL_SOURCES}		
		CMakeLists.txt
)

############################################################
# Discregrid
############################################################
target_include_directories(Simulation PUBLIC ${Discregrid_INCLUDE_DIR})
if (TARGET Ext_Discregrid)
	add_dependencies(Simulation Ext_Discregrid)
endif()

############################################################
# GenericParameters
############################################################
target_include_directories(Simulation PUBLIC ${GenericParameters_INCLUDE_DIR})
if(TARGET Ext_GenericParameters)
	add_dependencies(Simulation Ext_GenericParameters)
endif()

#target_link_libraries(Simulation PUBLIC PBD)

target_include_directories(Simulation
    PUBLIC
        ${PROJECT_SOURCE_DIR}/extern/eigen
        ${PROJECT_SOURCE_DIR}/include/Simulation
        ${PROJECT_SOURCE_DIR}/extern
        ${PROJECT_SOURCE_DIR}/Common
)

message("PROJECT_SOURCE_DIR ------ ${PROJECT_SOURCE_DIR}")
#set_target_properties(Simulation PROPERTIES LINKER_LANGUAGE C++)

install(TARGETS Simulation
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

# 将后缀名为.inl的复制到include/Simulation
#install(DIRECTORY .
#DESTINATION include/Simulation
#FILES_MATCHING PATTERN "*.inl")