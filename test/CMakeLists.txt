# Unit tests
# For Ubuntu, instructions are loosely based on
# http://www.thebigblob.com/getting-started-with-google-test-on-ubuntu/

project(unittest)

enable_testing()

file(GLOB TEST_SOURCES "*.cpp")

include_directories(
  include
  ${YAMLCPP_INCLUDE_DIR}
  ${CMAKE_SOURCE_DIR}/external/logging/include
  ${CMAKE_SOURCE_DIR}/external/commandline/include
  ${CMAKE_SOURCE_DIR}/googletest/include
)

add_executable (unittest
  ${TEST_SOURCES}
)

target_link_libraries(unittest ${YAMLCPP_LIBRARY} gtest cppcli cpplogging)
