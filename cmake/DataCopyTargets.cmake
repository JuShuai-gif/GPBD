# 将项目所需的着色器文件从源代码目录复制到可执行文件的目录
# 以便运行时能够读取，是一种常见的资源管理方法，确保了项目运行的正确性和完整性

add_custom_target(CopyPBDShaders
    ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/data/shaders
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/
    COMMENT "Copying PBD shaders"
)
# 设置自定义目标的属性，将其归入Data copy的文件夹中，方便管理整个项目结构
set_target_properties(CopyPBDShaders PROPERTIES FOLDER "Data copy")

# 模型资源
add_custom_target(CopyPBDModels
    ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/data/models
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/models
    COMMENT "Copying PBD models"
)
set_target_properties(CopyPBDModels PROPERTIES FOLDER "Data copy")

# 场景资源
add_custom_target(CopyPBDScenes
    ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/data/scenes
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/scenes
    COMMENT "Copying PBD scenes"
)
set_target_properties(CopyPBDScenes PROPERTIES FOLDER "Data copy")

# imgui字体资源
add_custom_target(CopyInguiFonts
    ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/extern/imgui/misc/fonts
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/fonts
    COMMENT "Copying PBD models"
)
set_target_properties(CopyInguiFonts PROPERTIES FOLDER "Data copy")
