set(executable_name anyvalue-editor)

add_executable(${executable_name} main.cpp main_window.cpp)

target_link_libraries(${executable_name} sup-gui-tools Qt${QT_VERSION_MAJOR}::Widgets)

install(TARGETS ${executable_name} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
