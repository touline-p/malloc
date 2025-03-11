#---------------------------------------------------------------------------------------#

cmake_minimum_required(VERSION 3.15)

project(libmymalloc)
#---------------------------------------------------------------------------------------#

set (INC inc)

set (LIBFT_EXTENDED libft_extended)
set (LIB_MALLOC malloc)

# Build libft lib

include(build/lib_sources.cmake)
include(build/libft_sources.cmake)
include(build/printf_sources.cmake)

message("this is fine i promise\n" ${LIBFT_SOURCES} "\n")

set(ALL_SOURCES ${SOURCES} ${LIBFT_SOURCES} ${PRINTF_SOURCES})

message(${ALL_SOURCES})

add_library(${LIB_MALLOC} SHARED ${ALL_SOURCES})
target_include_directories(${LIB_MALLOC} PUBLIC ${INC})
target_compile_options(${LIB_MALLOC} PRIVATE -g3)

#---------------------------------------------------------------------------------------#

include(build/tests_sources.cmake)

set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH})

include(CTest)

message(${CMAKE_CURRENT_SOURCE_DIR})
foreach(TEST_FILE IN LISTS TEST_SOURCES)
	cmake_path(GET TEST_FILE STEM TEST_FILE_NO_EXT)
	add_executable(${TEST_FILE_NO_EXT} ${TEST_FILE})
	target_link_libraries(${TEST_FILE_NO_EXT} PUBLIC ${LIB_MALLOC})
	target_include_directories(${TEST_FILE_NO_EXT} PUBLIC ${INC}/utest.h/)
	target_include_directories(${TEST_FILE_NO_EXT} PUBLIC ${INC})
	add_test(NAME ${TEST_FILE_NO_EXT} COMMAND ${TEST_FILE_NO_EXT})
	set(TEST_LIST ${TEST_LIST} ${TEST_FILE})
endforeach()

enable_testing()

#---------------------------------------------------------------------------------------#
