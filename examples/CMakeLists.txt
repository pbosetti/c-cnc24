#   _____                           _           
#  | ____|_  ____ _ _ __ ___  _ __ | | ___  ___ 
#  |  _| \ \/ / _` | '_ ` _ \| '_ \| |/ _ \/ __|
#  | |___ >  < (_| | | | | | | |_) | |  __/\__ \
#  |_____/_/\_\__,_|_| |_| |_| .__/|_|\___||___/
#  
cmake_minimum_required(VERSION 3.10)
project("C-CNC tests" VERSION 0.0.1 LANGUAGES C)

#   ____       _   _   _                 
#  / ___|  ___| |_| |_(_)_ __   __ _ ___ 
#  \___ \ / _ \ __| __| | '_ \ / _` / __|
#   ___) |  __/ |_| |_| | | | | (_| \__ \
#  |____/ \___|\__|\__|_|_| |_|\__, |___/
#    
set(CMAKE_C_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)

# Set sources directory
set(SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)

# Lets include my inc directory to my project
include_directories(${CMAKE_CURRENT_LIST_DIR}/inc)

#   _____                    _       
#  |_   _|_ _ _ __ __ _  ___| |_ ___ 
#    | |/ _` | '__/ _` |/ _ \ __/ __|
#    | | (_| | | | (_| |  __/ |_\__ \
#    |_|\__,_|_|  \__, |\___|\__|___/
#                 |___/    
add_executable(0_hello ${SOURCE_DIR}/0_hello.c)
add_executable(1_types ${SOURCE_DIR}/1_types.c)
add_executable(2_file ${SOURCE_DIR}/2_file.c)
add_executable(3_include_guards ${SOURCE_DIR}/3_include_guards.c)
add_executable(4_struct_union ${SOURCE_DIR}/4_struct_union.c)
add_executable(5_inout_p ${SOURCE_DIR}/5_inout_p.c)