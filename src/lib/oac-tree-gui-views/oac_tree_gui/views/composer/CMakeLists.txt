# Contains source for SequencerComposerView (main widget for procedure editing).

target_sources(${library_name} PRIVATE
  aggregate_panel.cpp
  aggregate_panel.h
  attribute_editor_actions.cpp
  attribute_editor_actions.h
  composer_panel.cpp
  composer_panel.h
  composer_widget_panel.cpp
  composer_widget_panel.h
  instruction_attribute_editor.cpp
  instruction_attribute_editor.h
  instruction_editor_actions.cpp
  instruction_editor_actions.h
  instruction_editor_widget.cpp
  instruction_editor_widget.h
  instruction_item_panel.cpp
  instruction_item_panel.h
  instruction_toolkit_tree_view.cpp
  instruction_toolkit_tree_view.h
  procedure_list_actions.cpp
  procedure_list_actions.h
  procedure_list_widget.cpp
  procedure_list_widget.h
  sequencer_composer_actions.cpp
  sequencer_composer_actions.h
  sequencer_composer_view.cpp
  sequencer_composer_view.h
  workspace_editor.cpp
  workspace_editor.h
  workspace_editor_actions.cpp
  workspace_editor_actions.h
  workspace_editor_widget.cpp
  workspace_editor_widget.h
  xml_panel.cpp
  xml_panel.h
)

