cmake_minimum_required(VERSION 3.15)
project(MyGLProject LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找库
find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(SOIL2 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

# 可执行目标
add_executable(
    ${PROJECT_NAME}
    src/main.cpp
    src/Utils.cpp
    src/Torus.cpp
)

# 链接库
target_link_libraries(${PROJECT_NAME}
    PRIVATE
      glfw
      GLEW::GLEW
      glm::glm
      soil2
      OpenGL::GL
)

# 包含目录（通常 find_package 自动处理 include）
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)

# === 复制 shaders 文件夹到构建目录 ===
file(GLOB SHADER_FILES "${CMAKE_SOURCE_DIR}/shaders/*")

add_custom_target(copy_shaders ALL
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${SHADER_FILES}
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMENT "Copying shader files to build directory"
)

# === 复制 assets 文件夹到构建目录 ===
file(GLOB ASSET_FILES "${CMAKE_SOURCE_DIR}/assets/*")

add_custom_target(copy_assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${ASSET_FILES}
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMENT "Copying asset files to build directory"
)