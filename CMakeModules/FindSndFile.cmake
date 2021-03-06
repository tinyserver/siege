FIND_PATH(SNDFILE_INCLUDE_DIR sndfile.h)

FIND_LIBRARY(SNDFILE_LIBRARY NAMES sndfile-1 sndfile)
#IF(NOT SNDFILE_LIBRARY)
#    MESSAGE(SEND_ERROR "SndFile not found")
#ENDIF(NOT SNDFILE_LIBRARY)

IF(SNDFILE_INCLUDE_DIR AND SNDFILE_LIBRARY)
   SET(SNDFILE_FOUND TRUE)
ENDIF(SNDFILE_INCLUDE_DIR AND SNDFILE_LIBRARY)

IF(SNDFILE_FOUND)
   IF (NOT SndFile_FIND_QUIETLY)
      MESSAGE(STATUS "Found SndFile: ${SNDFILE_LIBRARY}")
   ENDIF (NOT SndFile_FIND_QUIETLY)
ELSE(SNDFILE_FOUND)
   IF (SndFile_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find SndFile")
   ENDIF (SndFile_FIND_REQUIRED)
ENDIF(SNDFILE_FOUND)
