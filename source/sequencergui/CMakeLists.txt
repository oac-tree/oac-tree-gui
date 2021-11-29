set(library_name sequencerguicore)

set(CMAKE_AUTOMOC ON)

add_library(${library_name} STATIC "")
target_link_libraries(${library_name} PUBLIC pthread mvvm_view sequencer-shared)

add_subdirectory(composer)
add_subdirectory(mainwindow)
add_subdirectory(model)
add_subdirectory(monitor)
add_subdirectory(nodeeditor)
add_subdirectory(resources)

target_include_directories(${library_name} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/..)

