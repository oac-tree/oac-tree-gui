# Contains source for SequencerComposerView (main widget for procedure editing).

target_sources(${library_name} PRIVATE
  attribute_editor_action_handler.cpp
  attribute_editor_action_handler.h
  attribute_editor_context.h
  i_workspace_editor_action_handler.h
  instruction_copy_helper.cpp
  instruction_copy_helper.h
  instruction_editor_action_handler.cpp
  instruction_editor_action_handler.h
  instruction_editor_context.h
  procedure_list_context.h
  workspace_editor_action_handler.cpp
  workspace_editor_action_handler.h
  workspace_editor_context.h
)

add_subdirectory(objects)
