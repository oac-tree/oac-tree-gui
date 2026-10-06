# Tests for oac-tree-gui-views library.

set(test testoac-tree-gui-views)

add_executable(${test})

target_sources(${test} PRIVATE
  custom_celleditor_factory_tests.cpp
  instruction_editor_actions_tests.cpp
  instruction_task_area_widget_tests.cpp
  instruction_task_monitor_tests.cpp
  instruction_task_widget_builder_tests.cpp
  integration_scenario_tests.cpp
  job_list_widget_tests.cpp
  main.cpp
  main_window_helper_tests.cpp
  monitor_widget_tests.cpp
  node_editor_widget_tests.cpp
  operation_monitor_view_tests.cpp
  operation_splittable_widget_tests.cpp
  operation_tab_widget_tests.cpp
  procedure_list_actions_test.cpp
  procedure_list_widget_tests.cpp
  procedure_splittable_editor_widget_tests.cpp
  sequencer_main_window_context_tests.cpp
  server_settings_widget_tests.cpp
  status_indicator_helper_tests.cpp
  task_widget_tests.cpp
  zoom_factor_converter_tests.cpp
)

target_link_libraries(${test} PRIVATE oac-tree-gui-views oac-tree-gui-test-utils)

set_target_properties(${test} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIRECTORY})

gtest_discover_tests(${test} DISCOVERY_MODE PRE_TEST TEST_PREFIX Views.)
