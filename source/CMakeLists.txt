cmake_minimum_required(VERSION 3.14)
project(sequencer-gui VERSION 0.0.1 LANGUAGES CXX)

set(executable_name sequencer-gui)
add_subdirectory(sequencergui)
add_subdirectory(testsequencergui)

add_executable(${executable_name} main.cpp)

target_link_libraries(${executable_name} sequencerguicore Qt5::Widgets)
target_include_directories(${executable_name} PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>)
