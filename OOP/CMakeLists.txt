cmake_minimum_required (VERSION 3.2) 
project (oop_exercise_01) 
add_executable(main oop_exercise_01.cpp)
set_target_properties(main PROPERTIES 
CXX_STANDARD 14 
CXX_STANDARD_REQUIRED ON )