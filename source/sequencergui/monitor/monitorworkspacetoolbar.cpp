/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#include "sequencergui/monitor/monitorworkspacetoolbar.h"

#include "sequencergui/mainwindow/styleutils.h"

#include <QAction>
#include <QLabel>
#include <QMenu>
#include <QPushButton>
#include <QToolButton>

namespace sequi
{
MonitorWorkspaceToolBar::MonitorWorkspaceToolBar(QWidget *parent) : QToolBar(parent)
{
  setIconSize(StyleUtils::ToolBarIconSize());

  AddDotsMenu();
}

MonitorWorkspaceToolBar::~MonitorWorkspaceToolBar() = default;

void MonitorWorkspaceToolBar::AddDotsMenu()
{
  InsertStrech();

  auto button = new QToolButton;
  button->setIcon(QIcon(":/icons/dots-horizontal.svg"));
  button->setIconSize(StyleUtils::ToolBarIconSize());
  addWidget(button);
}

void MonitorWorkspaceToolBar::InsertStrech()
{
  auto empty = new QWidget(this);
  empty->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  addWidget(empty);
}

}  // namespace sequi
