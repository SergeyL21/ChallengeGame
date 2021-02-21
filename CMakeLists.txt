cmake_minimum_required(VERSION 3.5)

project(challenge_game LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_FILES
  main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
