# Contains source for SequencerComposerView (main widget for procedure editing).

target_sources(${library_name} PRIVATE
  attribute_editor_action_handler.cpp
  attribute_editor_action_handler.h
  attribute_editor_context.h
  CMakeLists.txt
  instruction_editor_action_handler.cpp
  instruction_editor_action_handler.h
  instruction_editor_context.h
  procedure_plugin_controller.cpp
  procedure_plugin_controller.h
)

