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

#ifndef SEQUENCERGUI_EXPLORER_EXPLORER_PANEL_H_
#define SEQUENCERGUI_EXPLORER_EXPLORER_PANEL_H_

#include <QWidget>

namespace sequencergui
{
class FileTreeView;
class PanelToolBar;
class ProcedureListWidget;
class SequencerModel;
class ProcedureItem;
class CollapsibleListView;

//! Vertical panel located on the left of SequencerExplorerView.

class ExplorerPanel : public QWidget
{
  Q_OBJECT

public:
  explicit ExplorerPanel(QWidget* parent = nullptr);
  ~ExplorerPanel() override;

  void SetModel(SequencerModel* model);

  ProcedureItem* GetSelectedProcedure();

signals:
  void ProcedureFileClicked(const QString& filename);
  void ProcedureFileDoubleClicked(const QString& filename);
  void ProcedureSelected(sequencergui::ProcedureItem* procedure_item);
  void CreateNewProcedureRequest();
  void RemoveProcedureRequest(sequencergui::ProcedureItem* procedure_item);

private:
  PanelToolBar* m_tool_bar{nullptr};
  CollapsibleListView* m_collapsible_list{nullptr};
  FileTreeView* m_file_tree_view{nullptr};
  ProcedureListWidget* m_procedure_list_view{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_EXPLORER_EXPLORER_PANEL_H_
