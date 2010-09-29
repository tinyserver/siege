CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
OPTION(BUILD_MOD_OPENGL "Build the OpenGL module" ON)

IF(BUILD_MOD_OPENGL)
    PROJECT(OpenGL)
    ADD_DEPENDENCIES(modules ${PROJECT_NAME})

    FIND_PACKAGE(OpenGL REQUIRED)

    INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})
    ADD_LIBRARY(${PROJECT_NAME} MODULE main context draw surface texture viewport)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${OPENGL_LIBRARY})
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES PREFIX "${SG_FNAME_PREFIX}")
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}${SG_FNAME_DEBUG}")
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES ENABLE_EXPORTS ON)
ENDIF(BUILD_MOD_OPENGL)