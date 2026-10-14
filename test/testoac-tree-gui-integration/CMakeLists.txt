# Complex integration tests for oac-tree-gui-components library (require QApplication).

set(test testoac-tree-gui-integration)

add_executable(${test})

target_sources(${test} PRIVATE
  job_manager_remote_scenario_tests.cpp
  main.cpp
  operation_action_handler_remote_scenario_tests.cpp
  remote_job_handler_tests.cpp
  test_automation_server.cpp
  test_automation_server.h
  workspace_synchronizer_pvaccess_tests.cpp
)

target_link_libraries(${test} PRIVATE oac-tree-gui-components oac-tree-gui-test-utils)

set_target_properties(${test} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIRECTORY})

# Integration tests run one at a time.
gtest_discover_tests(${test} DISCOVERY_TIMEOUT 10 DISCOVERY_MODE PRE_TEST TEST_PREFIX Integration.
  PROPERTIES RESOURCE_LOCK oac-tree-gui-integration)
