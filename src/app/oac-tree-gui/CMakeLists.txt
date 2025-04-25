set(executable_name oac-tree-gui)

add_executable(${executable_name} main.cpp)

target_link_libraries(${executable_name} oac-tree-gui-views Qt${QT_VERSION_MAJOR}::Widgets)
set_target_properties(${executable_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${SEQUENCERGUI_APP_RUNTIME_DIR})

install(TARGETS ${executable_name} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
