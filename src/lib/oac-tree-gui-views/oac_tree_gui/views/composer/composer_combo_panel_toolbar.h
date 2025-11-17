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
#include <functional>

class QComboBox;
class QAction;
class QMenu;

namespace sup::gui
{
class ActionMenu;
}  // namespace sup::gui

namespace oac_tree_gui
{

class ProcedureItem;

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
  using ProceduresCallback = std::function<std::vector<ProcedureItem*>()>;

  explicit ComposerComboPanelToolBar(const ProceduresCallback& procedure_callback,
                                     QWidget* parent_widget = nullptr);
  ~ComposerComboPanelToolBar() override;

signals:
  void splitViewRequest();
  void closeViewRequest();
  void SelectProcedureRequest(oac_tree_gui::ProcedureItem* item);

private:
  void InsertStretch();
  std::unique_ptr<QMenu> CreateSelectProcedureMenu();
  void OnAboutToShowMenu();

  ProceduresCallback m_available_procedures;
  std::unique_ptr<QMenu> m_select_procedure_menu;
  sup::gui::ActionMenu* m_select_procedure_action{nullptr};
  QComboBox* m_context_selector{nullptr};
  QAction* m_split_horizontally_action{nullptr};
  QAction* m_close_current_view_action{nullptr};
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_COMPOSER_COMBO_PANEL_TOOLBAR_H_
