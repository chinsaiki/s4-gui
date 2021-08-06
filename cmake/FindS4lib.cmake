# Try to find UUID library and include path.
# Once done this will define
#
# S4lib_FOUND
# S4lib_INCLUDE_DIR
# S4lib_LIBRARIES

set(INCLUDE_PREX fpga)
set(HEADER_FILE common/s4conf.h)
SET(S4lib_NAMES S4lib SQLiteCpp sqlite3 zlib zlibstatic zlibd zlibstaticd)

FIND_PATH(S4lib_INCLUDE_DIR ${HEADER_FILE}
        HINTS
        ${S4lib_ROOT}/include
        PATHS
        /usr/local/include
        /usr/include
        /usr/local/${INCLUDE_PREX}/include
        /usr/local/${INCLUDE_PREX}/include/${LIB_FILE}
        )
IF (S4lib_INCLUDE_DIR)
    message(STATUS "Found S4lib_INCLUDE_DIR = ${S4lib_INCLUDE_DIR}")
ELSE (S4lib_INCLUDE_DIR)
    message(STATUS "Not found S4lib_INCLUDE_DIR")
ENDIF (S4lib_INCLUDE_DIR)
foreach(S4lib ${S4lib_NAMES})
    FIND_LIBRARY(S4lib_LIBRARY_${S4lib}
            NAMES ${S4lib}
            HINTS
            PATHS
            /usr/lib
            /usr/local/lib
            /usr/local/${INCLUDE_PREX}/lib
            /usr/local/${INCLUDE_PREX}/lib/${LIB_FILE}
            )
    IF (S4lib_LIBRARY_${S4lib})
        list(APPEND S4lib_LIBRARIES "${S4lib_LIBRARY_${S4lib}}")
    endif()
endforeach()

IF (S4lib_LIBRARIES)
    message(STATUS "Found S4lib_LIBRARIES = ${S4lib_LIBRARIES}")
ELSE (S4lib_LIBRARIES)
    message(STATUS "Not found S4lib_LIBRARIES")
ENDIF (S4lib_LIBRARIES)

IF (S4lib_LIBRARIES AND S4lib_INCLUDE_DIR)
    SET(S4lib_FOUND "YES")
ELSE (S4lib_LIBRARIES AND S4lib_INCLUDE_DIR)
    SET(S4lib_FOUND "NO")
ENDIF (S4lib_LIBRARIES AND S4lib_INCLUDE_DIR)


IF (S4lib_FOUND)
    IF (NOT S4lib_FIND_QUIETLY)
        MESSAGE(STATUS "Found S4lib headers: ${S4lib_INCLUDE_DIR}")
        MESSAGE(STATUS "Found S4lib library: ${S4lib_LIBRARIES}")
    ENDIF (NOT S4lib_FIND_QUIETLY)
ELSE (S4lib_FOUND)
    IF (S4lib_FIND_REQUIRED)
        MESSAGE(FATAL_ERROR "Could not find S4lib library")
    ENDIF (S4lib_FIND_REQUIRED)
ENDIF (S4lib_FOUND)

# message(STATUS "S4lib_INCLUDE_DIR = ${S4lib_INCLUDE_DIR}")
# message(STATUS "S4lib_LIBRARY = ${S4lib_LIBRARY}")

# Handle the REQUIRED argument and set S4lib_FOUND
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(S4lib DEFAULT_MSG S4lib_LIBRARIES S4lib_INCLUDE_DIR)

mark_as_advanced(S4lib_INCLUDE_DIR)
mark_as_advanced(S4lib_LIBRARIES)

