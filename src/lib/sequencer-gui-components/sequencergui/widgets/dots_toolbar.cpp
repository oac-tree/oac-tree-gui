/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/widgets/dots_toolbar.h"

#include <sequencergui/domain/domain_utils.h>
#include <sequencergui/widgets/style_utils.h>

#include <mvvm/widgets/widget_utils.h>
#include <sup/gui/widgets/style_utils.h>

#include <QAction>
#include <QLabel>
#include <QMenu>
#include <QPushButton>
#include <QToolButton>

namespace sequencergui
{
DotsToolBar::DotsToolBar(QWidget *parent) : QToolBar(parent)
{
  setIconSize(sup::gui::utils::ToolBarIconSize());
}

void DotsToolBar::SetActions(const QList<QAction *> &actions)
{
  clear();
  for (auto action : actions)
  {
    addAction(action);
  }
  AddDotsMenu();
}

void DotsToolBar::SetWidgets(const QList<QWidget *> &widgets)
{
  clear();
  for (auto widget : widgets)
  {
    addWidget(widget);
  }
  AddDotsMenu();
}

DotsToolBar::~DotsToolBar() = default;

void DotsToolBar::AddDotsMenu()
{
  InsertStrech();

  auto button = new QToolButton;
  button->setIcon(styleutils::GetIcon("dots-horizontal.svg"));
  button->setIconSize(sup::gui::utils::ToolBarIconSize());
  addWidget(button);
}

void DotsToolBar::InsertStrech()
{
  auto empty = new QWidget(this);
  empty->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  addWidget(empty);
}

}  // namespace sequencergui
