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

#include <mvvm/widgets/item_view_component_provider.h>

#include <QTreeView>
#include <QVBoxLayout>

namespace sequencergui
{

JobListWidget::JobListWidget(QWidget *parent)
    : QWidget(parent)
    , m_tree_view(new QTreeView)
    , m_component_provider(mvvm::CreateProvider<JobListViewModel>(m_tree_view))
{
  setWindowTitle("JOBS");
  setToolTip("List of current sequencer running/idle procedures");

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_tree_view);

  m_tree_view->setRootIsDecorated(false);
  m_tree_view->setHeaderHidden(true);

  connect(m_component_provider.get(), &mvvm::ItemViewComponentProvider::SelectedItemChanged, this,
          [this](auto) { emit JobSelected(GetSelectedJob()); });
}

JobListWidget::~JobListWidget() = default;

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
}

QTreeView *JobListWidget::GetTreeView()
{
  return m_tree_view;
}

mvvm::ViewModel *JobListWidget::GetViewModel()
{
  return m_component_provider->GetViewModel();
}

}  // namespace sequencergui
