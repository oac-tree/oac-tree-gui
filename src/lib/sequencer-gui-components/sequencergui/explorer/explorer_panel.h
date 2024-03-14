/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
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

namespace sup::gui
{
class CollapsibleListView;
class ItemStackWidget;
}  // namespace sup::gui

namespace sequencergui
{

class FileTreeView;
class ProcedureListWidget;
class SequencerModel;
class ProcedureItem;

/**
 * @brief The ExplorerPanel class represents a vertical panel on the left of SequencerExplorerView.
 *
 * Contains a FileTree widget on top directory browsing, and list of opened procedures at the
 * bottom.
 */
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

private:
  void ReadSettings();
  void WriteSettings();

  sup::gui::CollapsibleListView* m_collapsible_list{nullptr};
  FileTreeView* m_file_tree_view{nullptr};
  ProcedureListWidget* m_procedure_list_view{nullptr};
  sup::gui::ItemStackWidget* m_stack_widget{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_EXPLORER_EXPLORER_PANEL_H_
