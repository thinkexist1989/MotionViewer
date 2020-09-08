cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(MotionViewer)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 COMPONENTS Widgets Network SerialPort Xml LinguistTools REQUIRED)

file(GLOB SRC "*.cpp")
file(GLOB HEADER "*.h")
file(GLOB UI "*.ui")
file(GLOB RES "*.qrc")

#qt5_create_translation(QM_FILES ${SRC} de.ts)
qt5_add_translation(QM_FILES "trans/*.ts")

set(LIBS Qt5::Widgets Qt5::Network Qt5::SerialPort Qt5::Xml)

add_executable(MotionViewer ${SRC} ${HEADER} ${UI} ${RES} )
target_link_libraries(MotionViewer ${LIBS})
