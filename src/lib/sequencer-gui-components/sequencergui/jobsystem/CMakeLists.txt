# Collection of classes to submit sequencer procedures as jobs running in a separate thread.

target_sources(${library_name} PRIVATE
  domain_event_dispatcher.cpp
  domain_event_dispatcher.h
  domain_event_queue.cpp
  domain_event_queue.h
  domain_events.cpp
  domain_events.h
  domain_job_observer.cpp
  domain_job_observer.h
  domain_procedure_observer.cpp
  domain_procedure_observer.h
  domain_runner.cpp
  domain_runner.h
  domain_runner_service.cpp
  domain_runner_service.h
  job_handler.cpp
  job_handler.h
  job_log.cpp
  job_log.h
  job_log_severity.cpp
  job_log_severity.h
  job_utils.cpp
  job_utils.h
  log_event.cpp
  log_event.h
  request_handler.h
  request_handler_queue.h
  request_types.h
  runner_status.cpp
  runner_status.h
  user_choice_provider.cpp
  user_choice_provider.h
  user_context.h
  user_input_provider.cpp
  user_input_provider.h
)
