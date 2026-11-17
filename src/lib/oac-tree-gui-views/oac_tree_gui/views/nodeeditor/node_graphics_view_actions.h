/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#ifndef OAC_TREE_GUI_VIEWS_NODEEDITOR_NODE_GRAPHICS_VIEW_ACTIONS_H_
#define OAC_TREE_GUI_VIEWS_NODEEDITOR_NODE_GRAPHICS_VIEW_ACTIONS_H_

#include <oac_tree_gui/nodeeditor/graphics_scene_types.h>

#include <sup/gui/components/action_map.h>

#include <QObject>
#include <memory>

class QToolBar;
class QButtonGroup;
class QToolButton;
class QMenu;
class QWidgetAction;

namespace oac_tree_gui
{

/**
 * @brief The NodeGraphicsViewActions class contains a collection of actions related to node
 * graphics view (zoom/pan/center and similar).
 */
class NodeGraphicsViewActions : public QObject
{
  Q_OBJECT

public:
  enum class ActionKey : std::uint8_t
  {
    kPointer,
    kPan,
    kAlign,
    kSubTree
  };

  explicit NodeGraphicsViewActions(QWidget* parent_widget = nullptr);
  ~NodeGraphicsViewActions() override;

  NodeGraphicsViewActions(const NodeGraphicsViewActions&) = delete;
  NodeGraphicsViewActions& operator=(const NodeGraphicsViewActions&) = delete;
  NodeGraphicsViewActions(NodeGraphicsViewActions&&) = delete;
  NodeGraphicsViewActions& operator=(NodeGraphicsViewActions&&) = delete;

  void UpdateButtonsToOperationMode(GraphicsViewOperationMode mode);

  /**
   * @brief Returns list of actions according to provided flags.
   */
  QList<QAction*> GetActions(const std::vector<ActionKey>& action_keys) const;

  /**
   * @brief Sets the text of the SubTree selector button.
   */
  void SetSubTreeText(const QString& text);

signals:
  void OperationModeChangeRequest(oac_tree_gui::GraphicsViewOperationMode);
  void alignSelectedRequest();

  /**
   * @brief Notifies that the SubTree selector menu is about to show. The receiver fills the menu.
   */
  void subTreeMenuAboutToShow(QMenu* menu);

private:
  std::unique_ptr<QMenu> CreateZoomMenu();

  QButtonGroup* m_pointer_mode_group{nullptr};
  QToolButton* m_pointer_button{nullptr};
  QWidgetAction* m_pointer_action{nullptr};
  QToolButton* m_pan_button{nullptr};
  QWidgetAction* m_pan_action{nullptr};
  QAction* m_align_action{nullptr};
  QToolButton* m_subtree_button{nullptr};
  QWidgetAction* m_subtree_action{nullptr};
  std::unique_ptr<QMenu> m_subtree_menu;

  sup::gui::ActionMap<ActionKey> m_action_map;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_NODEEDITOR_NODE_GRAPHICS_VIEW_ACTIONS_H_
