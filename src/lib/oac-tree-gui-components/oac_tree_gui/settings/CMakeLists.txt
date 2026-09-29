# Settings machinery.

target_sources(${library_name} PRIVATE
  i_settings_storage.h
  in_memory_settings_storage.cpp
  in_memory_settings_storage.h
  settings_archive.cpp
  settings_archive.h
)
