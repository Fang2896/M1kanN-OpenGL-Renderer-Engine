cmake_minimum_required(VERSION 3.21)
project(M1kanN_OpenGL_Renderer_Engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


# 版本分歧部分
if(APPLE)
    set(CMAKE_PREFIX_PATH "/Users/m1kann/Qt/5.15.2/clang_64")
    set(CMAKE_OSX_ARCHITECTURES "x86_64")
    set(ASSIMP_LIBRARIES "${CMAKE_SOURCE_DIR}/3rdParty/lib/clang64/libassimpd.5.dylib")
    set(ASSIMP_DLL_SRC "${CMAKE_SOURCE_DIR}/3rdParty/lib/clang64/libassimpd.5.dylib")
    set(ASSIMP_DLL_DST "${CMAKE_CURRENT_BINARY_DIR}/libassimpd.5.dylib")

elseif(WIN32)
    set(CMAKE_PREFIX_PATH "C:/Qt/Qt5.15.2/5.15.2/mingw81_64")
    set(ASSIMP_LIBRARIES "${CMAKE_SOURCE_DIR}/3rdParty/lib/mingw64/libassimp.dll.a")
    set(ASSIMP_DLL_SRC "${CMAKE_SOURCE_DIR}/3rdParty/lib/mingw64/libassimp-5.dll")
    set(ASSIMP_DLL_DST "${CMAKE_CURRENT_BINARY_DIR}/libassimp-5.dll")
endif()


include_directories(
        src/include
        src/include/object
        src/include/ui
        src/include/utils

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
        ${ASSIMP_LIBRARIES}
        )


# 创建一个自定义命令来复制DLL
add_custom_command(
        OUTPUT ${ASSIMP_DLL_DST}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${ASSIMP_DLL_SRC}
        ${ASSIMP_DLL_DST}
        DEPENDS ${ASSIMP_DLL_SRC}
        COMMENT "Copying libassimp library to target directory"
)

# 创建一个自定义目标来执行上面的复制命令
add_custom_target(
        CopyAssimpDLL ALL
        DEPENDS ${ASSIMP_DLL_DST}
)

add_dependencies(${PROJECT_NAME} CopyAssimpDLL)

