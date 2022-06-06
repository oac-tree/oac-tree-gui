# Collection of classes to submit sequencer procedures as jobs running in a separate thread.

target_sources(${library_name} PRIVATE
  abstract_job.cpp
  abstract_job.h
  function_runner.cpp
  function_runner.h
  runner_interface.h
  domain_runner_adapter.cpp
  domain_runner_adapter.h
  )
