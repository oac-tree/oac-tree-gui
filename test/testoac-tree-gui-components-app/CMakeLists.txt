# Tests for oac-tree-gui-components library, which require QApplication.

set(test testoac-tree-gui-components-app)

add_executable(${test})

target_sources(${test} PRIVATE
  domain_job_service_tests.cpp
  domain_workspace_listener_pvaccess_tests.cpp
  domain_workspace_listener_tests.cpp
  graphics_scene_component_provider_scene_tests.cpp
  graphics_scene_component_provider_tests.cpp
  instruction_tree_expand_controller_tests.cpp
  job_manager_extended_tests.cpp
  local_domain_runner_tests.cpp
  local_job_handler_tests.cpp
  main.cpp
  node_graphics_scene_tests.cpp
  operation_action_handler_extended_tests.cpp
  operation_action_handler_tests.cpp
  resource_folder_tests.cpp
  sequencer_align_utils_tests.cpp
  text_edit_controller_tests.cpp
  tooltip_helper_tests.cpp
  user_choice_provider_tests.cpp
  user_input_provider_tests.cpp
  workspace_synchronizer_tests.cpp
  workspace_view_component_provider_tests.cpp
)

target_link_libraries(${test} PRIVATE oac-tree-gui-components oac-tree-gui-test-utils sup-epics::sup-epics-test)

set_target_properties(${test} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIRECTORY})

gtest_discover_tests(${test} DISCOVERY_TIMEOUT 10 DISCOVERY_MODE PRE_TEST TEST_PREFIX Integration.)
