#---------------------------------------------------------------------------------------#

cmake_minimum_required(VERSION 3.15)

project(libmymalloc)
#---------------------------------------------------------------------------------------#

# Build malloc lib

include(build/lib_sources.cmake)

set (INC inc)

add_library(mymalloc SHARED ${SOURCES})

target_include_directories(mymalloc PUBLIC ${INC})

#---------------------------------------------------------------------------------------#

include(build/tests_sources.cmake)

set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH})

include(CTest)

message(${CMAKE_CURRENT_SOURCE_DIR})
foreach(TEST_FILE IN LISTS TEST_SOURCES)
	cmake_path(GET TEST_FILE STEM TEST_FILE_NO_EXT)
	add_executable(${TEST_FILE_NO_EXT} ${TEST_FILE})
	target_link_libraries(${TEST_FILE_NO_EXT} PUBLIC mymalloc)
	target_include_directories(${TEST_FILE_NO_EXT} PUBLIC ${INC}/utest.h/)
	target_include_directories(${TEST_FILE_NO_EXT} PUBLIC ${INC})
	add_test(NAME ${TEST_FILE_NO_EXT} COMMAND ${TEST_FILE_NO_EXT})
	set(TEST_LIST ${TEST_LIST} ${TEST_FILE})
endforeach()

enable_testing()

#---------------------------------------------------------------------------------------#
