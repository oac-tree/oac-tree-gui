# Contains source for SequencerMonitorView (main window for running procedures).

target_sources(${library_name} PRIVATE
  job_context.cpp
  job_context.h
  job_list_widget.cpp
  job_list_widget.h
  job_log.cpp
  job_log.h
  job_manager.cpp
  job_manager.h
  job_property_widget.cpp
  job_property_widget.h
  job_utils.cpp
  job_utils.h
  message_panel.cpp
  message_panel.h
  monitor_panel.cpp
  monitor_panel.h
  monitor_realtime_toolbar.cpp
  monitor_realtime_toolbar.h
  monitor_realtime_widget.cpp
  monitor_realtime_widget.h
  monitor_workspace_toolbar.cpp
  monitor_workspace_toolbar.h
  monitor_workspace_widget.cpp
  monitor_workspace_widget.h
  procedure_runner.cpp
  procedure_runner.h
  sequencer_monitor_view.cpp
  sequencer_monitor_view.h
  sequencer_observer.cpp
  sequencer_observer.h
  user_context.h
  user_controller.cpp
  user_controller.h
  )
