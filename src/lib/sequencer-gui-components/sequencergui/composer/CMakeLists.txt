# Contains source for SequencerComposerView (main widget for procedure editing).

target_sources(${library_name} PRIVATE
  aggregate_panel.cpp
  aggregate_panel.h
  composer_panel.cpp
  composer_panel.h
  composer_widget_panel.cpp
  composer_widget_panel.h
  instruction_editor_action_handler.cpp
  instruction_editor_action_handler.h
  instruction_editor_actions.cpp
  instruction_editor_actions.h
  instruction_editor_widget.cpp
  instruction_editor_widget.h
  instruction_item_panel.cpp
  instruction_item_panel.h
  sequencer_composer_actions.cpp
  sequencer_composer_actions.h
  sequencer_composer_view.cpp
  sequencer_composer_view.h
  workspace_editor_widget.cpp
  workspace_editor_widget.h
  xml_panel.cpp
  xml_panel.h
)

