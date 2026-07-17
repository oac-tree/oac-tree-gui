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

#include "operation_splittable_widget.h"

#include "operation_tab_widget.h"

#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/job_model.h>

#include <sup/gui/views/dtoeditor/splittable_combo_panel.h>
#include <sup/gui/views/dtoeditor/splittable_editor_controller.h>

#include <mvvm/utils/container_utils.h>

#include <QSplitter>
#include <QVBoxLayout>

namespace oac_tree_gui
{

OperationSplittableWidget::OperationSplittableWidget(QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_splitter(new QSplitter)
    , m_splitter_controller(CreateSplitterController())
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_splitter);

  connect(m_splitter_controller.get(),
          &sup::gui::SplittableEditorController::focusPanelSelectionChanged, this,
          &OperationSplittableWidget::focusWidgetJobSelectionChanged);

  // track changes of the active-jobs set (wired before the first panel is created below)
  connect(m_splitter_controller.get(), &sup::gui::SplittableEditorController::newPanelAdded, this,
          &OperationSplittableWidget::OnPanelAdded);
  connect(m_splitter_controller.get(), &sup::gui::SplittableEditorController::aboutToClosePanel,
          this, [this](sup::gui::SplittableComboPanel* panel)
          { emit activeJobsChanged(CollectActiveJobs(panel)); });

  CreatePanel();
}

OperationSplittableWidget::~OperationSplittableWidget() = default;

void OperationSplittableWidget::SetModel(JobModel* model)
{
  m_model = model;
  m_splitter_controller->SetModel(model);
}

void OperationSplittableWidget::SetCurrentJob(JobItem* job_item)
{
  if (auto focus_widget = m_splitter_controller->GetFocusWidget(); focus_widget)
  {
    focus_widget->SetItem(job_item);
  }
}

void OperationSplittableWidget::SetJobLog(JobItem* job, JobLog* job_log)
{
  for (auto panel : m_splitter_controller->GetPanels())
  {
    if (panel->GetCurrentItem() == job)
    {
      if (auto tab_widget = panel->GetMainEditor<OperationTabWidget>(); tab_widget)
      {
        tab_widget->SetJobLog(job_log);
      }
    }
  }
}

void OperationSplittableWidget::SetSelectedInstructions(
    JobItem* job, const std::vector<const InstructionItem*>& items)
{
  for (auto panel : m_splitter_controller->GetPanels())
  {
    if (panel->GetCurrentItem() == job)
    {
      if (auto tab_widget = panel->GetMainEditor<OperationTabWidget>(); tab_widget)
      {
        tab_widget->SetSelectedInstructions(items);
      }
    }
  }
}

void OperationSplittableWidget::SetTickTimeout(JobItem* job, int msec)
{
  for (auto panel : m_splitter_controller->GetPanels())
  {
    if (panel->GetCurrentItem() == job)
    {
      if (auto tab_widget = panel->GetMainEditor<OperationTabWidget>(); tab_widget)
      {
        tab_widget->SetCurrentTickTimeout(msec);
      }
    }
  }
}

int OperationSplittableWidget::GetCurrentTickTimeout()
{
  if (auto tab_widget = GetFocusTabWidget(); tab_widget)
  {
    return tab_widget->GetCurrentTickTimeout();
  }
  return 0;
}

sup::gui::SplittableComboPanel* OperationSplittableWidget::CreatePanel()
{
  return m_splitter_controller->CreatePanel();
}

sup::gui::SplittableEditorController* OperationSplittableWidget::GetController() const
{
  return m_splitter_controller.get();
}

OperationTabWidget* OperationSplittableWidget::GetFocusTabWidget() const
{
  auto focus_widget = m_splitter_controller->GetFocusWidget();
  return focus_widget == nullptr ? nullptr : focus_widget->GetMainEditor<OperationTabWidget>();
}

std::vector<JobItem*> OperationSplittableWidget::GetActiveJobs() const
{
  return CollectActiveJobs();
}

std::vector<JobItem*> OperationSplittableWidget::CollectActiveJobs(
    const sup::gui::SplittableComboPanel* excluded) const
{
  std::vector<JobItem*> result;
  for (auto panel : m_splitter_controller->GetPanels())
  {
    if (panel == excluded)
    {
      continue;
    }
    if (auto job = dynamic_cast<JobItem*>(panel->GetCurrentItem()); job != nullptr)
    {
      result.push_back(job);
    }
  }
  return result;
}

void OperationSplittableWidget::OnPanelAdded(sup::gui::SplittableComboPanel* panel)
{
  connect(panel, &sup::gui::SplittableComboPanel::itemSelectionChanged, this,
          [this](const mvvm::ItemSelection&) { emit activeJobsChanged(CollectActiveJobs()); });

  emit activeJobsChanged(CollectActiveJobs());
}

std::unique_ptr<sup::gui::SplittableEditorController>
OperationSplittableWidget::CreateSplitterController() const
{
  auto factory_func = [this]() -> std::unique_ptr<sup::gui::SplittableComboPanel>
  {
    auto main_editor = std::make_unique<OperationTabWidget>();

    // re-emit the panel's toolbar signals through the splittable widget, so that a single
    // set of connections in the parent view serves all panels
    auto* tab_widget = main_editor.get();
    connect(tab_widget, &OperationTabWidget::RunRequest, this,
            &OperationSplittableWidget::RunRequest);
    connect(tab_widget, &OperationTabWidget::PauseRequest, this,
            &OperationSplittableWidget::PauseRequest);
    connect(tab_widget, &OperationTabWidget::StepRequest, this,
            &OperationSplittableWidget::StepRequest);
    connect(tab_widget, &OperationTabWidget::StopRequest, this,
            &OperationSplittableWidget::StopRequest);
    connect(tab_widget, &OperationTabWidget::ResetRequest, this,
            &OperationSplittableWidget::ResetRequest);
    connect(tab_widget, &OperationTabWidget::ChangeDelayRequest, this,
            &OperationSplittableWidget::ChangeDelayRequest);
    connect(tab_widget, &OperationTabWidget::ToggleBreakpointRequest, this,
            &OperationSplittableWidget::ToggleBreakpointRequest);

    auto item_list_callback = [this]() -> std::vector<mvvm::SessionItem*>
    {
      return m_model == nullptr ? std::vector<mvvm::SessionItem*>()
                                : mvvm::utils::CastItems<mvvm::SessionItem>(m_model->GetJobItems());
    };

    auto result = std::make_unique<sup::gui::SplittableComboPanel>(std::move(main_editor),
                                                                   item_list_callback);
    result->SetModel(m_model);
    return result;
  };
  return std::make_unique<sup::gui::SplittableEditorController>(m_splitter, factory_func);
}

}  // namespace oac_tree_gui
