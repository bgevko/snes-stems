cmake_minimum_required(VERSION 3.27.4)
set(CMAKE_CXX_STANDARD 17)

# Project name and directories
project(SNES_STEMS)
set(APP_NAME snes-spc)
set(ROOT ${PROJECT_SOURCE_DIR})
set(BUILD ${PROJECT_BINARY_DIR})
set(INCLUDE ${ROOT}/include)
set(SRC ${ROOT}/src)
set(LIB ${ROOT}/lib)
set(TOOLS ${ROOT}/tools)

# Libraries and imports
add_library(Utils ${TOOLS}/utils.cpp)
add_library(Wave_Writer ${TOOLS}/wave_writer.c)
add_library(SPC_Player ${SRC}/SPC_Player.cpp)
add_executable(${APP_NAME} ${SRC}/main.cpp)

# Include directories
include_directories(${INCLUDE})

# Link libraries

target_link_libraries(SPC_Player
  PRIVATE Utils
  PRIVATE Wave_Writer
  PRIVATE ${LIB}/libSNES_SPC.dylib
)

target_link_libraries(${APP_NAME}
  PRIVATE SPC_Player
)

# Copy spc folder to build directory
file(COPY ${ROOT}/spc DESTINATION ${BUILD})