# Tests for oac-tree-gui-core library.

set(test testoac-tree-gui-core)

add_executable(${test})

target_sources(${test} PRIVATE
  anyvalue_item_transform_helper_tests.cpp
  application_models_tests.cpp
  attribute_item_transform_helper_tests.cpp
  component_types_tests.cpp
  composer_settings_item_tests.cpp
  domain_automation_helper_tests.cpp
  domain_constants_tests.cpp
  domain_helper_tests.cpp
  domain_library_loader_tests.cpp
  domain_object_group_helper_tests.cpp
  domain_procedure_builder_tests.cpp
  domain_workspace_builder_tests.cpp
  epics_instruction_items_tests.cpp
  exceptions_tests.cpp
  factory_tests.cpp
  instruction_container_item_tests.cpp
  instruction_info_item_tests.cpp
  instruction_item_tests.cpp
  instruction_item_transform_helper_tests.cpp
  instruction_status_tests.cpp
  item_factory_tests.cpp
  iterate_helper_tests.cpp
  job_item_tests.cpp
  job_model_tests.cpp
  main.cpp
  plugin_settings_item_tests.cpp
  procedure_item_job_info_builder_tests.cpp
  procedure_item_tests.cpp
  procedure_item_transform_helper_tests.cpp
  procedure_preamble_items_tests.cpp
  project_model_tests.cpp
  runner_status_tests.cpp
  sequencer_item_helper_tests.cpp
  sequencer_model_tests.cpp
  sequencer_settings_helper_tests.cpp
  sequencer_settings_model_tests.cpp
  standard_instruction_items_tests.cpp
  standard_job_items_tests.cpp
  standard_variable_items_tests.cpp
  text_edit_item_tests.cpp
  transform_from_domain_tests.cpp
  universal_instruction_item_tests.cpp
  universal_item_helper_tests.cpp
  universal_variable_item_tests.cpp
  variable_info_item_tests.cpp
  variable_item_tests.cpp
  variable_item_transform_helper_tests.cpp
  version_tests.cpp
  workspace_item_tests.cpp
  xml_utils_tests.cpp
)

target_link_libraries(${test} PRIVATE oac-tree-gui-core oac-tree-gui-test-utils)
set_target_properties(${test} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIRECTORY})

gtest_discover_tests(${test} DISCOVERY_TIMEOUT 5 DISCOVERY_MODE PRE_TEST TEST_PREFIX Core.)

# add dummy shared library for testing purposes
add_subdirectory(dummy_lib)
