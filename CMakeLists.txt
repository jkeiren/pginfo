cmake_minimum_required(VERSION 2.6)
project (pginfo)

# Include scripts
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/scripts")

# Trick to add header files to projects in IDEs like Visual Studio and XCode
FILE(GLOB HEADERS "include/*.h")
FILE(GLOB SUB_HEADERS "include/*/*.h")
add_custom_target(${PROJECT_NAME}_headers
  SOURCES ${HEADERS} ${SUB_HEADERS}
)

find_package(YamlCpp REQUIRED)
find_package(Mcrl2Utilities REQUIRED)
find_package(Boost COMPONENTS regex REQUIRED)

link_directories (${MCRL2_UTILITIES_LIBRARY})
include_directories (include ${MCRL2_UTILITIES_INCLUDE_DIR} ${YAMLCPP_INCLUDE_DIR} ${Boost_INCLUDE_DIRS})

add_executable (pginfo
  src/pginfo.cpp
)

target_link_libraries (pginfo ${MCRL2_UTILITIES_LIBRARY} ${YAMLCPP_LIBRARY} boost_regex)

add_subdirectory(test)
