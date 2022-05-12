# Hierarchy of widgets

## MainWindow

- SequencerExplorerView
- SequencerComposerView
- SequencerMonitorView

## SequencerExplorerView

## SequencerComposerView

- ComposerPanel
  - ProcedureListView
  - InstructionItemPanel
    - ItemListWidget
  - AggregatePanel
- NodeEditor
- ComposerProcedureEditor
  - InstructionTreeWidget
  - WorkspaceListWidget

## SequencerMonitorView

- MonitorPanel
  - JobListWidget
  - JobPropertyPanel
- MonitorRealTimeWidget
- MonitorWorkspaceWidget
  