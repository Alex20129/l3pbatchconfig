CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(l3pbatchconfig)

SET(CMAKE_CXX_STANDARD 11)

SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTORCC ON)
SET(CMAKE_AUTOUIC ON)

FIND_PACKAGE(Qt5 COMPONENTS Core Gui Widgets Network REQUIRED)

LINK_LIBRARIES(Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Network)
LINK_LIBRARIES(-static-libgcc -static-libstdc++)

ADD_EXECUTABLE(l3pbatchconfig
        mainwindow.ui
        main.h
        mainwindow.h
        main.cpp
        mainwindow.cpp
        l3pbatchconfig.qrc)
