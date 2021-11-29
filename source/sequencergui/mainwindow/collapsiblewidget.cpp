/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#include "sequencergui/mainwindow/collapsiblewidget.h"

#include "sequencergui/mainwindow/explorernarrowtoolbar.h"

#include <QSplitter>

namespace sequi
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
