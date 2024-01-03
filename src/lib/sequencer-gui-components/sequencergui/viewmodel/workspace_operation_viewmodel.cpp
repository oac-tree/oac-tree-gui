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

#include "workspace_operation_viewmodel.h"

#include "custom_children_strategies.h"
#include "custom_row_strategies.h"

#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/factories/viewmodel_controller_factory.h>
#include <mvvm/model/sessionitem.h>
#include <mvvm/viewmodel/standard_children_strategies.h>
#include <mvvm/viewmodel/standard_row_strategies.h>
#include <mvvm/viewmodel/viewmodel_controller.h>
#include <mvvm/viewmodel/viewmodel_controller_impl.h>

namespace
{

/**
 * @brief Creates implementation for ViewModelController with custom children and row strategies.
 */
std::unique_ptr<mvvm::IViewModelController> CreateImpl(mvvm::ViewModelBase *viewmodel)
{
  auto children_strategy = std::make_unique<sequencergui::VariableTableChildrenStrategy>();
  auto row_strategy = std::make_unique<sequencergui::VariableTableRowStrategy>();

  auto result = std::make_unique<mvvm::ViewModelControllerImpl>(
      viewmodel, std::move(children_strategy), std::move(row_strategy));

  return result;
}

}  // namespace

namespace sequencergui
{

/**
 * @brief The WorkspaceOperationViewModelController class serves viewmodel to present
 * Workspace variables in a table.
 */

class WorkspaceOperationViewModelController : public mvvm::ViewModelController
{
public:
  explicit WorkspaceOperationViewModelController(mvvm::ViewModelBase *viewmodel)
      : mvvm::ViewModelController(CreateImpl(viewmodel))
  {
  }

  /**
   * @brief Processes AboutToRemoveItemEvent.
   *
   * @details If AnyValueItem is removed, we have to update the whole branch related to its parent,
   * VariableItem.
   */
  void OnModelEvent(const mvvm::AboutToRemoveItemEvent &event) override
  {
    auto [parent, tag_index] = event;
    if (auto child = dynamic_cast<sup::gui::AnyValueItem *>(parent->GetItem(tag_index)); child)
    {
      UpdateBranch(parent);  // update VariableItem row
    }
    else
    {
      mvvm::ViewModelController::OnModelEvent(event);
    }
  }

  /**
   * @brief Processes ItemInsertedEvent.
   *
   * @details If AnyValueItem is inserted, we have to update the whole branch related to its parent,
   * VariableItem.
   */
  void OnModelEvent(const mvvm::ItemInsertedEvent &event) override
  {
    auto [parent, tag_index] = event;
    if (auto child = dynamic_cast<sup::gui::AnyValueItem *>(parent->GetItem(tag_index)); child)
    {
      UpdateBranch(parent);  // update VariableItem row
    }
    else
    {
      mvvm::ViewModelController::OnModelEvent(event);
    }
  }

  /**
   * @brief Special update which regenerate the whole branch releated to the given item.
   *
   *@details It is used for the case when AnyValueItem on board of VariableItem was created after
   *the table was already constructed. We ask the WorkspaceItem to regenerate the whole branch
   *related to VariableItem.
   */
  void UpdateBranch(mvvm::SessionItem *item)
  {
    auto tag_index = item->GetTagIndex();

    // we pretend that this item was removed
    mvvm::ViewModelController::OnModelEvent(
        mvvm::AboutToRemoveItemEvent{item->GetParent(), tag_index});

    // we pretend that this item was inserted
    mvvm::ViewModelController::OnModelEvent(mvvm::ItemInsertedEvent{item->GetParent(), tag_index});
  }
};

WorkspaceOperationViewModel::WorkspaceOperationViewModel(mvvm::SessionModelInterface *model,
                                                         QObject *parent)
    : ViewModel(parent)
{
  auto controller = std::make_unique<WorkspaceOperationViewModelController>(this);
  controller->SetModel(model);
  SetController(std::move(controller));
}

int WorkspaceOperationViewModel::columnCount(const QModelIndex &parent) const
{
  return 4;  // "Name", "Value", "Type", "Channel"
}

}  // namespace sequencergui
