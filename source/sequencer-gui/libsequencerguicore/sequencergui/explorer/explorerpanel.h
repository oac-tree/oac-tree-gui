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

#ifndef SEQUENCERGUI_EXPLORER_EXPLORERPANEL_H
#define SEQUENCERGUI_EXPLORER_EXPLORERPANEL_H

#include <QWidget>

class QSplitter;

namespace sequencergui
{
class FileTreeView;
class PanelToolBar;
class OpenDocumentsWidget;
class SequencerModel;
class ProcedureItem;

//! Vertical panel located on the left of XMLTreeView

class ExplorerPanel : public QWidget
{
  Q_OBJECT

public:
  explicit ExplorerPanel(QWidget* parent = nullptr);
  ~ExplorerPanel() override;

  void SetModel(SequencerModel* model);

  ProcedureItem* GetCurrentScratchpadProcedure();

signals:
  void procedureFileClicked(const QString& filename);
  void procedureFileDoubleClicked(const QString& filename);
  void createNewProcedureRequest();
  void sratchpadProcedureSelected(ProcedureItem* procedureItem);

private:
  PanelToolBar* m_tool_bar{nullptr};

  QSplitter* m_splitter{nullptr};
  FileTreeView* m_file_tree_view{nullptr};
  OpenDocumentsWidget* m_open_documents_widget{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_EXPLORER_EXPLORERPANEL_H
