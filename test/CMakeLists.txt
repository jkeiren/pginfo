# Unit tests
# For Ubuntu, instructions are loosely based on
# http://www.thebigblob.com/getting-started-with-google-test-on-ubuntu/

project(unittest)

enable_testing()

# Trick to add header files to projects in IDEs like Visual Studio and XCode
FILE(GLOB_RECURSE TEST_HEADERS "*.h")
add_custom_target(${PROJECT_NAME}_headers
  SOURCES ${TEST_HEADERS}
)

file(GLOB TEST_SOURCES "*.cpp")

include_directories(
  include
  ${YAMLCPP_INCLUDE_DIR}
  ${CMAKE_SOURCE_DIR}/external/logging/include
  ${CMAKE_SOURCE_DIR}/external/commandline/include
  ${CMAKE_SOURCE_DIR}/external/googletest/include
)

add_executable (unittest
  ${TEST_SOURCES}
)

target_link_libraries(unittest ${YAMLCPP_LIBRARY} gtest cppcli cpplogging)
