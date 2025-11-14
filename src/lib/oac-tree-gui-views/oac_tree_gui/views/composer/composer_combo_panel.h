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

#ifndef OAC_TREE_GUI_VIEWS_COMPOSER_COMPOSER_COMBO_PANEL_H_
#define OAC_TREE_GUI_VIEWS_COMPOSER_COMPOSER_COMBO_PANEL_H_

#include <QWidget>

class QStackedWidget;

namespace oac_tree_gui
{

class ComposerComboPanelToolBar;
class ProcedureComposerTabWidget;
class PlaceholderWidget;

/**
 * @brief The ComposerComboPanel class contains a complex procedure editor and allows switching
 * between editing different opened procedures.
 *
 * The widget contains a toolbar with a combo selector and buttons to split/close views, an instance
 * of ProcedureComposerTabWidget (complex procedure editor). It also contains a placeholder to show
 * when no procedure is opened.
 */
class ComposerComboPanel : public QWidget
{
  Q_OBJECT

public:
  explicit ComposerComboPanel(QWidget* parent_widget = nullptr);
  ~ComposerComboPanel() override;

signals:
  void splitViewRequest();
  void closeViewRequest();

protected:
  void mousePressEvent(QMouseEvent *event) override;
  void focusInEvent(QFocusEvent *event) override;

private:
  void SetupConnections();

  ComposerComboPanelToolBar* m_tool_bar{nullptr};
  QStackedWidget* m_stacked_widget{nullptr};
  PlaceholderWidget* m_placeholder_widget{nullptr};
  ProcedureComposerTabWidget* m_procedure_composer_widget{nullptr};
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_COMPOSER_COMPOSER_COMBO_PANEL_H_
