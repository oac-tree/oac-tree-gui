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

#include "sequencergui/widgets/collapsiblewidget.h"

#include "sequencergui/widgets/collapsibletoolbar.h"

#include <QAction>
#include <QDebug>
#include <QSplitter>
#include <QToolBar>
#include <QVBoxLayout>

namespace sequencergui
{

CollapsibleWidget::CollapsibleWidget(QWidget *context, QWidget *parent)
    : QWidget(parent), m_tool_bar(new CollapsibleToolBar)
{
  auto layout = new QVBoxLayout(this);
  layout->setMargin(0);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(context);
  m_tool_bar->SetControlledWidget(this);
  m_tool_bar->SetText(context->windowTitle());

  // To show user actions in a tool bar.
  for (auto action : context->actions())
  {
    m_tool_bar->AddAction(action);
  }
}

CollapsibleWidget::~CollapsibleWidget() = default;

void CollapsibleWidget::AddToSplitter(QSplitter *splitter)
{
  splitter->addWidget(m_tool_bar);
  splitter->setCollapsible(splitter->indexOf(m_tool_bar), false);

  splitter->addWidget(this);
}

CollapsibleToolBar *CollapsibleWidget::GetToolBar()
{
  return m_tool_bar;
}

void AddToSplitter(QSplitter *splitter, QWidget *context)
{
  auto collapsible_widget = new CollapsibleWidget(context);
  collapsible_widget->AddToSplitter(splitter);
}

}  // namespace sequencergui
