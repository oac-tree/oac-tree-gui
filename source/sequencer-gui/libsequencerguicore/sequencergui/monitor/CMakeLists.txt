# Contains source for SequencerMonitorView (main window for running procedures).

target_sources(${library_name} PRIVATE
  job_list_widget.cpp
  job_list_widget.h
  job_log.cpp
  job_log.h
  job_manager.cpp
  job_manager.h
  job_property_widget.cpp
  job_property_widget.h
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
  sequencer_monitor_view.cpp
  sequencer_monitor_view.h
  user_context.h
  user_controller.cpp
  user_controller.h
  )
