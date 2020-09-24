cmake_minimum_required(VERSION 3.14)
project (nmly)

ADD_EXECUTABLE(nmly
    src/helper.c src/helper.h
    src/string.c src/string.h
    src/nmly.c src/nmly.h)

ADD_EXECUTABLE(nmly_test
        src/helper.c src/helper.h
        src/string.c src/string.h
        tests/main.c)
