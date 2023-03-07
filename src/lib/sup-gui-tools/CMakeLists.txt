set(library_name sup-gui-tools)

set(CMAKE_AUTOMOC ON)

add_library(${library_name} SHARED "")

add_subdirectory(sup)

target_link_libraries(${library_name} PUBLIC pthread sup-gui-core)

target_include_directories(${library_name} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}$<SEMICOLON>${SEQUENCERGUI_AUTOGEN_DIR}>)

install(TARGETS ${library_name} LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
