# Widgets for real time operation views

target_sources(${library_name} PRIVATE
  breakpoint_controller.cpp
  breakpoint_controller.h
  breakpoint_helper.cpp
  breakpoint_helper.h
  breakpoint_model_delegate.cpp
  breakpoint_model_delegate.h
  breakpoint_types.h
  instruction_tree_expand_controller.cpp
  instruction_tree_expand_controller.h
  operation_action_context.h
  operation_action_handler.cpp
  operation_action_handler.h
  operation_action_helper.cpp
  operation_action_helper.h
  tooltip_helper.cpp
  tooltip_helper.h
  workspace_view_component_provider.cpp
  workspace_view_component_provider.h
)
