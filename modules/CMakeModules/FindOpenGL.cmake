FIND_PATH(OPENGL_INCLUDE_DIR GL)

FIND_LIBRARY(OPENGL_LIBRARY NAMES GL OpenGL32)
IF(NOT OPENGL_LIBRARY)
    MESSAGE(SEND_ERROR "OpenGL not found")
ENDIF(NOT OPENGL_LIBRARY)

IF(OPENGL_INCLUDE_DIR AND OPENGL_LIBRARY)
   SET(OPENGL_FOUND TRUE)
ENDIF(OPENGL_INCLUDE_DIR AND OPENGL_LIBRARY)

IF(OPENGL_FOUND)
   IF (NOT OpenGL_FIND_QUIETLY)
      MESSAGE(STATUS "Found OpenGL: ${OPENGL_LIBRARY}")
   ENDIF (NOT OpenGL_FIND_QUIETLY)
ELSE(OPENGL_FOUND)
   IF (OpenGL_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find OpenGL")
   ENDIF (OpenGL_FIND_REQUIRED)
ENDIF(OPENGL_FOUND)
