/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "node_editor_actions.h"

#include <sequencergui/nodeeditor/graphics_scene_types.h>

#include <sequencergui/domain/domain_utils.h>
#include <sup/gui/widgets/action_menu.h>
#include <sup/gui/widgets/style_utils.h>

#include <mvvm/widgets/widget_utils.h>

#include <QButtonGroup>
#include <QMenu>
#include <QToolButton>
#include <QWidgetAction>

namespace
{
const int kDefaultZoomLevel = 100;

QString GetZoomText(int scale)
{
  return QString("Zoom %1 \%").arg(scale);
}

}  // namespace

namespace sequencergui
{
NodeEditorActions::NodeEditorActions(QWidget *parent)
    : QObject(parent)
    , m_pointer_mode_group(new QButtonGroup(this))
    , m_pointer_button(new QToolButton)
    , m_pointer_action(new QWidgetAction(this))
    , m_pan_button(new QToolButton)
    , m_pan_action(new QWidgetAction(this))
    , m_center_action(new QAction(this))
    , m_zoom_action(new sup::gui::ActionMenu(this))
    , m_align_action(new QAction(this))
    , m_zoom_menu(CreateZoomMenu())
{
  m_pointer_button->setText("Select");
  m_pointer_button->setIcon(sup::gui::utils::GetIcon("arrow-top-left.svg"));
  m_pointer_button->setToolTip("Scene in edit mode");
  m_pointer_button->setCheckable(true);
  m_pointer_button->setChecked(true);
  m_pointer_button->setToolButtonStyle(Qt::ToolButtonFollowStyle);
  m_pointer_action->setDefaultWidget(m_pointer_button);
  m_action_map.Add(ActionKey::kPointer, m_pointer_action);

  m_pan_button->setText("Pan");
  m_pan_button->setIcon(sup::gui::utils::GetIcon("hand-back-right-outline.svg"));
  m_pan_button->setToolTip("Scene in pan mode (space)");
  m_pan_button->setCheckable(true);
  m_pan_button->setToolButtonStyle(Qt::ToolButtonFollowStyle);
  m_pan_action->setText("Select");
  m_pan_action->setDefaultWidget(m_pan_button);
  m_action_map.Add(ActionKey::kPan, m_pan_action);

  m_pointer_mode_group->addButton(m_pointer_button, kRubberSelection);
  m_pointer_mode_group->addButton(m_pan_button, kHandDrag);
  connect(m_pointer_mode_group, &QButtonGroup::idClicked, this, &NodeEditorActions::selectionMode);

  m_zoom_action->setText(GetZoomText(kDefaultZoomLevel));
  m_zoom_action->setIcon(sup::gui::utils::GetIcon("magnify-plus-outline.svg"));
  m_zoom_action->setMenu(m_zoom_menu.get());
  m_zoom_action->setToolTip("Zoom");
  m_action_map.Add(ActionKey::kZoom, m_zoom_action);

  m_center_action->setText("Center");
  m_center_action->setIcon(sup::gui::utils::GetIcon("camera-metering-center.svg"));
  m_center_action->setToolTip("Center view");
  connect(m_center_action, &QAction::triggered, this, &NodeEditorActions::centerView);
  m_action_map.Add(ActionKey::kCenter, m_center_action);

  m_align_action->setText("Align");
  m_align_action->setIcon(sup::gui::utils::GetIcon("dots-triangle.svg"));
  m_align_action->setToolTip("Align children of currently selected item");
  connect(m_align_action, &QAction::triggered, this, &NodeEditorActions::alignSelectedRequest);
  m_action_map.Add(ActionKey::kAlign, m_align_action);
}

NodeEditorActions::~NodeEditorActions() = default;

void NodeEditorActions::onViewSelectionMode(int mode)
{
  if (mode == kRubberSelection || mode == kHandDrag)
  {
    m_pointer_mode_group->button(mode)->setChecked(true);
  }
}

QList<QAction *> NodeEditorActions::GetActions(const std::vector<ActionKey> &action_keys) const
{
  return m_action_map.GetActions(action_keys);
}

std::unique_ptr<QMenu> NodeEditorActions::CreateZoomMenu()
{
  auto result = std::make_unique<QMenu>();
  result->setToolTipsVisible(true);

  const std::vector<int> scales = {10, 15, 25, 50, 75, 100, 125, 150};
  std::map<int, QAction *> scale_to_action;
  for (auto scale : scales)
  {
    auto text = GetZoomText(scale);
    auto action = result->addAction(text);
    scale_to_action[scale] = action;
    auto on_action = [this, scale, text]()
    {
      m_zoom_action->setText(text);
      emit changeScale(scale / 100.);
    };
    connect(action, &QAction::triggered, this, on_action);
  }

  result->setActiveAction(scale_to_action[kDefaultZoomLevel]);
  return result;
}

}  // namespace sequencergui
