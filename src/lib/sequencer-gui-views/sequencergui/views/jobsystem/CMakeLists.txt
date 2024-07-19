# Collection of classes to submit sequencer procedures as jobs running in a separate thread.

target_sources(${library_name} PRIVATE
  job_manager.cpp
  job_manager.h
)
