cmake_minimum_required(VERSION 2.6)
project (pginfo)

# Include scripts
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/scripts")

# Target directories for the installation
if(NOT DEFINED ${PROJECT_NAME}_BIN_DIR)
  set(${PROJECT_NAME}_BIN_DIR "bin" CACHE FILEPATH "Installation directory for the binaries")
endif()
if(NOT DEFINED ${PROJECT_NAME}_LIB_DIR)
  set(${PROJECT_NAME}_LIB_DIR "lib" CACHE FILEPATH "Installation directory for the libraries")
endif()
if(NOT DEFINED ${PROJECT_NAME}_INCLUDE_DIR)
  set(${PROJECT_NAME}_INCLUDE_DIR "include/${PROJECT_NAME}" CACHE FILEPATH "Installation directory for the headers")
endif()

# Google test
if(NOT DEFINED ${PROJECT_NAME}_USE_INTERNAL_GOOGLETEST)
  set(${PROJECT_NAME}_USE_INTERNAL_GOOGLETEST ON CACHE BOOL "If set, the internal version of googletest is used, otherwise we rely on a version of googletest being included in a calling script")
endif()

# Logging library
if(NOT DEFINED ${PROJECT_NAME}_USE_INTERNAL_CPPLOGGING)
  set(${PROJECT_NAME}_USE_INTERNAL_CPPLOGGING ON CACHE BOOL "If set, the internal version of cpplogging is used.")
endif()

# CLI library
if(NOT DEFINED ${PROJECT_NAME}_USE_INTERNAL_CPPCLI)
  set(${PROJECT_NAME}_USE_INTERNAL_CPPCLI ON CACHE BOOL "If set, the internal version of cppcli is used.")
endif()

### Include directories for external projects, if required
if(${PROJECT_NAME}_USE_INTERNAL_GOOGLETEST)
  message(STATUS "${PROJECT_NAME}: Using Google test from external/googletest")
  add_subdirectory(external/googletest)
endif()

if(${PROJECT_NAME}_USE_INTERNAL_CPPLOGGING)
  message(STATUS "${PROJECT_NAME}: Using cpplogging from external/cpplogging")
  set(cpplogging_USE_INTERNAL_GOOGLETEST OFF)
  add_subdirectory(external/cpplogging)
else()
  set(cpplogging_LIB_DIR "" CACHE PATH "cpplogging library path")
  set(cpplogging_INCLUDE_DIR "" CACHE PATH "cpplogging header path")
endif()
link_directories(${cpplogging_LIB_DIR})

if(${PROJECT_NAME}_USE_INTERNAL_CPPCLI)
  message(STATUS "${PROJECT_NAME}: Using cppcli from external/cppcli")
  set(cppcli_USE_INTERNAL_GOOGLETEST OFF)
  if(${PROJECT_NAME}_USE_INTERNAL_CPPLOGGING)
    set(cppcli_USE_INTERNAL_CPPLOGGING OFF)
  endif()
  add_subdirectory(external/cppcli)
else()
  set(cppcli_LIB_DIR "" CACHE PATH "cppcli library path")
  set(cppcli_INCLUDE_DIR "" CACHE PATH "cppcli header path")
endif()
link_directories(${cppcli_LIB_DIR})

### If boost was not already included elsewhere, get it here
if(NOT Boost_INCLUDE_DIR)
  find_package(Boost REQUIRED COMPONENTS graph)
  message(STATUS "Using Boost headers from ${Boost_INCLUDE_DIR}")
endif()

### Search for YamlCpp; we depend on this for logging output.
find_package(YamlCpp REQUIRED)

### Trick to add header files to projects in IDEs like Visual Studio and XCode
file(GLOB_RECURSE ${PROJECT_NAME}_HEADERS "include/**.h")
file(GLOB_RECURSE ${PROJECT_NAME}_SOURCES "src/**.cpp")

add_custom_target(${PROJECT_NAME}_headers
  SOURCES ${${PROJECT_NAME}_HEADERS}
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Wall -Wextra")
set(CMAKE_CXX_FLAGS_ANALYSIS "${CMAKE_CXX_FLAGS} --analyse")

include_directories(
  include
  ${cpplogging_INCLUDE_DIR}
  ${cppcli_INCLUDE_DIR}
  ${YAMLCPP_INCLUDE_DIR}
  ${Boost_INCLUDE_DIR}
)

add_executable (${PROJECT_NAME}
  src/pginfo.cpp
)

target_link_libraries (${PROJECT_NAME} ${YAMLCPP_LIBRARY} cppcli cpplogging)

add_subdirectory(test)
