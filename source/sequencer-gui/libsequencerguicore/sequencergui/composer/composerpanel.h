/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_COMPOSER_COMPOSERPANEL_H
#define SEQUENCERGUI_COMPOSER_COMPOSERPANEL_H

#include <QWidget>

class QSplitter;

namespace sequencergui
{
class InstructionPanel;
class OpenDocumentsWidget;
class ExplorerToolBar;
class SequencerModel;
class ProcedureItem;
class WorkspacePanel;

//! Left panel on SequencerComposerView.
//! Contains list of opened procedures, list of available instruction, and list of available
//! variables.

class ComposerPanel : public QWidget
{
  Q_OBJECT

public:
  ComposerPanel(QWidget* parent = nullptr);
  ~ComposerPanel();

  void SetModel(SequencerModel* model);

signals:
  void procedureSelected(ProcedureItem* procedureItem);

private:
  ExplorerToolBar* m_tool_bar{nullptr};
  QSplitter* m_splitter{nullptr};
  OpenDocumentsWidget* m_open_documents_widget{nullptr};
  InstructionPanel* m_instruction_panel{nullptr};
  WorkspacePanel* m_workspace_panel{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_COMPOSER_COMPOSERPANEL_H
