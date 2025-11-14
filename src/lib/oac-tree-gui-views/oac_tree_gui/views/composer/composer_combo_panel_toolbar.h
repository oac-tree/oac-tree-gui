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

#ifndef OAC_TREE_GUI_VIEWS_COMPOSER_COMBO_PANEL_TOOLBAR_H_
#define OAC_TREE_GUI_VIEWS_COMPOSER_COMBO_PANEL_TOOLBAR_H_

#include <QToolBar>

class QComboBox;
class QAction;

namespace oac_tree_gui
{

/**
 * @brief The ComposerComboPanelToolBar class contains a combo selector to switch between different
 * documents, and control elements to split/close views.
 *
 * It is a part of  ComposerComboPanel.
 */
class ComposerComboPanelToolBar : public QToolBar
{
  Q_OBJECT

public:
  explicit ComposerComboPanelToolBar(QWidget* parent_widget = nullptr);
  ~ComposerComboPanelToolBar() override;

signals:
  void splitViewRequest();
  void closeViewRequest();

private:
  void InsertStretch();

  QComboBox* m_context_selector{nullptr};
  QAction* m_split_horizontally_action{nullptr};
  QAction* m_close_current_view_action{nullptr};
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_COMPOSER_COMBO_PANEL_TOOLBAR_H_
