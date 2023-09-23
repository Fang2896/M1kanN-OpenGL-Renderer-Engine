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
    # set(CMAKE_PREFIX_PATH "C:/Qt/Qt5.15.2/5.15.2/mingw81_64")
    set(CMAKE_PREFIX_PATH "C:/Qt/Qt5.15.2/5.15.2/mingw81_32")
    # set(CMAKE_PREFIX_PATH "C:/Qt/Qt5.15.2/5.15.2/msvc2019_64")
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


# 设置DLL的源路径和目标路径
set(ASSIMP_DLL_SRC "${CMAKE_SOURCE_DIR}/src/3rdParty/lib/libassimp-5.dll")
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

# 确保你的主要目标（例如你的执行文件）依赖于上面的自定义目标
# 这样每次构建你的项目时，上面的复制命令都会被执行。
# 以下假设你的目标名称为"YourTargetName"，请将其替换为你实际的目标名称。
add_dependencies(${PROJECT_NAME} CopyAssimpDLL)


