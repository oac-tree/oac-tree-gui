set(library_name oac-tree-gui-views)

set(CMAKE_AUTOMOC ON)

add_library(${library_name} SHARED)

set_target_properties(${library_name} PROPERTIES
  VERSION ${LIBVERSION}
  SOVERSION ${LIBSOVERSION}
)

target_link_libraries(${library_name} PUBLIC pthread oac-tree-gui-components sup-gui-core::views)

add_subdirectory(oac_tree_gui)

target_include_directories(${library_name} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}$<SEMICOLON>${SEQUENCERGUI_AUTOGEN_DIR}>)

install(TARGETS ${library_name} LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
