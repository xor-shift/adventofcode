cmake_minimum_required(VERSION 3.24)
project(adventofcode)

set(CMAKE_CXX_STANDARD 23)

set(AOC_YEAR 2022)
set(AOC_DAY 6)
set(AOC_DO_BIGBOY ON)
set(AOC_SINGLE_PART ${AOC_DO_BIGBOY})
set(AOC_DO_EXAMPLES OFF)

if (${AOC_DO_BIGBOY})
    set(AOC_INFILE ./Solutions/${AOC_YEAR}/${AOC_DAY}/bigboy)
else()
    set(AOC_INFILE ./Solutions/${AOC_YEAR}/${AOC_DAY}/input)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -lc++abi")

if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -mtune=native -march=native")
endif()

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug" OR ${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize=undefined")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address -fsanitize=undefined")
endif()

set(LibStuffCompileTests off)
set(LibStuffCompileBenchmarks off)
set(LibStuffCompileExamples off)
add_subdirectory(Thirdparty/LibStuff)
add_subdirectory(Thirdparty/range-v3)

add_executable(${PROJECT_NAME} main.cpp Solutions/${AOC_YEAR}/${AOC_DAY}/Solution.cpp)
target_link_libraries(${PROJECT_NAME} range-v3 libstuff)
target_include_directories(${PROJECT_NAME} PRIVATE Solutions)

target_compile_definitions(${PROJECT_NAME} PRIVATE AOC_YEAR=${AOC_YEAR})
target_compile_definitions(${PROJECT_NAME} PRIVATE AOC_DAY=${AOC_DAY})
target_compile_definitions(${PROJECT_NAME} PRIVATE INPUT_FILE="${AOC_INFILE}")
if(${AOC_DO_EXAMPLES})
    target_compile_definitions(${PROJECT_NAME} PRIVATE AOC_DO_EXAMPLES)
endif()
if(${AOC_SINGLE_PART})
    target_compile_definitions(${PROJECT_NAME} PRIVATE AOC_SINGLE_PART)
endif()
