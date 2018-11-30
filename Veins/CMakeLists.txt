cmake_minimum_required(VERSION 2.8)
set_property(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS TRUE)

if(DEFINED ENV{OPENCV_INSTALL_DIR})
    find_package(OpenCV 3.4 REQUIRED PATHS $ENV{OPENCV_INSTALL_DIR})
else()
    message("OPENCV_INSTALL_DIR not set, searching in default location(s)")
    find_package(OpenCV 3.4 REQUIRED)
endif(DEFINED ENV{OPENCV_INSTALL_DIR})

include_directories( ${OpenCV_INCLUDE_DIRS} )

project(elo328_pro)
add_executable(${PROJECT_NAME} "main.cpp")
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
