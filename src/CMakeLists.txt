#add_subdirectory(Simulation)
add_subdirectory(utils)
#add_subdirectory(PBD)

if(NOT PBD_LIBS_ONLY)
    add_subdirectory(demo)
    if (USE_PYTHON_BINDINGS)
        add_subdirectory(pyPBD)
    endif ()   
endif()
 