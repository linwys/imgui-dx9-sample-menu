cmake_minimum_required(VERSION 3.16)
project(Imgui_DX9_Sample_UI CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE IMGUI_SOURCES "src/imgui/*.cpp")
file(GLOB_RECURSE PROJECT_SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} WIN32 ${PROJECT_SOURCES} ${IMGUI_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE "src" "src/imgui" "src/uicomponents" "src/tabs")
target_link_libraries(${PROJECT_NAME} PRIVATE d3d9 dwmapi)