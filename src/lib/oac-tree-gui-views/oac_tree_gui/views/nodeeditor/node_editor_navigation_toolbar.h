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

#ifndef OAC_TREE_GUI_VIEWS_NODEEDITOR_NODE_EDITOR_NAVIGATION_TOOLBAR_H_
#define OAC_TREE_GUI_VIEWS_NODEEDITOR_NODE_EDITOR_NAVIGATION_TOOLBAR_H_

#include <QToolBar>

#include <oac_tree_gui/views/nodeeditor/zoom_factor_converter.h>
#include <sup/gui/components/action_map.h>

class QSlider;
class QAction;
class QMenu;

namespace sup::gui
{
class ActionMenu;
}

namespace oac_tree_gui
{

class NodeEditorNavigationToolBar : public QToolBar
{
  Q_OBJECT

public:
  enum class ActionKey : std::uint8_t
  {
    kCenter,
    kFitToView,
    kFixedZoomLevel
  };

  explicit NodeEditorNavigationToolBar(QWidget* parent_widget = nullptr);

  void SetZoomFactor(double zoom_factor);

signals:
  void ZoomFactorRequest(double zoom_factor);
  void CenterViewRequest();
  void FitToViewRequest();

private:
  void InsertStretch();
  void SetupSlider();
  void SetupActions();
  std::unique_ptr<QMenu> CreateZoomMenu();

  QSlider* m_zoom_slider{nullptr};
  ZoomFactorConverter m_zoom_factor_converter;

  QAction* m_center_action{nullptr};
  QAction* m_fit_to_view_action{nullptr};
  sup::gui::ActionMenu* m_zoom_action{nullptr};

  std::unique_ptr<QMenu> m_zoom_menu;
  sup::gui::ActionMap<ActionKey> m_action_map;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_NODEEDITOR_NODE_EDITOR_NAVIGATION_TOOLBAR_H_
