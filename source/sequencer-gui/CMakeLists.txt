set(executable_name sequencer-gui)

if (SEQUENCERGUI_CODAC)
  add_definitions(-DSEQUENCERGUI_CODAC)
endif()

add_executable(${executable_name} main.cpp)

add_subdirectory(libsequencerguicore)

target_link_libraries(${executable_name} sequencerguicore Qt${QT_VERSION_MAJOR}::Widgets)
