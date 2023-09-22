cmake_minimum_required(VERSION 3.25)
project(Mikann_Renderer_Engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(APPLE)
    set(CMAKE_PREFIX_PATH "/Users/m1kann/Qt/5.15.2/clang_64")
    set(CMAKE_OSX_ARCHITECTURES "x86_64")
elseif(WIN32)
    set(CMAKE_PREFIX_PATH "C:/Qt/Qt5.15.2/5.15.2/mingw81_64")
endif()

include_directories(
        src/include
        src/3rdParty/include
)

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

set(QRC_SOURCE_FILES res.qrc)
qt_add_resources(${QRC_SOURCE_FILES})

file(GLOB_RECURSE SOURCE_FILES
        src/*.cpp
        src/*.hpp
        src/*.ui
        )

add_executable(${PROJECT_NAME}
        ${QRC_SOURCE_FILES}
        ${SOURCE_FILES})

set(LIBS "${CMAKE_SOURCE_DIR}/src/3rdParty/lib/libassimp.dll.a")

target_link_libraries(${PROJECT_NAME}
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        ${LIBS}
        )

