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

#include "suppvmonitor/monitor_widget_toolbar.h"

#include <sequencergui/domain/domain_utils.h>
#include <sequencergui/utils/style_utils.h>

#include <mvvm/widgets/widget_utils.h>

#include <sup/gui/widgets/style_utils.h>

#include <QMenu>
#include <QToolButton>

namespace suppvmonitor
{

MonitorWidgetToolBar::MonitorWidgetToolBar(QWidget *parent)
    : QToolBar(parent)
    , m_add_variable_menu(CreateAddVariableMenu())
    , m_add_button(new QToolButton)
    , m_edit_anyvalue_button(new QToolButton)
    , m_remove_button(new QToolButton)
    , m_start_button(new QToolButton)
    , m_stop_button(new QToolButton)
{
  setIconSize(sup::gui::utils::ToolBarIconSize());

  auto font = m_start_button->font();
  auto psize = font.pointSize() * 1.05;
  font.setPointSize(psize);

  using sequencergui::styleutils::GetIcon;

  m_add_button->setText("Add");
  m_add_button->setIcon(GetIcon("plus-circle-outline.svg"));
  m_add_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  m_add_button->setPopupMode(QToolButton::InstantPopup);
  m_add_button->setMenu(m_add_variable_menu.get());
  m_add_button->setToolTip(
      "Add sequencer variable to the workspace\n\n"
      "- If existing variable is selected, new variable\n"
      "  will be inserted after");
  addWidget(m_add_button);

  m_edit_anyvalue_button->setText("Edit");
  m_edit_anyvalue_button->setIcon(GetIcon("file-tree-outline.svg"));
  m_edit_anyvalue_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  m_edit_anyvalue_button->setToolTip("Edit value of currently selected variable");
  connect(m_edit_anyvalue_button, &QToolButton::clicked, this,
          &MonitorWidgetToolBar::EditAnyvalueRequest);
  addWidget(m_edit_anyvalue_button);

  m_remove_button->setText("Remove");
  m_remove_button->setIcon(GetIcon("beaker-remove-outline.svg"));
  m_remove_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  m_remove_button->setToolTip("Remove currently selected variable");
  connect(m_remove_button, &QToolButton::clicked, this,
          &MonitorWidgetToolBar::RemoveVariableRequest);
  addWidget(m_remove_button);

  addSeparator();

  m_start_button->setText("Start");
  m_start_button->setFont(font);
  m_start_button->setIcon(GetIcon("chevron-right-circle-outline.svg"));
  m_start_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  m_start_button->setToolTip("Start monitoring workspace variables");
  connect(m_start_button, &QToolButton::clicked, this,
          &MonitorWidgetToolBar::StartMonitoringRequest);
  addWidget(m_start_button);

  m_stop_button->setText("Stop");
  m_stop_button->setFont(font);
  m_stop_button->setIcon(GetIcon("stop-circle-outline.svg"));
  m_stop_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  m_stop_button->setToolTip("Stop monitoring");
  connect(m_stop_button, &QToolButton::clicked, this, &MonitorWidgetToolBar::StopMonitoringRequest);
  addWidget(m_stop_button);
}

MonitorWidgetToolBar::~MonitorWidgetToolBar() = default;

std::unique_ptr<QMenu> MonitorWidgetToolBar::CreateAddVariableMenu()
{
  auto result = std::make_unique<QMenu>();
  result->setToolTipsVisible(true);

  auto names = mvvm::utils::GetStringList(sequencergui::GetDomainVariableNames());
  for (const auto &name : names)
  {
    auto action = result->addAction(name);
    auto on_action = [this, name]() { emit AddVariableRequest(name); };
    connect(action, &QAction::triggered, this, on_action);
  }

  return result;
}

void MonitorWidgetToolBar::InsertStrech()
{
  auto empty = new QWidget(this);
  empty->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  addWidget(empty);
}

}  // namespace suppvmonitor
