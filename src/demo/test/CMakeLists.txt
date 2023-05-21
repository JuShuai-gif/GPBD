# 设置需要关联的库
set(SIMULATION_LINK_LIBRARIES utils)
set(SIMULATION_DEPENDENCIES utils)

add_executable(test
    test.cpp
)

#add_dependencies(BarDemo ${SIMULATION_DEPENDENCIES})
target_link_libraries(test ${SIMULATION_LINK_LIBRARIES})
target_include_directories(test PUBLIC
    ${PROJECT_SOURCE_DIR}/include/utils
)