cmake_minimum_required(VERSION 3.15)
project(MyGLProject LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
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
