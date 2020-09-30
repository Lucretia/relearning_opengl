cmake_minimum_required(VERSION 3.0.0)

project(shaders VERSION 0.1.0)

# project(learnopengl VERSION 0.1.0)

# include(CTest)
# enable_testing()

# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)

# add_subdirectory(../../glad/src)
# add_subdirectory(src)

include_directories(../../glad/include)
include_directories(../../utils/include)

add_link_options(-lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl)

add_executable(${PROJECT_NAME} src/${PROJECT_NAME}.cpp
               ../../glad/src/glad.c
               ../../utils/src/shader.cpp)
