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
  message_handler_factory.cpp
  message_handler_factory.h
  procedure_list_action_handler.cpp
  procedure_list_action_handler.h
  procedure_list_actions.cpp
  procedure_list_actions.h
  procedure_list_context.h
  procedure_list_widget.cpp
  procedure_list_widget.h
)

