# Collection of sequencer-related components that are used in more than one window.

target_sources(${library_name} PRIVATE
  abstract_anyvalue_editor.h
  anyvalue_compact_scalar_editor.cpp
  anyvalue_compact_scalar_editor.h
  anyvalue_compact_tree_editor.cpp
  anyvalue_compact_tree_editor.h
  anyvalue_editor_dialog.cpp
  anyvalue_editor_dialog.h
  anyvalue_extended_editor.cpp
  anyvalue_extended_editor.h
  custom_meta_types.cpp
  custom_meta_types.h
  message_handler_factory.cpp
  message_handler_factory.h
  message_helper.cpp
  message_helper.h
  procedure_list_widget.cpp
  procedure_list_widget.h
)

