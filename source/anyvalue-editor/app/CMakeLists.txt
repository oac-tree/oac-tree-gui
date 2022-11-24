set(executable_name anyvalue-editor)

add_executable(${executable_name} main.cpp main_window.cpp)

target_link_libraries(${executable_name} anyvalue-editor-core Qt${QT_VERSION_MAJOR}::Widgets)
