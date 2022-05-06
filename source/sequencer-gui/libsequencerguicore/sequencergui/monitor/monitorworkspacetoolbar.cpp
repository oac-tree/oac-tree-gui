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

#include "sequencergui/monitor/monitorworkspacetoolbar.h"

#include "sequencergui/utils/styleutils.h"

#include <QAction>
#include <QLabel>
#include <QMenu>
#include <QPushButton>
#include <QToolButton>

namespace sequencergui
{
MonitorWorkspaceToolBar::MonitorWorkspaceToolBar(QWidget *parent) : QToolBar(parent)
{
  setIconSize(StyleUtils::ToolBarIconSize());

  AddDotsMenu();
}

MonitorWorkspaceToolBar::~MonitorWorkspaceToolBar() = default;

void MonitorWorkspaceToolBar::AddDotsMenu()
{
  InsertStrech();

  auto button = new QToolButton;
  button->setIcon(StyleUtils::GetIcon("dots-horizontal.svg"));
  button->setIconSize(StyleUtils::ToolBarIconSize());
  addWidget(button);
}

void MonitorWorkspaceToolBar::InsertStrech()
{
  auto empty = new QWidget(this);
  empty->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  addWidget(empty);
}

}  // namespace sequencergui
