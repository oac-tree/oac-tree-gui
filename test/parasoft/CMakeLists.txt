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
  src-testoac-tree-gui-core=${CMAKE_SOURCE_DIR}/test/testoac-tree-gui-core
  src-testoac-tree-gui-components=${CMAKE_SOURCE_DIR}/test/testoac-tree-gui-components
  src-testoac-tree-gui-components-app=${CMAKE_SOURCE_DIR}/test/testoac-tree-gui-components-app
  src-testoac-tree-gui-views=${CMAKE_SOURCE_DIR}/test/testoac-tree-gui-views
  src-testoac-tree-gui-integration=${CMAKE_SOURCE_DIR}/test/testoac-tree-gui-integration
  TARGETS oac-tree-gui-core oac-tree-gui-components oac-tree-gui-views testoac-tree-gui-core testoac-tree-gui-components testoac-tree-gui-components-app testoac-tree-gui-views testoac-tree-gui-integration
)

get_target_property(SEQUENCER_GUI_CORE_INCLUDE_DIRECTORIES oac-tree-gui-core INCLUDE_DIRECTORIES)
get_target_property(SEQUENCER_GUI_COMPONENTS_INCLUDE_DIRECTORIES oac-tree-gui-components INCLUDE_DIRECTORIES)
get_target_property(SEQUENCER_GUI_VIEWS_INCLUDE_DIRECTORIES oac-tree-gui-views INCLUDE_DIRECTORIES)
target_include_directories(oac-tree-gui-cpptest PUBLIC ${SEQUENCER_GUI_CORE_INCLUDE_DIRECTORIES} ${SEQUENCER_GUI_COMPONENTS_INCLUDE_DIRECTORIES} ${SEQUENCER_GUI_VIEWS_INCLUDE_DIRECTORIES})

target_link_libraries(oac-tree-gui-cpptest
  PUBLIC
  sup-gui-core::core
  sup-gui-core::views
  sup-gui-core::components
  sup-mvvm::model
  sup-mvvm::view
  sup-mvvm::viewmodel
  oac-tree-gui-core
  oac-tree-gui-views
  oac-tree-gui-components
  oac-tree::oac-tree
  sup-epics::sup-epics
  oac-tree-server::oac-tree-server
)
