set(library_name sup-pvmonitor-core)

set(CMAKE_AUTOMOC ON)

add_library(${library_name} SHARED)

target_link_libraries(${library_name} PUBLIC pthread sequencer-gui-core sup-gui-core::tools)

add_subdirectory(sequencergui)

target_include_directories(${library_name} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}$<SEMICOLON>${SEQUENCERGUI_AUTOGEN_DIR}>)

install(TARGETS ${library_name} LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
