/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#include "sequencergui/mainwindow/explorertoolbar.h"

#include "sequencergui/mainwindow/styleutils.h"

#include <QLabel>
#include <QToolButton>

namespace sequi
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
  button->setIcon(QIcon(":/icons/dots-horizontal.svg"));
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

}  // namespace sequi
