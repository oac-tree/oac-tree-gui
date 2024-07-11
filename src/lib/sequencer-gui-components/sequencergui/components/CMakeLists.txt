# Collection of sequencer-related components that are used in more than one window.

target_sources(${library_name} PRIVATE
  app_constants.h
  app_settings.cpp
  app_settings.h
  anyvalue_compact_scalar_editor.cpp
  anyvalue_compact_scalar_editor.h
  anyvalue_compact_tree_editor.cpp
  anyvalue_compact_tree_editor.h
  anyvalue_editor_dialog_factory.cpp
  anyvalue_editor_dialog_factory.h
  custom_meta_types.cpp
  custom_meta_types.h
)

