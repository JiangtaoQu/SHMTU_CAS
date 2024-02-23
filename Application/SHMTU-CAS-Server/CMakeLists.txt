cmake_minimum_required(VERSION 3.22)
project(SHMTU_CAS_Server)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (WIN32)
    # Windows

    set(OpenCV_DIR "./3rdparty/OpenCV/opencv500-vs2022/install")
    set(Poco_DIR "./3rdparty/Poco/poco-poco-1.13.2-build/install/cmake")
    set(ncnn_DIR "./3rdparty/NCNN/ncnn-vs2022-debug/install/lib/cmake/ncnn")
elseif (LINUX)
    # Linux

    set(ncnn_DIR "./3rdparty/NCNN/ncnn-vs2022-debug/install/lib/cmake/ncnn")
endif ()
# macOS can install via HomeBrew

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

find_package(Poco REQUIRED COMPONENTS Foundation Net)
find_package(ncnn REQUIRED)

include_directories(
        ${PROJECT_SOURCE_DIR}/3rdparty/Tclap/tclap-1.4.0-rc1/include
)

set(SOURCES
        main.cpp

        CAS_OCR.cpp
        CAS_OCR.h
)

# Set Icon
if (WIN32)
    set(WINDOWS_ICON_RC_FILE "app_win32.rc")
endif ()

add_executable(SHMTU_CAS_Server
        ${SOURCES}

        # Windows Icon
        ${WINDOWS_ICON_RC_FILE}
)

target_link_libraries(SHMTU_CAS_Server ${OpenCV_LIBS})
target_link_libraries(SHMTU_CAS_Server ${Poco_LIBRARIES})
target_link_libraries(SHMTU_CAS_Server ncnn)
