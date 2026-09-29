# Settings machinery.

target_sources(${library_name} PRIVATE
  i_settings_manager.cpp
  i_settings_manager.h
  i_settings_storage.h
  in_memory_settings_storage.cpp
  in_memory_settings_storage.h
  scoped_callback.cpp
  scoped_callback.h
  settings_archive.cpp
  settings_archive.h
  settings_manager.cpp
  settings_manager.h
)
