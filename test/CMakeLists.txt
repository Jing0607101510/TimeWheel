cmake_minimum_required(VERSION 3.4.1)

project(timer_test)
add_executable(${PROJECT_NAME} TimerTest.cpp)

target_link_libraries(${PROJECT_NAME} timer)

