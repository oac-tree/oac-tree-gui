set(executable_name sup-pvmonitor)

add_executable(${executable_name} main.cpp monitor_main_window.cpp)

target_link_libraries(${executable_name} sup-pvmonitor-core Qt${QT_VERSION_MAJOR}::Widgets)

install(TARGETS ${executable_name} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
