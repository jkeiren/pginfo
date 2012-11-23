cmake_minimum_required(VERSION 2.6)
project (pginfo)

# Include scripts
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/scripts")

# Include external libraries
add_subdirectory(external/googletest)
add_subdirectory(external/cppcli)
add_subdirectory(external/cpplogging)

# Trick to add header files to projects in IDEs like Visual Studio and XCode
FILE(GLOB HEADERS "include/**.h")
FILE(GLOB SOURCES "src/**.h")
add_custom_target(${PROJECT_NAME}_headers
  SOURCES ${HEADERS} ${SUB_HEADERS}
)

# Search for YamlCpp; we depend on this for logging output.
find_package(YamlCpp REQUIRED)

include_directories(
  include
  ${cpplogging_SOURCE_DIR}/include
  ${cppcli_SOURCE_DIR}/include
  ${YAMLCPP_INCLUDE_DIR}
)

add_executable (pginfo
  src/pginfo.cpp
)

target_link_libraries (pginfo ${YAMLCPP_LIBRARY} cppcli cpplogging)

add_subdirectory(test)
