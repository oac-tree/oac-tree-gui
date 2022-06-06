# Contains source for SequencerComposerView (main widget for procedure editing).

target_sources(${library_name} PRIVATE
  aggregate_panel.cpp
  aggregate_panel.h
  composer_actions.cpp
  composer_actions.h
  composer_panel.cpp
  composer_panel.h
  composer_procedure_editor.cpp
  composer_procedure_editor.h
  instruction_item_panel.cpp
  instruction_item_panel.h
  instruction_tree_widget.cpp
  instruction_tree_widget.h
  sequencer_composer_view.cpp
  sequencer_composer_view.h
  workspace_list_widget.cpp
  workspace_list_widget.h
)

