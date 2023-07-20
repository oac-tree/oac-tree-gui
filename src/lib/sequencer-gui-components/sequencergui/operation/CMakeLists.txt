# Widgets for sequencer-operation standalone application

target_sources(${library_name} PRIVATE
  breakpoint_controller.cpp
  breakpoint_controller.h
  breakpoint_helper.cpp
  breakpoint_helper.h
  breakpoint_model_delegate.cpp
  breakpoint_model_delegate.h
  operation_job_panel.cpp
  operation_job_panel.h
  operation_job_panel_actions.cpp
  operation_job_panel_actions.h
  operation_realtime_panel.cpp
  operation_realtime_panel.h
  operation_workspace_panel.cpp
  operation_workspace_panel.h
  procedure_action_handler.cpp
  procedure_action_handler.h
)
