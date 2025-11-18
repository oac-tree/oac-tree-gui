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
#include <functional>

class QStackedWidget;

namespace mvvm
{
class ModelListener;
}

namespace oac_tree_gui
{

class ComposerComboPanelToolBar;
class ProcedureComposerTabWidget;
class PlaceholderWidget;
class ProcedureItem;

/**
 * @brief The ComposerComboPanel class contains a complex procedure editor and allows switching
 * between editing different opened procedures.
 *
 * The widget contains
 * - toolbar with a combo selector to select between various procedures
 * - tool buttons to request the split of the view, or current view close
 * - an instance of complex procedure editor (ProcedureComposerTabWidget)
 * - a placeholder to show when no procedure is opened.
 */
class ComposerComboPanel : public QWidget
{
  Q_OBJECT

public:
  using ProceduresCallback = std::function<std::vector<ProcedureItem*>()>;

  enum class WidgetType : std::uint8_t
  {
    kPlaceholderWidget,
    kProcedureComposerWidget
  };

  explicit ComposerComboPanel(const ProceduresCallback& procedure_callback,
                              QWidget* parent_widget = nullptr);
  ~ComposerComboPanel() override;

  /**
   * @brief Sets the procedure to be edited.
   */
  void SetProcedure(ProcedureItem* procedure_item);

  /**
   * @brief Marks this editor as active.
   */
  void ShowAsActive(bool value);

  /**
   * @brief Marks this editor as the last editor in the splitter.
   */
  void ShowAsLastEditor(bool value);

signals:
  void splitViewRequest();
  void closeViewRequest();
  void panelFocusRequest();

protected:
  void mousePressEvent(QMouseEvent* event) override;

private:
  void SetupConnections();
  void OnSelectProcedureRequest(oac_tree_gui::ProcedureItem* item);
  void ShowWidget(WidgetType widget_type);
  void InitModelListener();

  ComposerComboPanelToolBar* m_tool_bar{nullptr};
  QStackedWidget* m_stacked_widget{nullptr};
  PlaceholderWidget* m_placeholder_widget{nullptr};
  ProcedureComposerTabWidget* m_procedure_composer_widget{nullptr};
  std::unique_ptr<mvvm::ModelListener> m_listener;
  ProcedureItem* m_current_procedure_item{nullptr};
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_COMPOSER_COMPOSER_COMBO_PANEL_H_
