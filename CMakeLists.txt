cmake_minimum_required(VERSION 2.6)
project (pginfo)

# Include scripts
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/scripts")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Wall -Wextra")
set(CMAKE_CXX_FLAGS_ANALYSIS "${CMAKE_CXX_FLAGS} --analyse")

# Trick to add header files to projects in IDEs like Visual Studio and XCode
FILE(GLOB_RECURSE HEADERS "include/**.h")
add_custom_target(${PROJECT_NAME}_headers
  SOURCES ${HEADERS}
)

# Search for YamlCpp; we depend on this for logging output.
find_package(YamlCpp REQUIRED)
find_package(Boost REQUIRED COMPONENTS graph)

# Include external libraries
add_subdirectory(external/googletest)
set(CPPLOGGING_STANDALONE OFF)
add_subdirectory(external/cpplogging)
set(CPPCLI_STANDALONE OFF)
add_subdirectory(external/cppcli)


include_directories(
  include
  ${cpplogging_SOURCE_DIR}/include
  ${cppcli_SOURCE_DIR}/include
  ${YAMLCPP_INCLUDE_DIR}
  ${Boost_INCLUDE_DIR}
)

add_executable (pginfo
  src/pginfo.cpp
)

target_link_libraries (pginfo ${YAMLCPP_LIBRARY} cppcli cpplogging)

add_subdirectory(test)
