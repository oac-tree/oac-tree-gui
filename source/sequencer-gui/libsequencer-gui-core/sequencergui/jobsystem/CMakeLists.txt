# Collection of classes to submit sequencer procedures as jobs running in a separate thread.

target_sources(${library_name} PRIVATE
  abstract_job.cpp
  abstract_job.h
  domain_runner_adapter.cpp
  domain_runner_adapter.h
  flow_controller.cpp
  flow_controller.h
  function_runner.cpp
  function_runner.h
  job_context.cpp
  job_context.h
  job_manager.cpp
  job_manager.h
  job_types.h
  job_utils.cpp
  job_utils.h
  job_log_severity.cpp
  job_log_severity.h
  procedure_runner.cpp
  procedure_runner.h
  runner_interface.h
  sequencer_observer.cpp
  sequencer_observer.h
  user_context.h
  user_controller.cpp
  user_controller.h
  )
