# Collection of classes to submit sequencer procedures as jobs running in a separate thread.

target_sources(${library_name} PRIVATE
  abstractjob.cpp
  abstractjob.h
  abstractjobv2.cpp
  abstractjobv2.h
  functionrunner.cpp
  functionrunner.h
  functionrunnerv2.cpp
  functionrunnerv2.h
  jobstates.cpp
  jobstates.h
  runnerinterface.h
  )
