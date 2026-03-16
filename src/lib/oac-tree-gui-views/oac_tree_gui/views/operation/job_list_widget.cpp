/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "job_list_widget.h"

#include <oac_tree_gui/model/job_item.h>
#include <oac_tree_gui/model/job_model.h>
#include <oac_tree_gui/viewmodel/job_list_viewmodel.h>

#include <sup/gui/widgets/custom_header_view.h>
#include <mvvm/model/item_selection.h>

#include <mvvm/views/component_provider_helper.h>

#include <QSettings>
#include <QTreeView>
#include <QVBoxLayout>

namespace
{
const QString kHeaderStateSettingName = "JobListWidget/header_state";
}  // namespace

namespace oac_tree_gui
{

JobListWidget::JobListWidget(QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_tree_view(new QTreeView)
    , m_component_provider(mvvm::CreateProvider<JobListViewModel>(m_tree_view))
    , m_custom_header(new sup::gui::CustomHeaderView(kHeaderStateSettingName, this))
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

  // configure tree to allow internal move of procedures (handled by FlatListViewModel)
  m_tree_view->setDragEnabled(true);
  m_tree_view->setDropIndicatorShown(true);
  m_tree_view->setDefaultDropAction(Qt::MoveAction);
  m_tree_view->setDragDropMode(QAbstractItemView::DragDrop);

  connect(m_component_provider.get(), &mvvm::ItemViewComponentProvider::SelectionChanged, this,
          [this](const auto&) { emit JobSelected(GetSelectedJob()); });
}

JobListWidget::~JobListWidget() = default;

void JobListWidget::SetJobModel(JobModel* model)
{
  m_model = model;
  m_component_provider->SetApplicationModel(model);
}

JobItem* JobListWidget::GetSelectedJob()
{
  auto selection = m_component_provider->GetSelection();
  return const_cast<JobItem*>(selection.GetSelected<JobItem>());
}

void JobListWidget::SetSelectedJob(JobItem* job)
{
  m_component_provider->SetSelection(mvvm::ItemSelection(job));
  m_custom_header->AdjustColumnsWidth();
}

QTreeView* JobListWidget::GetTreeView()
{
  return m_tree_view;
}

mvvm::ViewModel* JobListWidget::GetViewModel()
{
  return m_component_provider->GetViewModel();
}

}  // namespace oac_tree_gui
