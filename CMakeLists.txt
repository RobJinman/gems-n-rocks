cmake_minimum_required(VERSION 2.8)

project(gems)

find_package(GLUT REQUIRED)
find_package(SDL_mixer REQUIRED)

include_directories(include)
include_directories(${GLUT_INCLUDE_DIRS})
include_directories(${SDL_MIXER_INCLUDE_DIRS})

link_directories(${GLUT_LIBRARY_DIRS})
link_directories(${SDL_MIXER_LIBRARY_DIRS})

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -DDEBUG")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif()

add_compile_options(${GLUT_CFLAGS})
add_compile_options(${SDL_MIXER_CFLAGS})

set(CMAKE_EXE_LINKER_FLAGS ${GLUT_LDFLAGS} ${SDL_MIXER_LDFLAGS})

file(GLOB_RECURSE SRCS src/*.cpp)
add_executable(gems ${SRCS})

message("${SDL_MIXER_LIBRARIES}")
target_link_libraries(gems ${GLUT_LIBRARIES} ${SDL_MIXER_LIBRARIES} -lGL -lSDL)

file(COPY data DESTINATION ${CMAKE_BINARY_DIR})

