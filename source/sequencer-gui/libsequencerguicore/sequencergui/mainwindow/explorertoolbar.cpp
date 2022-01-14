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

#include "sequencergui/mainwindow/explorertoolbar.h"

#include "sequencergui/mainwindow/styleutils.h"

#include <QLabel>
#include <QToolButton>

namespace sequencergui
{
ExplorerToolBar::ExplorerToolBar(QWidget *parent) : QToolBar(parent), m_label(new QLabel)
{
  setIconSize(StyleUtils::ToolBarIconSize());
  //  setStyleSheet("QToolButton:!hover {background-color:#eff6fc} QToolBar {background: #eff6fc}");

  addWidget(m_label);

  AddDotsMenu();
}

void ExplorerToolBar::setText(const QString &text)
{
  m_label->setText(text);
}

void ExplorerToolBar::AddDotsMenu()
{
  insertStrech();

  auto button = new QToolButton;
  button->setIcon(StyleUtils::GetIcon("dots-horizontal.svg"));
  button->setIconSize(StyleUtils::ToolBarIconSize());
  addWidget(button);
}

//! Insert element after the label, or last insert element.
//! Push button representing dots and spacer will always stay at the right.

void ExplorerToolBar::InsertElement(QWidget *widget)
{
  insertWidget(m_spacer_action, widget);
}

void ExplorerToolBar::insertStrech()
{
  auto empty = new QWidget(this);
  empty->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  m_spacer_action = addWidget(empty);
}

}  // namespace sequencergui
