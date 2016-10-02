cmake_minimum_required(VERSION 3.3)
project(hand_detect_recognition)

include_directories(D:/openccc/install/include/opencv2)
include_directories(D:/openccc/install/include/opencv)
include_directories(D:/openccc/install/include)
include_directories(D:/complier/mingw64/x86_64-w64-mingw32/include)
include_directories(F:/desktop/show/DirectShow/Include)
#find_library(PROTOBUF_LIBRARY protobuf HINTS d:/Code/Opencv-2.4.11/opencv-gcc-x64-530-seh/install/x64/mingw/lib)
#link_directories(d:/Code/Opencv-2.4.11/opencv-gcc-x64-530-seh/install/x64/mingw/lib/)
#target_link_libraries(None PUBLIC ${PROTOBUF_LIBRARY})
link_libraries(
        ws2_32
        D:/openccc/install/x64/mingw/lib/libopencv_calib3d249.dll.a
        D:/openccc/install/x64/mingw/lib/libopencv_contrib249.dll.a
        D:/openccc/install/x64/mingw/lib/libopencv_core249.dll.a
        D:/openccc/install/x64/mingw/lib/libopencv_features2d249.dll.a
        D:/openccc/install/x64/mingw/lib/libopencv_flann249.dll.a
        D:/openccc/install/x64/mingw/lib/libopencv_gpu249.dll.a
        D:/openccc/install/x64/mingw/lib/libopencv_highgui249.dll.a
        D:/openccc/install/x64/mingw/lib/libopencv_imgproc249.dll.a
        D:/openccc/install/x64/mingw/lib/libopencv_legacy249.dll.a
        D:/openccc/install/x64/mingw/lib/libopencv_ml249.dll.a
        D:/openccc/install/x64/mingw/lib/libopencv_nonfree249.dll.a
        D:/openccc/install/x64/mingw/lib/libopencv_objdetect249.dll.a
        D:/openccc/install/x64/mingw/lib/libopencv_ocl249.dll.a
        D:/openccc/install/x64/mingw/lib/libopencv_photo249.dll.a
        D:/openccc/install/x64/mingw/lib/libopencv_stitching249.dll.a
        D:/openccc/install/x64/mingw/lib/libopencv_superres249.dll.a
        D:/openccc/install/x64/mingw/lib/libopencv_ts249.a
        D:/openccc/install/x64/mingw/lib/libopencv_video249.dll.a
        D:/openccc/install/x64/mingw/lib/libopencv_videostab249.dll.a
        D:/complier/mingw64/x86_64-w64-mingw32/lib/libopengl32.a
        F:/desktop/show/DirectShow/Lib/strmiids.lib
        D:/complier/mingw64/x86_64-w64-mingw32/lib/libglu32.a)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp hand_detect.cpp hand_detect.h hand_recognition.cpp hand_recognition.h)
add_executable(hand_detect_recognition ${SOURCE_FILES})