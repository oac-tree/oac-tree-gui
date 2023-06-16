# Contains source for SequencerMonitorView, a third main view of the MainWindow.
# It is intended for real time procedure execution.

target_sources(${library_name} PRIVATE
  job_list_widget.cpp
  job_list_widget.h
  job_log.cpp
  job_log.h
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
  realtime_instruction_tree_widget.cpp
  realtime_instruction_tree_widget.h
  sequencer_monitor_actions.cpp
  sequencer_monitor_actions.h
  sequencer_monitor_view.cpp
  sequencer_monitor_view.h
  )
