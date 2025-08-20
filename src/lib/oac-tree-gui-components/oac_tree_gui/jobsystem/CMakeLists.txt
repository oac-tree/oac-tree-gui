# Collection of classes to submit oac-tree procedures as jobs running in a separate thread.

target_sources(${library_name} PRIVATE
  abstract_domain_runner.cpp
  abstract_domain_runner.h
  automation_client.cpp
  automation_client.h
  domain_event_helper.cpp
  domain_event_helper.h
  domain_events.cpp
  domain_events.h
  domain_job_observer.cpp
  domain_job_observer.h
  domain_job_service.cpp
  domain_job_service.h
  i_automation_client.h
  i_job_handler.h
  i_job_item_manager.h
  i_remote_connection_service.h
  job_log_severity.cpp
  job_log_severity.h
  job_utils.cpp
  job_utils.h
  local_domain_runner.cpp
  local_domain_runner.h
  log_event.cpp
  log_event.h
  remote_connection_info.h
  remote_connection_service.cpp
  remote_connection_service.h
  remote_domain_runner.cpp
  remote_domain_runner.h
  request_handler.h
  request_handler_queue.h
  request_types.cpp
  user_context.h
)

add_subdirectory(objects)

