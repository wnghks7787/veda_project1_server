cmake_minimum_required(VERSION 3.16)

project(veda_project1_server LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Widgets Network)

add_executable(veda_project1_server
  main.cpp
  server.h server.cpp
  login_module.h login_module.cpp
  user_module.h user_module.cpp
)
target_link_libraries(veda_project1_server PRIVATE Qt${QT_VERSION_MAJOR}::Core PRIVATE Qt6::Network)

include(GNUInstallDirs)
install(TARGETS veda_project1_server
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
