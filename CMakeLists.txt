cmake_minimum_required(VERSION 3.0.0)
project(example VERSION 0.1.0)

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

add_executable(example main.cpp)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
