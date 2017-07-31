# Copyright 2016 rapmocha

#### Configure meta data
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(OpenGLExercises)
set(VERSION_MAJOR 0)
set(VERSION_MINOR 0)

#### Set directory
set(CPPLINT /usr/local/mybin/cpplint.py)
set(SOURCES_DIR_REL src)
set(INCLUDE_DIR_REL include)
set(RESOURCES_DIR_REL resources)
set(SOURCES_DIR ${PROJECT_SOURCE_DIR}/${SOURCES_DIR_REL})
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/${INCLUDE_DIR_REL})
set(RESOURCES_DIR ${PROJECT_SOURCE_DIR}/${RESOURCES_DIR_REL})
set(EXCLUDE_CPPLINT_SOURCES)
message("[Project name]: ${PROJECT_NAME}")
message("[Project root dir]: ${PROJECT_SOURCE_DIR}")
message("[Build binary dir]: ${CMAKE_BINARY_DIR}")
message("[Include headers dir]: (project root)/${INCLUDE_DIR_REL}")
message("[Sources dir]: (project root)/${SOURCES_DIR_REL}")
message("[Resources dir]: (project root)/${RESOURCES_DIR_REL}")
message("[cpplint path]: ${CPPLINT}")

#### Symlink resources directory
if(NOT EXISTS ${RESOURCES_DIR_REL})
  execute_process(COMMAND ln -s ${RESOURCES_DIR})
  message("Symlinked resources to build dir: ${RESOURCES_DIR}")
endif()

#### Find source files
# GLOB_RECURSE:    ファイルを再帰的に探索する。
# INCLUDES:        ヘッダファイル
# SOURCES:         ソースファイル
# EXCLUDE_SOURCES: ビルドしないソースファイル
file(GLOB_RECURSE INCLUDES ${INCLUDE_DIR}/*.h)
file(GLOB_RECURSE SOURCES ${SOURCES_DIR}/*.cpp)
file(GLOB_RECURSE EXCLUDE_SOURCES foo.cpp)
file(GLOB_RECURSE RESOURCES ${RESOURCES_DIR}/*)
list(REMOVE_ITEM SOURCES "${EXCLUDE_SOURCES}")
add_executable(${PROJECT_NAME}
  ${SOURCES}
  ${RESOURCES}
  )

#### Find Package
find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
pkg_search_module(GLEW REQUIRED glew)

#### Specify include header directories
include_directories(${INCLUDE_DIR})

#### Specify Libraries
target_link_libraries(${PROJECT_NAME}
  ${OPENGL_LIBRARY}
  ${GLFW_LIBRARIES}
  ${GLEW_LIBRARIES}
  )

#### Set compiler option
# C++11拡張を有効にする。
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)

#### Other targets
# run
add_custom_target(run
  COMMAND ./${PROJECT_NAME}
  DEPENDS ${PROJECT_NAME}
  WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
  )

# cpplint
set(CPPLINT_SOURCE ${SOURCES} ${INCLUDES})
list(REMOVE_ITEM CPPLINT_SOURCE "${EXCLUDE_CPPLINT_SOURCES}")
add_custom_target(cpplint
  COMMAND ${CPPLINT} --root=${INCLUDE_DIR_REL} ${CPPLINT_SOURCE}
  WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
  )

