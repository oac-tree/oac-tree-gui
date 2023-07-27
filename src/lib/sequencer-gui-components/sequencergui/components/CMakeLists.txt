# Collection of sequencer-related components that are used in more than one window.

target_sources(${library_name} PRIVATE
  custom_meta_types.cpp
  custom_meta_types.h
  message_handler_factory.cpp
  message_handler_factory.h
  message_helper.cpp
  message_helper.h
  procedure_list_widget.cpp
  procedure_list_widget.h
  universal_property_editor.cpp
  universal_property_editor.h
  visibility_agent_base.cpp
  visibility_agent_base.h
)

