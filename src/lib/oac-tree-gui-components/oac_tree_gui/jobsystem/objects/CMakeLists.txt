# This folder contains QObject derived classes (they are excluded from the Sonar analysis)

target_sources(${library_name} PRIVATE
  abstract_job_handler.cpp
  abstract_job_handler.h
  domain_event_dispatcher.cpp
  domain_event_dispatcher.h
  domain_event_queue.cpp
  domain_event_queue.h
  job_log.cpp
  job_log.h
  job_manager.cpp
  job_manager.h
  local_job_handler.cpp
  local_job_handler.h
  remote_job_handler.cpp
  remote_job_handler.h
  user_choice_provider.cpp
  user_choice_provider.h
  user_input_provider.cpp
  user_input_provider.h
)
