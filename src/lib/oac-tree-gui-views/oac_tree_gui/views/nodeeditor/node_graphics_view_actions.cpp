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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "node_graphics_view_actions.h"

#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/nodeeditor/graphics_scene_types.h>
#include <oac_tree_gui/style/style_helper.h>

#include <sup/gui/widgets/action_menu.h>

#include <mvvm/widgets/widget_utils.h>

#include <QButtonGroup>
#include <QMenu>
#include <QToolButton>
#include <QWidgetAction>

namespace oac_tree_gui
{
NodeGraphicsViewActions::NodeGraphicsViewActions(QWidget *parent_widget)
    : QObject(parent_widget)
    , m_pointer_mode_group(new QButtonGroup(this))
    , m_pointer_button(new QToolButton)
    , m_pointer_action(new QWidgetAction(this))
    , m_pan_button(new QToolButton)
    , m_pan_action(new QWidgetAction(this))
    , m_align_action(new QAction(this))
{
  m_pointer_button->setText("Select");
  m_pointer_button->setIcon(FindIcon("arrow-top-left"));
  m_pointer_button->setToolTip("Scene in edit mode");
  m_pointer_button->setCheckable(true);
  m_pointer_button->setChecked(true);
  m_pointer_button->setToolButtonStyle(Qt::ToolButtonFollowStyle);
  m_pointer_action->setDefaultWidget(m_pointer_button);
  m_action_map.Add(ActionKey::kPointer, m_pointer_action);

  m_pan_button->setText("Pan");
  m_pan_button->setIcon(FindIcon("hand-back-right-outline"));
  m_pan_button->setToolTip("Scene in pan mode (space)");
  m_pan_button->setCheckable(true);
  m_pan_button->setToolButtonStyle(Qt::ToolButtonFollowStyle);
  m_pan_action->setText("Select");
  m_pan_action->setDefaultWidget(m_pan_button);
  m_action_map.Add(ActionKey::kPan, m_pan_action);

  m_pointer_mode_group->addButton(m_pointer_button,
                                  static_cast<int>(GraphicsViewOperationMode::kRubberSelection));
  m_pointer_mode_group->addButton(m_pan_button,
                                  static_cast<int>(GraphicsViewOperationMode::kHandDrag));
  auto on_button_clicked = [this](int button_id)
  {
    if (button_id >= 0)
    {
      emit OperationModeChangeRequest(static_cast<GraphicsViewOperationMode>(button_id));
    }
  };
  connect(m_pointer_mode_group, &QButtonGroup::idClicked, this, on_button_clicked);

  m_align_action->setText("Align");
  m_align_action->setIcon(FindIcon("dots-triangle"));
  m_align_action->setToolTip("Align children of currently selected item");
  connect(m_align_action, &QAction::triggered, this,
          &NodeGraphicsViewActions::alignSelectedRequest);
  m_action_map.Add(ActionKey::kAlign, m_align_action);
}

NodeGraphicsViewActions::~NodeGraphicsViewActions() = default;

void NodeGraphicsViewActions::UpdateButtonsToOperationMode(GraphicsViewOperationMode mode)
{
  if (mode == GraphicsViewOperationMode::kRubberSelection
      || mode == GraphicsViewOperationMode::kHandDrag)
  {
    m_pointer_mode_group->button(static_cast<int>(mode))->setChecked(true);
  }
}

QList<QAction *> NodeGraphicsViewActions::GetActions(
    const std::vector<ActionKey> &action_keys) const
{
  return m_action_map.GetActions(action_keys);
}

}  // namespace oac_tree_gui
