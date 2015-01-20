cmake_minimum_required(VERSION 2.8)
project (pginfo)

# Include scripts
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/scripts")

### Global compiler options
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Wall -Wextra")
set(CMAKE_CXX_FLAGS_ANALYSIS "${CMAKE_CXX_FLAGS} --analyse")

### Include google test externally, but only if it has not yet been included
if(NOT TARGET gtest)
  add_subdirectory(external/googletest)
endif()

### Find logging library
if(NOT CPPLOGGING_INCLUDE_DIR)
  find_package(cpplogging)
  if(NOT CPPLOGGING_FOUND)
    message(STATUS "Using built-in cpplogging")
    add_subdirectory(external/cpplogging)
    SET(CPPLOGGING_INCLUDE_DIR external/cpplogging/include)
    SET(CPPLOGGING_LIBRARY cpplogging)
  endif()
endif()
include_directories(${CPPLOGGING_INCLUDE_DIR})

### Find command line interface library
if(NOT CPPCLI_INCLUDE_DIR)
  find_package(cppcli)
  if(NOT CPPCLI_FOUND)
    message(STATUS "Using built-in cppcli")
    add_subdirectory(external/cppcli)
    SET(CPPCLI_INCLUDE_DIR external/cppcli/include)
    SET(CPPCLI_LIBRARY cppcli)
  endif()
endif()
include_directories(${CPPCLI_INCLUDE_DIR})

### If boost was not already included elsewhere, get it here
if(NOT Boost_INCLUDE_DIR)
  find_package(Boost REQUIRED COMPONENTS graph)
  message(STATUS "Using Boost headers from ${Boost_INCLUDE_DIR}")
endif()
include_directories(${Boost_INCLUDE_DIR})

### Search for YamlCpp; we depend on this for output.
find_package(YamlCpp REQUIRED)
include_directories(${YAMLCPP_INCLUDE_DIR})

### Trick to add header files to projects in IDEs like Visual Studio and XCode
file(GLOB_RECURSE ${PROJECT_NAME}_HEADERS "include/**.h")
file(GLOB_RECURSE ${PROJECT_NAME}_SOURCES "src/**.cpp")

add_custom_target(${PROJECT_NAME}_headers
  SOURCES ${${PROJECT_NAME}_HEADERS}
)

message(STATUS "CPPCLI_INCLUDE_DIR:     ${CPPCLI_INCLUDE_DIR}")
message(STATUS "CPPLOGGING_INCLUDE_DIR: ${CPPLOGGING_INCLUDE_DIR}")
message(STATUS "YAMLCPP_INCLUDE_DIR:    ${YAMLCPP_INCLUDE_DIR}")
message(STATUS "YAMLCCP_LIBRARY:        ${YAMLCPP_LIBRARY}")
message(STATUS "Boost_INCLUDE_DIR:      ${Boost_INCLUDE_DIR}")

include_directories(include)
add_executable (${PROJECT_NAME} src/pginfo.cpp)
target_link_libraries (${PROJECT_NAME} ${YAMLCPP_LIBRARY} ${CPPCLI_LIBRARY} ${CPPLOGGING_LIBRARY})
install (TARGETS ${PROJECT_NAME} DESTINATION bin)

add_subdirectory(test)
