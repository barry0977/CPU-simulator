cmake_minimum_required(VERSION 3.22)
project(code)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(code main.cpp
        memory.h
        registerfile.h
        reorderbuffer.h
        reservationstation.h
        decode.h
        alu.h
        cpu.h
        myqueue.h
        instructionqueue.h
        bus.h
)