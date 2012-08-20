# Locate mCRL2 utilities
##
# This module defines
#  MCRL2_UTILITIES_FOUND, if false, do not try to link to yaml-cpp
#  MCRL2_UTILITIES_LIBRARY, where to find mcrl2-utilities
#  MCRL2_UTILITIES_INCLUDE_DIR, where to find mcrl2 utilities headers
#
# If mcrl2 is not installed in a standard path, you can use the MCRL2_DIR CMake variable
# to tell CMake where mCRL2 is.

# find the mCRL2 include directory
find_path(MCRL2_UTILITIES_INCLUDE_DIR mcrl2/utilities/logger.h
          PATHS ${MCRL2_DIR}/include/)

# find the mCRL2 utilities library
find_library(MCRL2_UTILITIES_LIBRARY
             mcrl2_utilities
                   ${MCRL2_DIR}/lib/mcrl2)

# handle the QUIETLY and REQUIRED arguments and set MCRL2_UTILITIES_FOUND to TRUE if all listed variables are TRUE
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(MCRL2 DEFAULT_MSG MCRL2_UTILITIES_INCLUDE_DIR MCRL2_UTILITIES_LIBRARY)
mark_as_advanced(MCRL2_UTILITIES_INCLUDE_DIR MCRL2_UTILITIES_LIBRARY)

