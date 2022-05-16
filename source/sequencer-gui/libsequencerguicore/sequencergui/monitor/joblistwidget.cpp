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

#include "sequencergui/monitor/joblistwidget.h"

#include "sequencergui/components/itemviewcomponentprovider.h"
#include "sequencergui/model/jobitem.h"
#include "sequencergui/model/jobmodel.h"
#include "sequencergui/viewmodel/joblistviewmodel.h"
#include "sequencergui/utils/styleutils.h"

#include "mvvm/viewmodel/topitemsviewmodel.h"

#include <QAction>
#include <QTreeView>
#include <QVBoxLayout>

namespace sequencergui
{

JobListWidget::JobListWidget(QWidget *parent)
    : QWidget(parent)
    , m_submit_job_action(new QAction)
    , m_remove_selected_action(new QAction)
    , m_tree_view(new QTreeView)
    , m_component_provider(std::make_unique<ItemViewComponentProvider>(
          CreateViewModel<JobListViewModel>, m_tree_view))
{
  setWindowTitle("JOBS");
  setToolTip("List of current sequencer running/idle procedures");

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setMargin(0);
  layout->setSpacing(0);
  layout->addWidget(m_tree_view);

  SetupActions();

  m_tree_view->setRootIsDecorated(false);
  m_tree_view->setHeaderHidden(true);

  connect(m_component_provider.get(), &ItemViewComponentProvider::SelectedItemChanged, this,
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

void JobListWidget::SetupActions()
{
    m_submit_job_action->setIcon(StyleUtils::GetIcon("file-plus-outline.svg"));
//    connect(m_submit_job_action, &QAction::triggered, this,
//            &JobListWidget::createNewProcedureRequest);
    addAction(m_submit_job_action);

    m_remove_selected_action->setIcon(StyleUtils::GetIcon("beaker-remove-outline.svg"));
//    connect(m_remove_selected_action, &QAction::triggered, this,
//            &JobListWidget::onRemoveSelectedRequest);
    addAction(m_remove_selected_action);
}

}  // namespace sequencergui
