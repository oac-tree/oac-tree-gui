/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/nodeeditor/nodeeditortoolbar.h"

#include "sequencergui/mainwindow/styleutils.h"
#include "sequencergui/model/domainutils.h"
#include "sequencergui/nodeeditor/graphicsview.h"

#include "mvvm/widgets/widgetutils.h"

#include <QButtonGroup>
#include <QMenu>
#include <QToolButton>

namespace
{
const int kDefaultZoomLevel = 100;
}

namespace sequencergui
{
NodeEditorToolBar::NodeEditorToolBar(QWidget *parent)
    : QToolBar(parent)
    , m_pointer_mode_group(new QButtonGroup(this))
    , m_pointer_button(new QToolButton)
    , m_pan_button(new QToolButton)
    , m_center_button(new QToolButton)
    , m_zoom_button(new QToolButton)
    , m_align_button(new QToolButton)
    , m_align_button_v2(new QToolButton)
{
  setIconSize(StyleUtils::ToolBarIconSize());

  m_pointer_button->setIcon(StyleUtils::GetIcon("arrow-top-left.svg"));
  m_pointer_button->setToolButtonStyle(Qt::ToolButtonIconOnly);
  m_pointer_button->setToolTip("Scene in edit mode");
  m_pointer_button->setCheckable(true);
  m_pointer_button->setChecked(true);
  addWidget(m_pointer_button);

  m_pan_button->setIcon(StyleUtils::GetIcon("hand-back-right-outline.svg"));
  m_pan_button->setToolButtonStyle(Qt::ToolButtonIconOnly);
  m_pan_button->setToolTip("Scene in pan mode (space)");
  m_pan_button->setCheckable(true);
  addWidget(m_pan_button);

  m_pointer_mode_group = new QButtonGroup(this);
  m_pointer_mode_group->addButton(m_pointer_button, GraphicsView::kRubberSelection);
  m_pointer_mode_group->addButton(m_pan_button, GraphicsView::kHandDrag);
  connect(m_pointer_mode_group, &QButtonGroup::idClicked, this, &NodeEditorToolBar::selectionMode);

  addSeparator();

  m_zoom_menu = CreateZoomMenu();
  m_zoom_button->setText(QString("%1 \%").arg(kDefaultZoomLevel));
  m_zoom_button->setIcon(StyleUtils::GetIcon("magnify-plus-outline.svg"));

  //  m_zoom_button->setToolButtonStyle(Qt::ToolButtonTextOnly);
  m_zoom_button->setToolButtonStyle(Qt::ToolButtonIconOnly);

  m_zoom_button->setPopupMode(QToolButton::InstantPopup);
  m_zoom_button->setMenu(m_zoom_menu.get());
  m_zoom_button->setToolTip("Zoom");
  addWidget(m_zoom_button);

  m_center_button->setIcon(StyleUtils::GetIcon("camera-metering-center.svg"));
  m_center_button->setToolButtonStyle(Qt::ToolButtonIconOnly);
  m_center_button->setToolTip("Center view");
  connect(m_center_button, &QToolButton::clicked, this, &NodeEditorToolBar::centerView);

  addWidget(m_center_button);

  addSeparator();

  m_align_button->setIcon(StyleUtils::GetIcon("dots-triangle.svg"));
  m_align_button->setToolButtonStyle(Qt::ToolButtonIconOnly);
  m_align_button->setToolTip("Align children of currently selected item");
  connect(m_align_button, &QToolButton::clicked, this, &NodeEditorToolBar::alignSelectedRequest);
  addWidget(m_align_button);

  m_align_button_v2->setIcon(StyleUtils::GetIcon("dots-triangle.svg"));
  m_align_button_v2->setToolButtonStyle(Qt::ToolButtonIconOnly);
  m_align_button_v2->setToolTip("Align children of currently selected item");
  connect(m_align_button_v2, &QToolButton::clicked, this, &NodeEditorToolBar::alignSelectedRequestV2);
  addWidget(m_align_button_v2);

  AddDotsMenu();
}

NodeEditorToolBar::~NodeEditorToolBar() = default;

void NodeEditorToolBar::onViewSelectionMode(int mode)
{
  if (mode == GraphicsView::kRubberSelection || mode == GraphicsView::kHandDrag)
  {
    m_pointer_mode_group->button(mode)->setChecked(true);
  }
}

void NodeEditorToolBar::AddDotsMenu()
{
  InsertStrech();

  auto button = new QToolButton;
  button->setIcon(StyleUtils::GetIcon("dots-horizontal.svg"));
  button->setIconSize(StyleUtils::ToolBarIconSize());
  addWidget(button);
}

void NodeEditorToolBar::InsertStrech()
{
  auto empty = new QWidget(this);
  empty->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  addWidget(empty);
}

std::unique_ptr<QMenu> NodeEditorToolBar::CreateZoomMenu()
{
  auto result = std::make_unique<QMenu>();
  result->setToolTipsVisible(true);

  const std::vector<int> scales = {25, 50, 75, 100, 125, 150};
  std::map<int, QAction *> scale_to_action;
  for (auto scale : scales)
  {
    auto text = QString("%1 \%").arg(scale);
    auto action = result->addAction(text);
    scale_to_action[scale] = action;
    auto on_action = [this, scale, text]()
    {
      m_zoom_button->setText(text);
      emit changeScale(scale / 100.);
    };
    connect(action, &QAction::triggered, this, on_action);
  }

  result->setActiveAction(scale_to_action[kDefaultZoomLevel]);
  return result;
}

}  // namespace sequencergui
