file(GLOB ALL_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_SOURCE_DIR}/include/PBD/*.h"
)
# 静态链接库的名字为 PBD
add_library(PBD
		 ${PROJECT_PATH}/Common/Common.h
         ${ALL_SOURCES}		
		CMakeLists.txt
)
message("PROJECT_PATH---- ${PROJECT_PATH}")

find_package( Eigen3 REQUIRED )
include_directories( ${EIGEN3_INCLUDE_DIR} )

target_include_directories(PBD PUBLIC
    ${PROJECT_PATH}/include/PBD
)

set_target_properties(Simulation PROPERTIES LINKER_LANGUAGE C++)

install(TARGETS PBD
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)