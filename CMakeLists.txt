cmake_minimum_required(VERSION 3.0)

project(l3pbatchconfig)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

FIND_PACKAGE(Qt5 COMPONENTS Core Gui Widgets Network REQUIRED)

LINK_LIBRARIES(Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Network)

ADD_EXECUTABLE(l3pbatchconfig
        mainwindow.ui
        main.h
        mainwindow.h
        main.cpp
        mainwindow.cpp
        l3pbatchconfig.qrc
)
