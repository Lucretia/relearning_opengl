cmake_minimum_required(VERSION 3.0.0)

project(camera VERSION 0.1.0)

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
include_directories(../../stb_image/include)

add_link_options(-lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl)

add_executable(${PROJECT_NAME} src/${PROJECT_NAME}.cpp
               ../../glad/src/glad.c
               ../../stb_image/src/stb_image.cpp
               ../../utils/src/shader.cpp)
