/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#ifndef SEQUENCERGUI_COMPOSER_COMPOSERPANEL_H
#define SEQUENCERGUI_COMPOSER_COMPOSERPANEL_H

#include <QWidget>

class QSplitter;

namespace sequi
{
class InstructionPanel;
class OpenDocumentsWidget;
class ExplorerToolBar;
class SequencerModel;
class ProcedureItem;
class WorkspacePanel;

//! Left panel on SequencerComposerView.
//! Contains scratchpad, list of available instruction, and list of available variables.

class ComposerPanel : public QWidget
{
  Q_OBJECT

public:
  ComposerPanel(QWidget* parent = nullptr);
  ~ComposerPanel();

  void SetModel(SequencerModel* model);

signals:
  void sratchpadProcedureSelected(ProcedureItem* procedureItem);

private:
  ExplorerToolBar* m_tool_bar{nullptr};
  QSplitter* m_splitter{nullptr};
  OpenDocumentsWidget* m_open_documents_widget{nullptr};
  InstructionPanel* m_instruction_panel{nullptr};
  WorkspacePanel* m_workspace_panel{nullptr};
};

}  // namespace sequi

#endif  // SEQUENCERGUI_COMPOSER_COMPOSERPANEL_H
