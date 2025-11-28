/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef OAC_TREE_GUI_VIEWS_EXPLORER_EXPLORER_PANEL_H_
#define OAC_TREE_GUI_VIEWS_EXPLORER_EXPLORER_PANEL_H_

#include <QWidget>

namespace sup::gui
{
class CollapsibleListView;
class ItemStackWidget;
class IAppCommandService;
}  // namespace sup::gui

namespace oac_tree_gui
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
  explicit ExplorerPanel(sup::gui::IAppCommandService& command_service,
                         QWidget* parent_widget = nullptr);
  ~ExplorerPanel() override;

  void SetModel(SequencerModel* model);

signals:
  void FileTreeClicked(const QString& filename);
  void ProcedureFileDoubleClicked(const QString& filename);

private:
  void ReadSettings();
  void WriteSettings();

  sup::gui::CollapsibleListView* m_collapsible_list{nullptr};
  FileTreeView* m_file_tree_view{nullptr};
  ProcedureListWidget* m_procedure_list_view{nullptr};
  sup::gui::ItemStackWidget* m_stack_widget{nullptr};
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_EXPLORER_EXPLORER_PANEL_H_
