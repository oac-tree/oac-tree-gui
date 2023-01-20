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

#include "sequencergui/monitor/message_panel.h"

#include <mvvm/editors/selectable_combobox_editor.h>
#include <sequencergui/jobsystem/job_log_severity.h>
#include <sequencergui/utils/style_utils.h>
#include <sequencergui/viewmodel/job_log_viewmodel.h>

#include <QAction>
#include <QDebug>
#include <QMenu>
#include <QRegularExpression>
#include <QSortFilterProxyModel>
#include <QToolButton>
#include <QTreeView>
#include <QVBoxLayout>
#include <QWidgetAction>

namespace
{
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
    , m_remove_selected_action(new QAction(this))
    , m_tree_view(new QTreeView)
    , m_view_model(new JobLogViewModel(nullptr))
    , m_proxy_model(new QSortFilterProxyModel)
    , m_test_action(new QWidgetAction(this))
    , m_selection_menu(CreateSeverityChoiceMenu())
{
  setWindowTitle("LOG");

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_tree_view);

  m_test_action->setDefaultWidget(CreateSeveritySelectionWidget().release());
  addAction(m_test_action);

  m_tree_view->setAlternatingRowColors(true);
  m_tree_view->setRootIsDecorated(false);
  m_tree_view->setModel(m_proxy_model);
  m_tree_view->setSortingEnabled(true);

  m_proxy_model->setSourceModel(m_view_model);
  m_proxy_model->setFilterKeyColumn(2);
  //  QRegularExpression regexp("\b(?:WARNING|DEBUG)\b");
}

MessagePanel::~MessagePanel() = default;

void MessagePanel::SetLog(JobLog* job_log)
{
  m_view_model->SetLog(job_log);
}

std::unique_ptr<QWidget> MessagePanel::CreateSeveritySelectionWidget()
{
  auto result = std::make_unique<QToolButton>();
  result->setText("Severity");
  result->setIcon(styleutils::GetIcon("cog-outline.svg"));
  result->setPopupMode(QToolButton::InstantPopup);
  result->setToolButtonStyle(Qt::ToolButtonIconOnly);
  result->setMenu(m_selection_menu.get());

  return result;
}

std::unique_ptr<QMenu> MessagePanel::CreateSeverityChoiceMenu()
{
  auto result = std::make_unique<QMenu>();

  for (auto severity : kSeveritiesToSelect)
  {
    auto name = QString::fromStdString(SeverityToString(severity));
    m_show_severity_flag[name] = true;
    auto action = result->addAction(name);
    action->setCheckable(true);
    action->setChecked(true);
    auto on_action = [this, name, action]()
    {
      m_show_severity_flag[name] = action->isChecked();
      UpdateSeverityFilter();
    };
    connect(action, &QAction::triggered, this, on_action);
  }

  return result;
}

void MessagePanel::UpdateSeverityFilter()
{
  QString pattern;
  for (auto [severity, flag] : m_show_severity_flag)
  {
    qDebug() << severity << flag;
    if (flag)
    {
      if (!pattern.isEmpty())
      {
        pattern.append("|");
      }
      pattern.append(severity);
    }
  }

  QRegularExpression regexp(QString("(%1)").arg(pattern));
  m_proxy_model->setFilterRegularExpression(regexp);

  qDebug() << "severity change " << pattern;
}

}  // namespace sequencergui
