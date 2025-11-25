if(NOT COA_PARASOFT_INTEGRATION)
  return()
endif()

include(COASetupParasoft)

# Add parasoft project for sup-mvvm
# This generates the build time information needed by Parasoft for analysis
cpptest_add_executable(oac-tree-gui-cpptest
  CPPTEST_PROJECT_LOC ${CMAKE_CURRENT_BINARY_DIR}
  CPPTEST_PROJECT_FOLDERS
  src-oac-tree-gui-core=${CMAKE_SOURCE_DIR}/src/lib/oac-tree-gui-core
  src-oac-tree-gui-components=${CMAKE_SOURCE_DIR}/src/lib/oac-tree-gui-components
  src-oac-tree-gui-views=${CMAKE_SOURCE_DIR}/src/lib/oac-tree-gui-views
  TARGETS oac-tree-gui-core oac-tree-gui-components oac-tree-gui-views
)

get_target_property(SEQUENCER_GUI_CORE_INCLUDE_DIRECTORIES oac-tree-gui-core INCLUDE_DIRECTORIES)
get_target_property(SEQUENCER_GUI_COMPONENTS_INCLUDE_DIRECTORIES oac-tree-gui-components INCLUDE_DIRECTORIES)
get_target_property(SEQUENCER_GUI_VIEWS_INCLUDE_DIRECTORIES oac-tree-gui-views INCLUDE_DIRECTORIES)
target_include_directories(oac-tree-gui-cpptest PUBLIC ${SEQUENCER_GUI_CORE_INCLUDE_DIRECTORIES} ${SEQUENCER_GUI_COMPONENTS_INCLUDE_DIRECTORIES} ${SEQUENCER_GUI_VIEWS_INCLUDE_DIRECTORIES})

target_link_libraries(oac-tree-gui-cpptest
  PUBLIC
  sup-gui-core::views
  oac-tree::oac-tree
  sup-epics::sup-epics
  oac-tree-server::oac-tree-server
)
