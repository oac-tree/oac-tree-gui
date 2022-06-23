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

#include "sequencergui/widgets/panel_toolbar.h"

#include <sequencergui/utils/style_utils.h>

#include <QLabel>
#include <QToolButton>

namespace sequencergui
{
PanelToolBar::PanelToolBar(QWidget *parent) : QToolBar(parent), m_label(new QLabel)
{
  setIconSize(styleutils::ToolBarIconSize());
  //  setStyleSheet("QToolButton:!hover {background-color:#eff6fc} QToolBar {background: #eff6fc}");

  addWidget(m_label);

  AddDotsMenu();
}

void PanelToolBar::setText(const QString &text)
{
  m_label->setText(text);
}

void PanelToolBar::AddDotsMenu()
{
  InsertStrech();

  m_dots_button = new QToolButton;
  m_dots_button->setIcon(styleutils::GetIcon("dots-horizontal.svg"));
  m_dots_button->setIconSize(styleutils::ToolBarIconSize());
  addWidget(m_dots_button);
}

//! Insert element after the label, or last insert element.
//! Push button representing dots and spacer will always stay at the right.

QAction *PanelToolBar::InsertElement(QWidget *widget)
{
  return insertWidget(m_spacer_action, widget);
}

//! Sets a menu to the button with dots. The menu ownership remains on caller side.

void PanelToolBar::SetDotsMenu(QMenu *dots_menu)
{
  m_dots_button->setMenu(dots_menu);
  m_dots_button->setPopupMode(QToolButton::InstantPopup);
}

void PanelToolBar::InsertStrech()
{
  auto empty = new QWidget(this);
  empty->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  m_spacer_action = addWidget(empty);
}

}  // namespace sequencergui
