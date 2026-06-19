# Tests for oac-tree-gui-components library.
set(test testoac-tree-gui-components)

find_package(sup-epics REQUIRED)

add_executable(${test})

target_sources(${test} PRIVATE
  align_node_tests.cpp
  align_utils_tests.cpp
  attribute_editor_action_handler_tests.cpp
  breakpoint_controller_tests.cpp
  breakpoint_helper_tests.cpp
  connectable_instruction_adapter_tests.cpp
  connectable_shape_factory_tests.cpp
  copy_and_paste_helper_tests.cpp
  custom_children_strategies_tests.cpp
  custom_presentation_item_tests.cpp
  custom_row_strategies_tests.cpp
  custom_viewitem_factory_tests.cpp
  domain_event_dispatcher_tests.cpp
  domain_event_helper_tests.cpp
  domain_event_queue_tests.cpp
  domain_events_tests.cpp
  domain_job_observer_tests.cpp
  drag_and_drop_helper_tests.cpp
  favorite_instructions_viewmodel_extended_tests.cpp
  favorite_instructions_viewmodel_tests.cpp
  flatlist_viewmodel_tests.cpp
  graphics_scene_action_handler_tests.cpp
  graphics_scene_style_tests.cpp
  graphics_view_style_tests.cpp
  instruction_editor_action_handler_copypaste_tests.cpp
  instruction_editor_action_handler_tests.cpp
  instruction_editor_action_handler_undo_tests.cpp
  instruction_editor_drop_handler_tests.cpp
  instruction_editor_viewmodel_tests.cpp
  instruction_operation_viewmodel_tests.cpp
  instruction_toolkit_viewmodel_tests.cpp
  job_list_viewmodel_tests.cpp
  job_log_severity_tests.cpp
  job_log_tests.cpp
  job_log_viewmodel_tests.cpp
  job_manager_tests.cpp
  job_utils_tests.cpp
  json_style_factory_tests.cpp
  log_event_tests.cpp
  main.cpp
  monitor_model_tests.cpp
  operation_action_helper_tests.cpp
  procedure_list_action_handler_tests.cpp
  procedure_list_viewmodel_tests.cpp
  remote_connection_service_tests.cpp
  request_handler_queue_tests.cpp
  request_handler_tests.cpp
  request_types_tests.cpp
  scene_utils_tests.cpp
  sequencer_workspace_corner_case_tests.cpp
  style_helper_tests.cpp
  workspace_editor_action_handler_copyandpaste_tests.cpp
  workspace_editor_action_handler_tests.cpp
  workspace_editor_action_handler_undo_tests.cpp
  workspace_editor_viewmodel_tests.cpp
  workspace_filter_viewmodel_tests.cpp
  workspace_item_listener_tests.cpp
  workspace_monitor_helper_tests.cpp
  workspace_operation_viewmodel_tests.cpp
  xml_panel_controller_extended_tests.cpp
  xml_panel_controller_tests.cpp
)

target_link_libraries(${test} PRIVATE oac-tree-gui-components oac-tree-gui-test-utils PRIVATE sup-epics::sup-epics-test)

set_target_properties(${test} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIRECTORY})

gtest_discover_tests(${test} DISCOVERY_MODE PRE_TEST TEST_PREFIX Components.)
