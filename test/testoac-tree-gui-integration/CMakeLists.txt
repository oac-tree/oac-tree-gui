# Tests for oac-tree-gui-components library, which require QApplication.

set(test testoac-tree-gui-integration)

add_executable(${test})

target_sources(${test} PRIVATE
  domain_workspace_listener_softioc_tests.cpp
  global_ioc_environment.cpp
  job_manager_remote_scenario_tests.cpp
  main.cpp
  operation_action_handler_remote_scenario_tests.cpp
  remote_job_handler_tests.cpp
  test_automation_server.cpp
  test_automation_server.h
  workspace_synchronizer_pvaccess_tests.cpp
  workspace_synchronizer_softioc_tests.cpp
)

target_link_libraries(${test} PRIVATE oac-tree-gui-components oac-tree-gui-test-utils sup-epics::sup-epics-test)

set_target_properties(${test} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIRECTORY})

gtest_discover_tests(${test} DISCOVERY_TIMEOUT 10 DISCOVERY_MODE PRE_TEST TEST_PREFIX Integration.)
