set(SIMULATION_LINK_LIBRARIES utils)
set(SIMULATION_DEPENDENCIES utils)

add_executable(BarDemo
    main.cpp
)

#add_dependencies(BarDemo ${SIMULATION_DEPENDENCIES})
target_link_libraries(BarDemo ${SIMULATION_LINK_LIBRARIES})
target_include_directories(BarDemo PUBLIC
    ${PROJECT_SOURCE_DIR}/include/utils
)