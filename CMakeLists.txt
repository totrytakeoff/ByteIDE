cmake_minimum_required(VERSION 3.16)

project(ByteIDE VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# 手动指定需要 MOC 处理的头文件
set(MOC_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/qscintilla/Qsci/qsciscintilla.h
    # 其他需要 MOC 处理的头文件
)

qt_wrap_cpp(MOC_SOURCES ${MOC_HEADERS})

set(PROJECT_SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ByteIDE
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        src/resourcemanager.h src/resourcemanager.cpp
        src/editarea.h src/editarea.cpp
        src/newfile.h src/newfile.cpp
        src/terminal.cpp src/terminal.h
        src/coderunner.h src/coderunner.cpp
        src/searchwidget.h src/searchwidget.cpp
        src/codetabwidget.h src/codetabwidget.cpp
        res/res.qrc
        res/res.rc
        src/updatelogwidget.h src/updatelogwidget.cpp
    )
else()
    if(ANDROID)
        add_library(ByteIDE SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(ByteIDE
            ${PROJECT_SOURCES}
            res/res.rc
        )
    endif()
endif()

set(QSCILIB ${CMAKE_CURRENT_LIST_DIR}/qscintilla/libqscintilla2_qt6d.a)

target_link_libraries(ByteIDE PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(ByteIDE PRIVATE ${CMAKE_SOURCE_DIR}/qscintilla/libqscintilla2_qt6d.a)
target_link_directories(ByteIDE PRIVATE ${CMAKE_SOURCE_DIR}/qscintilla/Qsci)

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.ByteIDE)
endif()
set_target_properties(ByteIDE PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# 将 MOC 文件添加到可执行文件
target_sources(ByteIDE PRIVATE ${MOC_SOURCES})

include(GNUInstallDirs)
install(TARGETS ByteIDE
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# 设置图标（仅适用于Windows）
if(WIN32)
    set(RESOURCE_SCRIPT "${CMAKE_SOURCE_DIR}/res/res.rc")
    set_source_files_properties(${RESOURCE_SCRIPT} PROPERTIES LANGUAGE RC)
    target_sources(${PROJECT_NAME} PRIVATE ${RESOURCE_SCRIPT})
endif()

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(ByteIDE)
endif()
