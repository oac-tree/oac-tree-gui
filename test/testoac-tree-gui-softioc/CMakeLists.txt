# Tests of oac-tree-gui-components library against a softIoc started in the global environment
# (require QApplication).

set(test testoac-tree-gui-softioc)

add_executable(${test})

target_sources(${test} PRIVATE
  domain_workspace_listener_softioc_tests.cpp
  global_ioc_environment.cpp
  main.cpp
  workspace_synchronizer_softioc_tests.cpp
)

target_link_libraries(${test} PRIVATE oac-tree-gui-components oac-tree-gui-test-utils sup-epics::sup-epics-test)

set_target_properties(${test} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIRECTORY})

# SoftIoc tests run one at a time.
gtest_discover_tests(${test} DISCOVERY_TIMEOUT 10 DISCOVERY_MODE PRE_TEST TEST_PREFIX SoftIoc.
  PROPERTIES RESOURCE_LOCK oac-tree-gui-softioc)
