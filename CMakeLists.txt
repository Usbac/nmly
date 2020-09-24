cmake_minimum_required(VERSION 3.14)
project (nmly)

#SET SOURCE FILES
SET(PROJECT_SOURCE_FILES
	src/nmly.c
	src/helper.c
    src/string.c)

#SET HEADERS FILES
SET(PROJECT_HEADER_FILES
	src/nmly.h
	src/helper.h
    src/string.h)

ADD_EXECUTABLE(nmly ${PROJECT_SOURCE_FILES} ${PROJECT_HEADER_FILES})

