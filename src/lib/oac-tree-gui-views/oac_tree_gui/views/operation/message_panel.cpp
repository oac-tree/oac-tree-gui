/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "message_panel.h"

#include <oac_tree_gui/jobsystem/job_log_severity.h>
#include <oac_tree_gui/jobsystem/job_utils.h>
#include <oac_tree_gui/style/style_helper.h>
#include <oac_tree_gui/viewmodel/job_log_viewmodel.h>

#include <sup/gui/widgets/custom_header_view.h>
#include <sup/gui/widgets/steady_menu.h>

#include <mvvm/editors/selectable_combobox_editor.h>

#include <QAction>
#include <QRegularExpression>
#include <QScrollBar>
#include <QSettings>
#include <QSortFilterProxyModel>
#include <QToolButton>
#include <QTreeView>
#include <QVBoxLayout>
#include <QWidgetAction>

namespace
{
const QString kGroupName("MessagePanel");
const QString kUncheckedSeveritiesSettingName = kGroupName + "/" + "unchecked";
const QString kHeaderStateSettingName = kGroupName + "/" + "header_state";

std::vector<oac_tree_gui::Severity> kSeveritiesToSelect = {
    oac_tree_gui::Severity::kEmergency, oac_tree_gui::Severity::kAlert,
    oac_tree_gui::Severity::kCritical,  oac_tree_gui::Severity::kError,
    oac_tree_gui::Severity::kWarning,   oac_tree_gui::Severity::kNotice,
    oac_tree_gui::Severity::kInfo,      oac_tree_gui::Severity::kDebug,
    oac_tree_gui::Severity::kTrace};

const std::vector<int> kDefaultColumnStretch({2, 2, 2, 1, 6});
}  // namespace

namespace oac_tree_gui
{

MessagePanel::MessagePanel(QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_tree_view(new QTreeView)
    , m_custom_header(
          new sup::gui::CustomHeaderView(kHeaderStateSettingName, kDefaultColumnStretch, this))
    , m_view_model(new JobLogViewModel(nullptr, this))
    , m_proxy_model(new QSortFilterProxyModel(this))
    , m_severity_selector_action(new QWidgetAction(this))
{
  setWindowTitle("LOG");
  ReadSettings();

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_tree_view);

  m_severity_selector_action->setDefaultWidget(CreateSeveritySelectorWidget().release());
  addAction(m_severity_selector_action);

  m_tree_view->setHeader(m_custom_header);
  m_tree_view->setAlternatingRowColors(true);
  m_tree_view->setRootIsDecorated(false);
  m_tree_view->setModel(m_proxy_model);
  m_tree_view->setSortingEnabled(true);

  m_proxy_model->setSourceModel(m_view_model);
  m_proxy_model->setFilterKeyColumn(2);

  SetupAutoscroll();
  UpdateSeverityFilter();
}

MessagePanel::~MessagePanel()
{
  WriteSettings();
}

void MessagePanel::SetLog(JobLog* job_log)
{
  m_view_model->SetLog(job_log);
}

void MessagePanel::ReadSettings()
{
  const QSettings settings;
  if (settings.contains(kUncheckedSeveritiesSettingName))
  {
    m_unchecked_severitites =
        settings.value(kUncheckedSeveritiesSettingName, true).value<QStringList>();
  }
  else
  {
    m_unchecked_severitites.push_back(
        QString::fromStdString(ToString(oac_tree_gui::Severity::kDebug)));
  }
}

void MessagePanel::WriteSettings()
{
  m_unchecked_severitites.clear();
  QSettings settings;
  // collecting the last status of checked severities
  for (auto [severity, status] : m_show_severity_flag)
  {
    if (!status)
    {
      m_unchecked_severitites.push_back(QString::fromStdString(ToString(severity)));
    }
  }

  settings.setValue(kUncheckedSeveritiesSettingName, m_unchecked_severitites);
}

std::unique_ptr<QWidget> MessagePanel::CreateSeveritySelectorWidget()
{
  m_severity_selector_menu = CreateSeveritySelectorMenu();

  auto result = std::make_unique<QToolButton>();
  result->setText("Severity");
  result->setIcon(FindIcon("cog-outline"));
  result->setPopupMode(QToolButton::InstantPopup);
  result->setToolButtonStyle(Qt::ToolButtonIconOnly);
  result->setMenu(m_severity_selector_menu.get());

  return result;
}

std::unique_ptr<sup::gui::SteadyMenu> MessagePanel::CreateSeveritySelectorMenu()
{
  auto result = std::make_unique<sup::gui::SteadyMenu>();

  for (auto severity : kSeveritiesToSelect)
  {
    auto name = QString::fromStdString(ToString(severity));
    auto action = result->addAction(name);

    m_show_severity_flag[severity] = !m_unchecked_severitites.contains(name);

    action->setCheckable(true);
    action->setChecked(!m_unchecked_severitites.contains(name));

    auto on_action = [this, severity, action]()
    {
      m_show_severity_flag[severity] = action->isChecked();
      UpdateSeverityFilter();
    };
    connect(action, &QAction::triggered, this, on_action);
  }

  return result;
}

void MessagePanel::SetupAutoscroll()
{
  auto on_row_about_to_be_inserted = [this](auto)
  {
    auto bar = m_tree_view->verticalScrollBar();
    m_tree_at_the_bottom = bar ? (bar->value() == bar->maximum()) : false;
  };
  connect(m_tree_view->model(), &QAbstractItemModel::rowsAboutToBeInserted, this,
          on_row_about_to_be_inserted);

  auto on_row_inserted = [this](auto)
  {
    if (m_tree_at_the_bottom)
    {
      m_tree_view->scrollToBottom();
    }
  };
  connect(m_tree_view->model(), &QAbstractItemModel::rowsInserted, this, on_row_inserted);
}

void MessagePanel::UpdateSeverityFilter()
{
  std::vector<std::string> names;

  for (const auto& [severity, flag] : m_show_severity_flag)
  {
    if (flag)
    {
      names.push_back(ToString(severity));
    }
  }

  const QRegularExpression regexp(QString::fromStdString(GetRegExpPattern(names)));
  m_proxy_model->setFilterRegularExpression(regexp);
}

}  // namespace oac_tree_gui
