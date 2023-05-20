include(GetGitRevisionDescription)
get_git_head_revision(GIT_REFSPEC GIT_SHA1)
git_local_changes(GIT_LOCAL_CHANGES)

if (${GIT_LOCAL_CHANGES} STREQUAL "DIRTY")
	set(COMPILER_MESSAGE "#pragma WARNING(Local changes not committed.)")
endif()

file(STRINGS "../../version.txt" PBD_VERSION)

#configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Version.h.in ${CMAKE_CURRENT_SOURCE_DIR}/Version.h @ONLY)

file(GLOB ALL_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_SOURCE_DIR}/include/utils/*.h"
)

add_library(utils
    ${ALL_SOURCES}

    CMakeLists.txt
)

include_directories(
    ${PROJECT_SOURCE_DIR}/include/utils
)

#添加GenericParameters

install(TARGETS utils
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

