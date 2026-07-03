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

#include <oac_tree_gui/model/job_item.h>
#include <oac_tree_gui/model/job_model.h>

#include <mvvm/utils/container_utils.h>

#include <sup/gui/views/dtoeditor/splittable_combo_panel.h>
#include <sup/gui/views/dtoeditor/splittable_editor_controller.h>

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

sup::gui::SplittableComboPanel* OperationSplittableWidget::CreatePanel()
{
  return m_splitter_controller->CreatePanel();
}

sup::gui::SplittableEditorController* OperationSplittableWidget::GetController() const
{
  return m_splitter_controller.get();
}

std::unique_ptr<sup::gui::SplittableEditorController>
OperationSplittableWidget::CreateSplitterController() const
{
  auto factory_func = [this]() -> std::unique_ptr<sup::gui::SplittableComboPanel>
  {
    auto main_editor = std::make_unique<OperationTabWidget>();

    auto item_list_callback = [this]() -> std::vector<mvvm::SessionItem*>
    {
      return m_model == nullptr
                 ? std::vector<mvvm::SessionItem*>()
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
