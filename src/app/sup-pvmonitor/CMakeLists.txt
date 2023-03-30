set(executable_name sup-pvmonitor)

add_executable(${executable_name} main.cpp pvmonitor_main_window.cpp)

target_link_libraries(${executable_name} sequencer-gui-components Qt${QT_VERSION_MAJOR}::Widgets)

install(TARGETS ${executable_name} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
