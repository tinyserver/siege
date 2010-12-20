FIND_PATH(CPYTHON_INCLUDE_DIR Python.h PATH_SUFFIXES python2.6 python2.7 python3.0 python3.1)

FIND_LIBRARY(CPYTHON_LIBRARY NAMES python2.6 python2.7 python3.0 python3.1 PATH_SUFFIXES python2.6 python2.7 python3.0 python3.1)
IF(NOT CPYTHON_LIBRARY)
    MESSAGE(SEND_ERROR "CPython not found")
ENDIF(NOT CPYTHON_LIBRARY)

IF(CPYTHON_INCLUDE_DIR AND CPYTHON_LIBRARY)
   SET(CPYTHON_FOUND TRUE)
ENDIF(CPYTHON_INCLUDE_DIR AND CPYTHON_LIBRARY)

IF(CPYTHON_FOUND)
   IF (NOT CPython_FIND_QUIETLY)
	   MESSAGE(STATUS "Found CPython: ${CPYTHON_LIBRARY}")
   ENDIF (NOT CPython_FIND_QUIETLY)
ELSE(CPYTHON_FOUND)
   IF (CPython_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find CPython")
   ENDIF (CPython_FIND_REQUIRED)
ENDIF(CPYTHON_FOUND)
