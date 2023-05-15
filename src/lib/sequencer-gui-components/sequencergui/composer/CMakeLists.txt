# Contains source for SequencerComposerView (main widget for procedure editing).

target_sources(${library_name} PRIVATE
  aggregate_panel.cpp
  aggregate_panel.h
  composer_panel.cpp
  composer_panel.h
  composer_procedure_editor.cpp
  composer_procedure_editor.h
  instruction_editor_actions.cpp
  instruction_editor_actions.h
  instruction_editor_widget.cpp
  instruction_editor_widget.h
  instruction_item_panel.cpp
  instruction_item_panel.h
  sequencer_composer_view.cpp
  sequencer_composer_view.h
  sequencer_composer_actions.cpp
  sequencer_composer_actions.h
  workspace_editor_widget.cpp
  workspace_editor_widget.h
)

