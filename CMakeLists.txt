cmake_minimum_required(VERSION 3.0.0)
project(opencv_project VERSION 0.1.0)

include(CTest)
enable_testing()

# Find Package
# set( OpenCV_DIR "C:/Opencv3/build/x64/buildMinGW" )
set( OpenCV_DIR "C:/Opencv3/build/x64/MinGW" )
find_package( OpenCV REQUIRED )
message( STATUS "OpenCV library status:" )
message( STATUS "    config: ${OpenCV_DIR}" )
message( STATUS "    version: ${OpenCV_VERSION}" )
message( STATUS "    libraries: ${OpenCV_LIBS}" )
message( STATUS "    include path: ${OpenCV_INCLUDE_DIRS}" )
include_directories( ${OpenCV_INCLUDE_DIRS} )

set(ch2 "ch2")
set(ch3 "ch3")


add_executable(${ch2} main.cpp)
# add executable
add_executable( colorDetection  colorDetection.cpp colordetector.cpp)
add_executable( huesaturation huesaturation.cpp)
add_executable( histogram histograms.cpp)
add_executable( contentFinder contentFinder.cpp)
add_executable( finder finder.cpp)
add_executable( retrieve retrieve.cpp)

# link libraries
target_link_libraries(${ch2} ${OpenCV_LIBS})
target_link_libraries( colorDetection  ${OpenCV_LIBS})
target_link_libraries( huesaturation ${OpenCV_LIBS})
target_link_libraries( histogram ${OpenCV_LIBS})
target_link_libraries( contentFinder ${OpenCV_LIBS})
target_link_libraries( finder ${OpenCV_LIBS})
target_link_libraries( retrieve ${OpenCV_LIBS})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
