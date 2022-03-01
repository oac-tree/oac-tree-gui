set(executable_name anyvalue-editor)

add_executable(${executable_name} main.cpp)

add_subdirectory(anyvalueeditor)

target_link_libraries(${executable_name} anyvalueeditorcore Qt5::Widgets)
