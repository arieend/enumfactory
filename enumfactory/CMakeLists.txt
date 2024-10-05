cmake_minimum_required(VERSION 3.30.1)

project(enumfactory VERSION 1.0.0)

set(LibType SHARED) # options are SHARED or STATIC 
#set(LibType STATIC)

include_directories(include)

set (SourceFiles 
    src/enumfactory.c
)

add_library(${PROJECT_NAME} ${LibType} ${SourceFiles} )

