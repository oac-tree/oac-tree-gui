if(NOT COA_PARASOFT_INTEGRATION)
  return()
endif()

include(COASetupParasoft)

# Add parasoft project for sup-mvvm
# This generates the build time information needed by Parasoft for analysis
cpptest_add_executable(sequencer-gui-cpptest
  CPPTEST_PROJECT_LOC ${CMAKE_CURRENT_BINARY_DIR}
  CPPTEST_PROJECT_FOLDERS
  src-sequencer-gui-core=${CMAKE_SOURCE_DIR}/src/lib/sequencer-gui-core
  src-sequencer-gui-components=${CMAKE_SOURCE_DIR}/src/lib/sequencer-gui-components
  src-sequencer-gui-views=${CMAKE_SOURCE_DIR}/src/lib/sequencer-gui-views
  src-testsequencer-gui-core=${CMAKE_SOURCE_DIR}/test/testsequencer-gui-core
  src-testsequencer-gui-components=${CMAKE_SOURCE_DIR}/test/testsequencer-gui-components
  src-testsequencer-gui-components-app=${CMAKE_SOURCE_DIR}/test/testsequencer-gui-components-app
  src-testsequencer-gui-views=${CMAKE_SOURCE_DIR}/test/testsequencer-gui-views
  src-testsequencer-gui-integration=${CMAKE_SOURCE_DIR}/test/testsequencer-gui-integration
  TARGETS sequencer-gui-core sequencer-gui-components sequencer-gui-views testsequencer-gui-core testsequencer-gui-components testsequencer-gui-components-app testsequencer-gui-views testsequencer-gui-integration
)

get_target_property(SEQUENCER_GUI_INCLUDE_DIRECTORIES sequencer-gui-views INCLUDE_DIRECTORIES)
target_include_directories(sequencer-gui-cpptest PUBLIC ${SEQUENCER_GUI_INCLUDE_DIRECTORIES})

target_link_libraries(sequencer-gui-cpptest
  PUBLIC
  sup-gui-core::views
  sequencer::sequencer
  sup-epics::sup-epics
  sup-auto-server::sup-auto-server
)
