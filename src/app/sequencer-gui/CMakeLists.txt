set(executable_name sequencer-gui)

add_executable(${executable_name} main.cpp)

target_link_libraries(${executable_name} sequencer-gui-components Qt${QT_VERSION_MAJOR}::Widgets)

install(TARGETS ${executable_name} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
