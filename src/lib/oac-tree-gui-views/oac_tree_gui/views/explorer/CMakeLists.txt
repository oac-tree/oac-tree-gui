# Contains source for SequencerExplorerView.
# This is one of 3 main views of MainWindow.

target_sources(${library_name} PRIVATE
  explorer_panel.cpp
  explorer_panel.h
  procedure_trees_widget.cpp
  procedure_trees_widget.h
  sequencer_explorer_view.cpp
  sequencer_explorer_view.h
)
