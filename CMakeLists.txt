cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(MotionViewer)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 COMPONENTS Core Widgets Network SerialPort Xml REQUIRED)
