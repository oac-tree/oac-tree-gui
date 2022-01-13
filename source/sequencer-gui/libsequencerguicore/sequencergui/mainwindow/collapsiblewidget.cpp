/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Creation, execution and monitoring of the Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/mainwindow/collapsiblewidget.h"

#include "sequencergui/mainwindow/explorernarrowtoolbar.h"

#include <QSplitter>

namespace sequencergui
{

CollapsibleWidget::CollapsibleWidget(QWidget *parent)
    : QWidget(parent), m_tool_bar(new ExplorerNarrowToolBar)
{
  m_tool_bar->SetControlledWidget(this);
}

CollapsibleWidget::~CollapsibleWidget() = default;

void CollapsibleWidget::AddToSplitter(QSplitter *splitter)
{
  splitter->addWidget(m_tool_bar);
  splitter->setCollapsible(splitter->indexOf(m_tool_bar), false);

  splitter->addWidget(this);
}

ExplorerNarrowToolBar *CollapsibleWidget::GetToolBar()
{
  return m_tool_bar;
}

void CollapsibleWidget::SetText(const QString &text)
{
  m_tool_bar->SetText(text);
}

}  // namespace sequi
