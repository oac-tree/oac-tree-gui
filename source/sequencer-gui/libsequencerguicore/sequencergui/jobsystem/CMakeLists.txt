# Collection of classes to submit sequencer procedures as jobs running in a separate thread.

target_sources(${library_name} PRIVATE
  abstractjob.cpp
  abstractjob.h
  functionrunnerv2.cpp
  functionrunnerv2.h
  runnerinterface.h
  )
