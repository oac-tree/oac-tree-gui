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

#include "sequencergui/widgets/collapsiblewidget_v2.h"

#include "sequencergui/widgets/collapsibletoolbar_v2.h"

#include <QSplitter>

namespace sequencergui
{

CollapsibleWidgetV2::CollapsibleWidgetV2(QWidget *parent)
    : QWidget(parent), m_tool_bar(new CollapsibleToolBarV2)
{
  m_tool_bar->SetControlledWidget(this);
}

CollapsibleWidgetV2::~CollapsibleWidgetV2() = default;

void CollapsibleWidgetV2::AddToSplitter(QSplitter *splitter)
{
  splitter->addWidget(m_tool_bar);
  splitter->setCollapsible(splitter->indexOf(m_tool_bar), false);

  splitter->addWidget(this);
}

CollapsibleToolBarV2 *CollapsibleWidgetV2::GetToolBar()
{
  return m_tool_bar;
}

void CollapsibleWidgetV2::SetText(const QString &text)
{
  m_tool_bar->SetText(text);
}

}  // namespace sequencergui
