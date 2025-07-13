cmake_minimum_required(VERSION 3.12)
#-------------------------------------------------------------------------------------------
# I'm going to use vcpk in most cases for our install of 3rd party libs
# this is going to check the environment variable for CMAKE_TOOLCHAIN_FILE and this must point to where
# vcpkg.cmake is in the University this is set in your .bash_profile to
# export CMAKE_TOOLCHAIN_FILE=/public/devel/2020/vcpkg/scripts/buildsystems/vcpkg.cmake
#-------------------------------------------------------------------------------------------
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE AND DEFINED ENV{CMAKE_TOOLCHAIN_FILE})
   set(CMAKE_TOOLCHAIN_FILE $ENV{CMAKE_TOOLCHAIN_FILE})
endif()

# Name of the project
project(FeatherBuild)
# This is the name of the Exe change this and it will change everywhere
set(TargetName Feather)
# This will include the file NGLConfig.cmake, you need to add the location to this either using
# -DCMAKE_PREFIX_PATH=~/NGL or as a system environment variable. 
find_package(NGL CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(gl3w CONFIG REQUIRED)
find_package(OpenMP)
find_package(OpenImageIO CONFIG)
# Instruct CMake to run moc automatically when needed (Qt projects only)
set(CMAKE_AUTOMOC ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS ${PROJECT_SOURCE_DIR}/ui)

# run rcc if required
set(CMAKE_AUTORCC ON)

# find Qt we can use either Qt6 or Qt5
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets OpenGLWidgets)
if (Qt6_FOUND)
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets OpenGLWidgets)
else()
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
endif()

# use C++ 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
# Set the name of the executable we want to build
add_executable(${TargetName})

set(UI_FILES ${PROJECT_SOURCE_DIR}/ui/mainwindow.ui)
# Add NGL include path
include_directories(include $ENV{HOME}/NGL/include)
target_sources(${TargetName} PRIVATE ${PROJECT_SOURCE_DIR}/src/main.cpp  
			${PROJECT_SOURCE_DIR}/src/NGLScene.cpp  
			${PROJECT_SOURCE_DIR}/include/NGLScene.h
            ${PROJECT_SOURCE_DIR}/src/Curve.cpp
            ${PROJECT_SOURCE_DIR}/include/Curve.h
            ${PROJECT_SOURCE_DIR}/include/Feather.h
            ${PROJECT_SOURCE_DIR}/src/Feather.cpp
            ${PROJECT_SOURCE_DIR}/src/mainwindow.cpp
            ${PROJECT_SOURCE_DIR}/include/mainwindow.h
            ${UI_FILES}
)

target_link_libraries(${TargetName} PRIVATE  NGL Qt${QT_VERSION_MAJOR}::Widgets)

if (Qt6_FOUND)
    target_link_libraries(${TargetName} PRIVATE Qt${QT_VERSION_MAJOR}::OpenGLWidgets)
endif()
#################################################################################
# Testing code
#################################################################################
find_package(GTest CONFIG REQUIRED)
include(GoogleTest)
enable_testing()
add_executable(FeatherTests)
target_sources(FeatherTests PRIVATE tests/FeatherTest.cpp
        src/Curve.cpp src/Feather.cpp include/Curve.h include/Feather.h
)
target_link_libraries(FeatherTests PRIVATE GTest::gtest GTest::gtest_main NGL Qt${QT_VERSION_MAJOR}::Widgets)
if (Qt6_FOUND)
    target_link_libraries(FeatherTests PRIVATE Qt${QT_VERSION_MAJOR}::OpenGLWidgets)
endif()
gtest_discover_tests(FeatherTests)
