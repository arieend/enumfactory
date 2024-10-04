cmake_minimum_required(VERSION 3.30.3)

project(ENUMFACTORY_TEST)

add_executable(${PROJECT_NAME}  enumfactorytest.c )

set(CMAKE_BUILD_TYPE Release)
install( TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_SOURCE_DIR}/bin     )
