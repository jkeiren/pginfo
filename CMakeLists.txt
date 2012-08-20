cmake_minimum_required(VERSION 2.6)
project (pginfo)

# Find google test framework
find_package(GTest)

# Trick to add header files to projects in IDEs like Visual Studio and XCode
FILE(GLOB HEADERS "include/*.h")
FILE(GLOB SUB_HEADERS "include/*/*.h")
add_custom_target(${PROJECT_NAME}_headers
  SOURCES ${HEADERS} ${SUB_HEADERS}
)

set (MCRL2_LIB "" CACHE PATH "Path to mCRL2 lib dir.")
set (MCRL2_INCLUDE "" CACHE PATH "Path to mCRL2 include dir.")
set (YAML_CPP_LIB "" CACHE PATH "Path to YAML-CPP lib dir.")
set (YAML_CPP_INCLUDE "" CACHE PATH "Path to YAML-CPP include dir.")

link_directories (${MCRL2_LIB})
include_directories (include ${MCRL2_INCLUDE})

add_library (libmcrl2_utilities STATIC IMPORTED)

add_executable (pginfo
  src/pginfo.cpp
)

target_link_libraries (pginfo mcrl2_utilities yaml-cpp boost_regex)
