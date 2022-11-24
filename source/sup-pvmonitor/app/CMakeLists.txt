set(executable_name sup-pvmonitor)

add_executable(${executable_name} main.cpp main_window.cpp)

target_link_libraries(${executable_name} sup-pvmonitor-core Qt${QT_VERSION_MAJOR}::Widgets)
