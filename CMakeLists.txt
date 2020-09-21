cmake_minimum_required(VERSION 3.10)

project(MotionViewer)

# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message("Setting build type to 'RelWithDebInfo' as none was specified.")
    set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
            "MinSizeRel" "RelWithDebInfo")
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 COMPONENTS Widgets Network Serialport Xml LinguistTools OpenGL REQUIRED)
find_package(Assimp REQUIRED)
find_package(OpenGL REQUIRED)

file(GLOB SRC "MotionViewer/*.cpp")
file(GLOB HEADER "MotionViewer/*.h")
file(GLOB UI "MotionViewer/*.ui")
file(GLOB RES "MotionViewer/*.qrc")
file(GLOB SHADERS "MotionViewer/gl/*.qrc")

file(GLOB TRANS "MotionViewer/*.ts")
#qt5_create_translation(QM_FILES ${SRC} de.ts)
qt5_add_translation(QM_FILES ${TRANS})


set(LIBS Qt5::Widgets Qt5::Network Qt5::SerialPort Qt5::Xml Qt5::OpenGL ${ASSIMP_LIBRARIES})

if(WIN32)
    set(LIBS ${LIBS} OpenGL::GL) #注意，windows下import target只有OpenGL::GL
elseif(UNIX AND NOT APPLE)
    set(LIBS ${LIBS} OpenGL::OpenGL)
else()
    set(LIBS )
endif(WIN32)


link_directories(${ASSIMP_LIBRARY_DIRS})
add_executable(MotionViewer ${SRC} ${HEADER} ${UI} ${RES} ${QM_FILES} ${SHADERS})
target_include_directories(MotionViewer PUBLIC ${ASSIMP_INCLUDE_DIRS} ${PROJECT_NAME})
target_link_libraries(MotionViewer ${LIBS})

#3D Model File
file(GLOB MODELS
        "MotionViewer/gl/*.stl"
        "MotionViewer/gl/*.dae"
)

file(COPY ${MODELS} DESTINATION ${CMAKE_BINARY_DIR})

#tooldef.xml file
file(COPY "MotionViewer/tooldef.xml" DESTINATION ${CMAKE_BINARY_DIR})