set(executable_name sequencer-gui)

add_subdirectory(sequencergui)

if (SEQUENCERGUI_CODAC)
  add_definitions(-DSEQUENCERGUI_CODAC)
endif()

add_executable(${executable_name} main.cpp)

target_link_libraries(${executable_name} sequencerguicore Qt5::Widgets)
target_include_directories(${executable_name} PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>)
