cmake_minimum_required(VERSION 3.5)

project(SHMTU_CAS_OCR_Demo_QT VERSION 0.1 LANGUAGES CXX)

if (WIN32)
    set(CMAKE_PREFIX_PATH "D:/Dev/Qt/6.6.1/msvc2019_64")
elseif (APPLE)
    set(CMAKE_PREFIX_PATH "/Users/konghaomin/Dev/Qt/Qt/6.6.1/macos")
endif ()

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt6 COMPONENTS Network REQUIRED)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

if (WIN32)
    # OpenCV
    set(
            OpenCV_DIR
            H:/SHMTU/shmtu-cas/Deploy/SHMTU_CAS_OCR_Demo_Windows/VC/3rdparty/OpenCV/opencv5_build/install
    )

    # Tencent NCNN
    set(
            ncnn_DIR
            H:/SHMTU/shmtu-cas/Deploy/SHMTU_CAS_OCR_Demo_Windows/VC/3rdparty/ncnn/ncnn_build/install/lib/cmake/ncnn
    )
endif ()

find_package(OpenCV REQUIRED)
find_package(ncnn REQUIRED)

set(
        TS_FILES

        SHMTU_CAS_OCR_Demo_QT_zh_CN.ts
)

set(PROJECT_SOURCES
        ${TS_FILES}

        main.cpp

        mainwindow.cpp
        mainwindow.h
        mainwindow.ui

        QtOpenCV.cpp
        QtOpenCV.hpp

        CAS_OCR.cpp
        CAS_OCR.h

        CAS_OCR_QT.cpp
        CAS_OCR_QT.h

        CasNetwork.cpp
        CasNetwork.h

        QtUIHelper.cpp
        QtUIHelper.h

        STD_CPP_Utils.cpp
        STD_CPP_Utils.h
)

# Set Icon
if (WIN32)

elseif (APPLE)
    # 设置 macOS 应用程序的图标文件
    set(MACOSX_BUNDLE_ICON_FILE icon.icns)

    # 将图标文件添加到资源中
    set_source_files_properties(
            ${MACOSX_BUNDLE_ICON_FILE} PROPERTIES
            MACOSX_PACKAGE_LOCATION "Resources"
    )
endif ()

if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(
            SHMTU_CAS_OCR_Demo_QT
            MANUAL_FINALIZATION MACOSX_BUNDLE

            ${PROJECT_SOURCES}

            ${MACOSX_BUNDLE_ICON_FILE}

            res.qrc
    )

    # Define target properties for Android with Qt 6 as:
    #    set_property(TARGET SHMTU_CAS_OCR_Demo_QT APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
    #                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
    # For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else ()
    if (ANDROID)
        add_library(SHMTU_CAS_OCR_Demo_QT SHARED
                ${PROJECT_SOURCES}
        )
        # Define properties for Android with Qt 5 after find_package() calls as:
        #    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else ()
        add_executable(SHMTU_CAS_OCR_Demo_QT
                ${PROJECT_SOURCES}
        )
    endif ()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif ()

target_link_libraries(SHMTU_CAS_OCR_Demo_QT PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(SHMTU_CAS_OCR_Demo_QT PRIVATE Qt6::Network)

# Add Link libraries
target_link_libraries(SHMTU_CAS_OCR_Demo_QT PRIVATE ncnn)
target_link_libraries(SHMTU_CAS_OCR_Demo_QT PRIVATE ${OpenCV_LIBS})

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if (${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.khm.shmtu.cas.ocr.demo)
endif ()
set_target_properties(SHMTU_CAS_OCR_Demo_QT PROPERTIES
        ${BUNDLE_ID_OPTION}
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS SHMTU_CAS_OCR_Demo_QT
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if (QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(SHMTU_CAS_OCR_Demo_QT)
endif ()
