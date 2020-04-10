cmake_minimum_required(VERSION 3.5)

project(smap LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(
    #-Wall
)
file(GLOB HEADERS "include/*.hpp")
file(GLOB SOURCES  "src/*.cpp")
find_package(Boost 1.67.0 REQUIRED COMPONENTS system regex thread program_options)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    include_directories(${PROJECT_SOURCE_DIR})
    add_executable(Smap ${SOURCES} ${HEADERS})
    target_link_libraries(Smap ${Boost_LIBRARIES})
endif()
