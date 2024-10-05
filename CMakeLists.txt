cmake_minimum_required(VERSION 3.30.1)

project(ENUMFACTORY_TEST)


#set(CMAKE_BUILD_TYPE Release)
set(OSBitness 32)
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
   set(OSBitness 64)
endif()

set(FULL_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin/${CMAKE_SYSTEM_NAME}${OSBitness}/${CMAKE_BUILD_TYPE}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${FULL_OUTPUT_PATH}/static libs")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${FULL_OUTPUT_PATH}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${FULL_OUTPUT_PATH}")

include_directories(.)

add_library(enumfactory SHARED enumfactory.c)

add_executable(${PROJECT_NAME}  enumfactorytest.c )
target_link_libraries(${PROJECT_NAME} enumfactory)

#install( TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_SOURCE_DIR}/bin     )
