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

#include "item_stack_widget.h"

#include <sequencergui/widgets/panel_toolbar.h>

#include <QAction>
#include <QMenu>
#include <QStackedWidget>
#include <QToolBar>
#include <QVBoxLayout>

namespace sequencergui
{

ItemStackWidget::ItemStackWidget(QWidget *parent)
    : QWidget(parent)
    , m_stacked_widget(new QStackedWidget)
    , m_widget_selection_menu(std::make_unique<QMenu>())
    , m_main_toolbar(new PanelToolBar)
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  layout->addWidget(m_main_toolbar);
  layout->addWidget(m_stacked_widget);

  m_main_toolbar->SetDotsMenu(m_widget_selection_menu.get());
}

ItemStackWidget::~ItemStackWidget() = default;

void ItemStackWidget::AddWidget(QWidget *widget)
{
  m_stacked_widget->addWidget(widget);
  AddMenuEntry(widget);
}

void ItemStackWidget::AddWidget(QWidget *widget, const QList<QAction *> &actions,
                                bool is_always_visible)
{
  AddWidget(widget);
  AddGuestActions(actions, is_always_visible);
}

void ItemStackWidget::SetCurrentIndex(int index)
{
  m_stacked_widget->setCurrentIndex(index);
  UpdateControlElementsVisibility();
}

//! Add entry to corner menu to switch to this widget.

void ItemStackWidget::AddMenuEntry(QWidget *widget)
{
  int index = m_stacked_widget->count() - 1;
  auto action = m_widget_selection_menu->addAction(widget->windowTitle());
  auto on_action = [this, index]() { SetCurrentIndex(index); };
  connect(action, &QAction::triggered, this, on_action);
}

void ItemStackWidget::AddGuestActions(const QList<QAction *> &actions, bool is_always_visible)
{
  QList<QAction *> hide_actions;  // handles to hide actions

  if (m_stacked_widget->count() > 1)
  {
    hide_actions.append(m_main_toolbar->AppendSeparator());
  }

  for (auto action : actions)
  {
    hide_actions.append(m_main_toolbar->InsertElement(action));
  }

  m_toolbar_data.append({actions, is_always_visible});
  UpdateControlElementsVisibility();
}

//! Updates visibility of guest toolbars.
//! It will be visible for current widget in the stack, and hidded for others (unless
//! is_always_visible flag is present);

void ItemStackWidget::UpdateControlElementsVisibility()
{
  for (int i = 0; i < m_toolbar_data.size(); ++i)
  {
    bool is_visible = m_stacked_widget->currentIndex() == i || m_toolbar_data[i].is_always_visible;
    for (auto action : m_toolbar_data[i].actions)
    {
      action->setVisible(is_visible);
    }
  }
}

}  // namespace sequencergui
