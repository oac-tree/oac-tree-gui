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

#include "job_list_widget.h"

#include <sequencergui/model/job_item.h>
#include <sequencergui/model/job_model.h>
#include <sequencergui/viewmodel/job_list_viewmodel.h>
#include <sup/gui/widgets/custom_header_view.h>

#include <mvvm/widgets/item_view_component_provider.h>

#include <QSettings>
#include <QTreeView>
#include <QVBoxLayout>

namespace
{
const QString kGroupName("JobListWidget");
const QString kHeaderStateSettingName = kGroupName + "/" + "header_state";
}  // namespace

namespace sequencergui
{

JobListWidget::JobListWidget(QWidget *parent)
    : QWidget(parent)
    , m_tree_view(new QTreeView)
    , m_component_provider(mvvm::CreateProvider<JobListViewModel>(m_tree_view))
    , m_custom_header(new sup::gui::CustomHeaderView(this))
{
  setWindowTitle("JOBS");
  setToolTip("List of current sequencer running/idle procedures");

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_tree_view);

  m_tree_view->setRootIsDecorated(false);
  m_tree_view->setHeader(m_custom_header);
  m_tree_view->setAlternatingRowColors(true);
  m_tree_view->header()->setStretchLastSection(true);

  connect(m_component_provider.get(), &mvvm::ItemViewComponentProvider::SelectedItemChanged, this,
          [this](auto) { emit JobSelected(GetSelectedJob()); });

  ReadSettings();
}

JobListWidget::~JobListWidget()
{
  WriteSettings();
}

void JobListWidget::SetJobModel(JobModel *model)
{
  m_model = model;
  m_component_provider->SetApplicationModel(model);
}

JobItem *JobListWidget::GetSelectedJob()
{
  return m_component_provider->GetSelected<JobItem>();
}

void JobListWidget::SetSelectedJob(JobItem *job)
{
  m_component_provider->SetSelectedItem(job);
  AdjustColumnWidth();
}

QTreeView *JobListWidget::GetTreeView()
{
  return m_tree_view;
}

mvvm::ViewModel *JobListWidget::GetViewModel()
{
  return m_component_provider->GetViewModel();
}

void JobListWidget::ReadSettings()
{
  const QSettings settings;
  if (settings.contains(kHeaderStateSettingName))
  {
    m_custom_header->SetAsFavoriteState(settings.value(kHeaderStateSettingName).toByteArray());
  }
}

void JobListWidget::WriteSettings()
{
  QSettings settings;
  if (m_custom_header->HasFavoriteState())
  {
    settings.setValue(kHeaderStateSettingName, m_custom_header->GetFavoriteState());
  }
}

void JobListWidget::AdjustColumnWidth()
{
  if (m_custom_header->HasFavoriteState())
  {
    m_custom_header->RestoreFavoriteState();
  }
  else
  {
    m_tree_view->resizeColumnToContents(0);
  }
}

}  // namespace sequencergui
