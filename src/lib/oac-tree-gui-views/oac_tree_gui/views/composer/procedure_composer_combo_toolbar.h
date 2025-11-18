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

#ifndef OAC_TREE_GUI_VIEWS_COMPOSER_PROCEDURE_COMPOSER_COMBO_TOOLBAR_H_
#define OAC_TREE_GUI_VIEWS_COMPOSER_PROCEDURE_COMPOSER_COMBO_TOOLBAR_H_

#include <QToolBar>
#include <functional>

class QMenu;
class QToolButton;

namespace oac_tree_gui
{

class ProcedureItem;

/**
 * @brief The ComposerComboPanelToolBar class contains a combo selector to switch between different
 * documents, and control elements to split/close views.
 *
 * It is a part of  ProcedureComposerComboPanel.
 */
class ProcedureComposerComboToolBar : public QToolBar
{
  Q_OBJECT

public:
  using ProceduresCallback = std::function<std::vector<ProcedureItem*>()>;

  explicit ProcedureComposerComboToolBar(const ProceduresCallback& procedure_callback,
                                              QWidget* parent_widget = nullptr);
  ~ProcedureComposerComboToolBar() override;

  /**
   * @brief Shows the toolbar as active/inactive.
   *
   * Active toolbar corresponds to the widget where the user clicked last, and it has a thin green
   * line at the bottom.
   */
  void ShowAsActive(bool value);

  /**
   * @brief Marks this toolbar as the last editor in the splitter.
   */
  void ShowAsLastEditor(bool value);

  /**
   * @brief Updates the procedure selection menu to highlight the selected procedure.
   */
  void UpdateProcedureSelectionMenu(ProcedureItem* selected_procedure);

  /**
   * @brief Returns a procedure name selected in a toolbar combo.
   */
  QString GetSelectedProcedureName() const;

signals:
  void splitViewRequest();
  void closeViewRequest();
  void SelectProcedureRequest(oac_tree_gui::ProcedureItem* item);

protected:
  void paintEvent(QPaintEvent* event) override;

private:
  void InsertStretch();
  std::unique_ptr<QMenu> CreateSelectProcedureMenu();
  void OnAboutToShowMenu();

  ProceduresCallback m_available_procedures;
  std::unique_ptr<QMenu> m_select_procedure_menu;
  QToolButton* m_select_procedure_button{nullptr};
  QToolButton* m_split_horizontally_button{nullptr};
  QToolButton* m_close_current_view_button{nullptr};
  bool m_show_as_active{false};
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_COMPOSER_PROCEDURE_COMPOSER_COMBO_TOOLBAR_H_
