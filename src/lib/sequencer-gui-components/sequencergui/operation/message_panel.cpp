/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "message_panel.h"

#include <sequencergui/jobsystem/job_log_severity.h>
#include <sequencergui/jobsystem/job_utils.h>
#include <sequencergui/viewmodel/job_log_viewmodel.h>
#include <sequencergui/widgets/steady_menu.h>
#include <sequencergui/widgets/tree_helper.h>
#include <sup/gui/widgets/custom_header_view.h>
#include <sup/gui/widgets/style_utils.h>

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

std::vector<sequencergui::Severity> kSeveritiesToSelect = {
    sequencergui::Severity::kEmergency, sequencergui::Severity::kAlert,
    sequencergui::Severity::kCritical,  sequencergui::Severity::kError,
    sequencergui::Severity::kWarning,   sequencergui::Severity::kNotice,
    sequencergui::Severity::kInfo,      sequencergui::Severity::kDebug,
    sequencergui::Severity::kTrace};

}  // namespace

namespace sequencergui
{

MessagePanel::MessagePanel(QWidget* parent)
    : QWidget(parent)
    , m_tree_view(new QTreeView)
    , m_custom_header(new sup::gui::CustomHeaderView(this))
    , m_view_model(new JobLogViewModel(nullptr))
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

void MessagePanel::showEvent(QShowEvent *event)
{
  if (m_custom_header->HasFavoriteState())
  {
    m_custom_header->RestoreFavoriteState();
  }
  else
  {
    AdjustWidthOfColumns(*m_tree_view, {2,2,2,1,6});
  }
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
        QString::fromStdString(SeverityToString(sequencergui::Severity::kDebug)));
  }

  if (settings.contains(kHeaderStateSettingName))
  {
    m_custom_header->SetAsFavoriteState(settings.value(kHeaderStateSettingName).toByteArray());
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
      m_unchecked_severitites.push_back(QString::fromStdString(SeverityToString(severity)));
    }
  }

  settings.setValue(kUncheckedSeveritiesSettingName, m_unchecked_severitites);

  if (m_custom_header->HasFavoriteState())
  {
    settings.setValue(kHeaderStateSettingName, m_custom_header->GetFavoriteState());
  }
}

std::unique_ptr<QWidget> MessagePanel::CreateSeveritySelectorWidget()
{
  m_severity_selector_menu = std::move(CreateSeveritySelectorMenu());

  auto result = std::make_unique<QToolButton>();
  result->setText("Severity");
  result->setIcon(sup::gui::utils::GetIcon("cog-outline.svg"));
  result->setPopupMode(QToolButton::InstantPopup);
  result->setToolButtonStyle(Qt::ToolButtonIconOnly);
  result->setMenu(m_severity_selector_menu.get());

  return result;
}

std::unique_ptr<SteadyMenu> MessagePanel::CreateSeveritySelectorMenu()
{
  auto result = std::make_unique<SteadyMenu>();

  for (auto severity : kSeveritiesToSelect)
  {
    auto name = QString::fromStdString(SeverityToString(severity));
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
      names.push_back(SeverityToString(severity));
    }
  }

  const QRegularExpression regexp(QString::fromStdString(GetRegExpPattern(names)));
  m_proxy_model->setFilterRegularExpression(regexp);
}

}  // namespace sequencergui
