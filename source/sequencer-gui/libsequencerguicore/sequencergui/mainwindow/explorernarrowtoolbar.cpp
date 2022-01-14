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

#include "sequencergui/mainwindow/explorernarrowtoolbar.h"

#include "sequencergui/mainwindow/styleutils.h"
#include "mvvm/widgets/widgetutils.h"

#include <QLabel>
#include <QToolBar>
#include <QToolButton>
#include <QVBoxLayout>

namespace sequencergui
{
ExplorerNarrowToolBar::ExplorerNarrowToolBar(QWidget *parent)
    : QFrame(parent)
    , m_tool_bar(new QToolBar)
    , m_expand_button(new QToolButton)
    , m_label(new QLabel)
{
  auto layout = new QVBoxLayout(this);
  layout->setMargin(0);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_tool_bar);

  m_tool_bar->setIconSize(StyleUtils::NarrowToolBarIconSize());

  // make it narrow, buttons closer to each other
  m_tool_bar->setStyleSheet(
      "QToolBar { padding-bottom: 0px; margin-bottom: 0px; } QToolButton { padding: 0px; margin: "
      "0px; }");

  m_expand_button->setIcon(StyleUtils::GetIcon("chevron-down.svg"));
  auto on_expand_click = [this]()
  {
    m_expanded = !m_expanded;
    UpdateToolBar();
  };
  connect(m_expand_button, &QToolButton::clicked, this, on_expand_click);
  m_tool_bar->addWidget(m_expand_button);

  m_tool_bar->addWidget(m_label);
  InsertStrech();

  setFixedHeight(StyleUtils::NarrowToolBarIconSize().height() * 1.5);

  UpdateToolBar();
}

//! Set text next to collapse/expand icon.

void ExplorerNarrowToolBar::SetText(const QString &text)
{
  mvvm::utils::ScaleLabelFont(m_label, 0.9);
  m_label->setText(text);
}

//! Sets the widget which will be collapsed/expanded.

void ExplorerNarrowToolBar::SetControlledWidget(QWidget *widget)
{
  m_controlled_widget = widget;
}

void ExplorerNarrowToolBar::AddWidget(QWidget *widget)
{
  // Feature of QToolBar: returns action on adding any widget or button.
  // These actions can be used to hide widgets from the toolbar (see ::UpdateToolBar)
  auto action = m_tool_bar->addWidget(widget);
  m_toolbar_actions.append(action);  // to hide/show elements later
}

//! Updates toolbar appearance depending on collapsed/expaned status

void ExplorerNarrowToolBar::UpdateToolBar()
{
  UpdateIcon();

  // show/hide controlled widget
  if (m_controlled_widget)
  {
    m_controlled_widget->setVisible(m_expanded);
  }

  // show/hide toolbar elements
  for (auto action : m_toolbar_actions)
  {
    action->setVisible(m_expanded);
  }
}

//! Updates collapse/expand icon.

void ExplorerNarrowToolBar::UpdateIcon()
{
  if (m_expanded)
  {
    m_expand_button->setIcon(StyleUtils::GetIcon("chevron-down.svg"));
    setFrameStyle(QFrame::StyledPanel);
  }
  else
  {
    m_expand_button->setIcon(StyleUtils::GetIcon("chevron-right.svg"));
    setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
  }
}

void ExplorerNarrowToolBar::InsertStrech()
{
  auto empty = new QWidget(this);
  empty->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  m_tool_bar->addWidget(empty);
}

}  // namespace sequi
