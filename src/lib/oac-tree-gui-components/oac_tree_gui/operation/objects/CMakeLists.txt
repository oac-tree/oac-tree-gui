# This folder contains QObject derived classes (they are excluded from the Sonar analysis)

target_sources(${library_name} PRIVATE
  breakpoint_model_delegate.cpp
  breakpoint_model_delegate.h
  instruction_tree_expand_controller.cpp
  instruction_tree_expand_controller.h
  operation_action_handler.cpp
  operation_action_handler.h
  workspace_view_component_provider.cpp
  workspace_view_component_provider.h
)
