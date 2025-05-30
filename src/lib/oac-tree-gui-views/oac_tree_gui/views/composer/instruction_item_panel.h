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

#ifndef OAC_TREE_GUI_VIEWS_COMPOSER_INSTRUCTION_ITEM_PANEL_H_
#define OAC_TREE_GUI_VIEWS_COMPOSER_INSTRUCTION_ITEM_PANEL_H_

#include <QWidget>

class QLineEdit;

namespace mvvm
{
class FilterNameViewModel;
}

namespace oac_tree_gui
{

class ToolKitViewModel;
class InsructionToolKitTreeView;

/**
 * @brief The InstructionItemPanel class is a tree with instruction type names grouped according to
 * their plugin origin.
 *
 * It is located on the left-hand-side of SequencerComposerView.
 */
class InstructionItemPanel : public QWidget
{
  Q_OBJECT

public:
  explicit InstructionItemPanel(QWidget* parent_widget = nullptr);

signals:
  void InstructionDoubleClicked(const QString& name);

private:
  void OnContextMenuRequest(const QPoint& point);

  ToolKitViewModel* m_instruction_toolkit_viewmodel{nullptr};
  mvvm::FilterNameViewModel* m_proxy_model{nullptr};
  InsructionToolKitTreeView* m_tree_view{nullptr};
  QLineEdit* m_line_edit{nullptr};
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_COMPOSER_INSTRUCTION_ITEM_PANEL_H_
