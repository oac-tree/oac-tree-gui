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

#include "sequencergui/widgets/item_stack_widget.h"

#include <sequencergui/widgets/panel_toolbar.h>

#include <QStackedWidget>
#include <QToolBar>
#include <QVBoxLayout>

namespace sequencergui
{

ItemStackWidget::ItemStackWidget(QWidget *parent)
    : QWidget(parent), m_stacked_widget(new QStackedWidget), m_main_toolbar(new PanelToolBar)
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->setMargin(0);

  layout->addWidget(m_main_toolbar);
  layout->addWidget(m_stacked_widget);
}

void ItemStackWidget::AddWidget(QWidget *widget, QToolBar *toolbar, bool toolbar_is_always_visible)
{
  m_stacked_widget->addWidget(widget);

  if (toolbar)
  {
    m_main_toolbar->InsertElement(toolbar);
  }
}

void ItemStackWidget::SetCurrentIndex(int index)
{
  m_stacked_widget->setCurrentIndex(index);
}

}  // namespace sequencergui
