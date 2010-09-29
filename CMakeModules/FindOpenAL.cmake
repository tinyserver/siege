FIND_PATH(OPENAL_INCLUDE_DIR AL)

FIND_LIBRARY(OPENAL_LIBRARY NAMES openal OpenAL32)
IF(NOT OPENAL_LIBRARY)
    MESSAGE(SEND_ERROR "OpenAL not found")
ENDIF(NOT OPENAL_LIBRARY)

IF(OPENAL_INCLUDE_DIR AND OPENAL_LIBRARY)
   SET(OPENAL_FOUND TRUE)
ENDIF(OPENAL_INCLUDE_DIR AND OPENAL_LIBRARY)

IF(OPENAL_FOUND)
   IF (NOT OpenAL_FIND_QUIETLY)
      MESSAGE(STATUS "Found OpenAL: ${OPENAL_LIBRARY}")
   ENDIF (NOT OpenAL_FIND_QUIETLY)
ELSE(OPENAL_FOUND)
   IF (OpenAL_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find OpenAL")
   ENDIF (OpenAL_FIND_REQUIRED)
ENDIF(OPENAL_FOUND)