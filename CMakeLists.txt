cmake_minimum_required(VERSION 3.21)

project(BabyPhoto VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(BabyPhoto
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    gui_filter_adapter.cpp
    gui_filter_adapter.h
)

target_include_directories(BabyPhoto PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(BabyPhoto PRIVATE Qt6::Widgets)
