# Collection of classes to submit sequencer procedures as jobs running in a separate thread.

target_sources(${library_name} PRIVATE
  abstractjob.cpp
  abstractjob.h
  abstractrunner.cpp
  abstractrunner.h
  jobstates.cpp
  jobstates.h
  runnerinterface.h
  )
