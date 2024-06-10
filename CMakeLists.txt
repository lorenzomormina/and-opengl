cmake_minimum_required(VERSION 3.27)
project(MyProject)

add_executable(main main.cpp shader.h  "Rectangle.h" "Texture.h" "Btn1.h" "text.h" "BtnOp.h" "Btn2.h" "BtnRes.h" "platform.h"   "globals.h" "TexLed.h" "Expr.h")

target_include_directories(main PUBLIC ${CMAKE_SOURCE_DIR})

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(main PRIVATE glfw)

find_package(glad CONFIG REQUIRED)
target_link_libraries(main PRIVATE glad::glad)

find_package(Stb REQUIRED)
target_include_directories(main PRIVATE ${Stb_INCLUDE_DIR})

find_package(Freetype REQUIRED)
target_link_libraries(main PRIVATE Freetype::Freetype) # since CMake 3.10

find_package(glm CONFIG REQUIRED)
target_link_libraries(main PRIVATE glm::glm)

find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(main PRIVATE nlohmann_json::nlohmann_json)

file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})