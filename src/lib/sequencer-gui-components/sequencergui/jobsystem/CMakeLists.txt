# Collection of classes to submit sequencer procedures as jobs running in a separate thread.

target_sources(${library_name} PRIVATE
  abstract_job.cpp
  abstract_job.h
  domain_runner_adapter.cpp
  domain_runner_adapter.h
  domain_runner_adapter_new.cpp
  domain_runner_adapter_new.h
  flow_controller.cpp
  flow_controller.h
  function_runner.cpp
  function_runner.h
  job_handler.cpp
  job_handler.h
  job_log_severity.cpp
  job_log_severity.h
  job_manager.cpp
  job_manager.h
  job_types.h
  job_utils.cpp
  job_utils.h
  log_event.cpp
  log_event.h
  procedure_reporter.cpp
  procedure_reporter.h
  request_handler.h
  request_handler_queue.h
  request_types.h
  runner_interface.h
  sequencer_observer.cpp
  sequencer_observer.h
  user_choice_provider.cpp
  user_choice_provider.h
  user_context.h
  user_input_provider.cpp
  user_input_provider.h
)
