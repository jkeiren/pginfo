# Unit tests
# For Ubuntu, instructions are loosely based on http://www.thebigblob.com/getting-started-with-google-test-on-ubuntu/

project(unittest)

enable_testing()
find_package(GoogleTest)

link_directories (${MCRL2_UTILITIES_LIBRARY})
include_directories (include ${MCRL2_UTILITIES_INCLUDE_DIR} ${YAMLCPP_INCLUDE_DIR} ${Boost_INCLUDE_DIRS} ${GTEST_INCLUDE_DIR})

add_executable (unittest
  unittest.cpp
)

target_link_libraries (unittest ${MCRL2_UTILITIES_LIBRARY} ${YAMLCPP_LIBRARY} boost_regex ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} pthread)
