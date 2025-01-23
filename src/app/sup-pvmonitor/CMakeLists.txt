set(executable_name sup-pvmonitor)

add_executable(${executable_name} main.cpp)

target_link_libraries(${executable_name} oac-tree-gui-views Qt${QT_VERSION_MAJOR}::Widgets)

install(TARGETS ${executable_name} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
