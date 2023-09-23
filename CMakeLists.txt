cmake_minimum_required(VERSION 3.25)
project(Mikann_Renderer_Engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


# 版本分歧部分
if(APPLE)
    set(CMAKE_PREFIX_PATH "/Users/m1kann/Qt/5.15.2/clang_64")
    set(CMAKE_OSX_ARCHITECTURES "x86_64")

    set(LIBS "${CMAKE_SOURCE_DIR}/3rdParty/lib/mingw64/libassimp.dll.a")    # 改
    # 设置DLL的源路径和目标路径 (改)
    set(ASSIMP_DLL_SRC "${CMAKE_SOURCE_DIR}/3rdParty/lib/mingw64/libassimp-5.dll")

    include_directories(3rdParty/include/clang64)
elseif(WIN32)
    set(CMAKE_PREFIX_PATH "C:/Qt/Qt5.15.2/5.15.2/mingw81_64")

    set(LIBS "${CMAKE_SOURCE_DIR}/3rdParty/lib/mingw64/libassimp.dll.a")
    # 设置DLL的源路径和目标路径
    set(ASSIMP_DLL_SRC "${CMAKE_SOURCE_DIR}/3rdParty/lib/mingw64/libassimp-5.dll")

    include_directories(3rdParty/include/mingw64)
endif()


include_directories(
        src/include
        3rdParty/include
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

target_link_libraries(${PROJECT_NAME}
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        ${LIBS}
        )

set(ASSIMP_DLL_DST "${CMAKE_CURRENT_BINARY_DIR}/libassimp-5.dll")
# 创建一个自定义命令来复制DLL
add_custom_command(
        OUTPUT ${ASSIMP_DLL_DST}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${ASSIMP_DLL_SRC}
        ${ASSIMP_DLL_DST}
        DEPENDS ${ASSIMP_DLL_SRC}
        COMMENT "Copying libassimp-5.dll to target directory"
)

# 创建一个自定义目标来执行上面的复制命令
add_custom_target(
        CopyAssimpDLL ALL
        DEPENDS ${ASSIMP_DLL_DST}
)

add_dependencies(${PROJECT_NAME} CopyAssimpDLL)


