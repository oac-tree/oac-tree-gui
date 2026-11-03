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

#include "workspace_operation_viewmodel.h"

#include <oac_tree_gui/components/custom_children_strategies.h>
#include <oac_tree_gui/components/custom_row_strategies.h>
#include <oac_tree_gui/model/variable_item.h>

#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/model/item_utils.h>
#include <mvvm/model/session_item.h>
#include <mvvm/providers/viewmodel_controller.h>

namespace oac_tree_gui
{

namespace
{

//! Returns the variable owning the AnyValueItem at the given position, or nullptr.
VariableItem* FindVariableOfAnyValueItem(mvvm::SessionItem* parent, const mvvm::TagIndex& tag_index)
{
  if (parent->FindItem<sup::gui::AnyValueItem>(tag_index) == nullptr)
  {
    return nullptr;
  }
  return mvvm::utils::FindItemUp<VariableItem>(parent);
}

}  // namespace

/**
 * @brief The WorkspaceOperationViewModelController class serves viewmodel to present
 * Workspace variables in a table.
 *
 * The row of a variable depends on its AnyValueItem branch. Any insert or remove in this branch
 * rebuilds the variable row.
 */
class WorkspaceOperationViewModelController : public mvvm::ViewModelController
{
public:
  explicit WorkspaceOperationViewModelController(mvvm::ViewModelBase* viewmodel)
      : mvvm::ViewModelController(viewmodel,
                                  std::make_unique<oac_tree_gui::VariableTableChildrenStrategy>(),
                                  std::make_unique<oac_tree_gui::VariableTableRowStrategy>())
  {
  }

  /**
   * @brief Processes ItemInsertedEvent.
   *
   * @details An AnyValueItem inserted into a variable branch rebuilds the variable row.
   */
  void OnItemInsertedEvent(const mvvm::ItemInsertedEvent& event) override
  {
    auto [parent, tag_index] = event;
    if (auto* variable = FindVariableOfAnyValueItem(parent, tag_index); variable != nullptr)
    {
      RemoveVariableRow(*variable);
      InsertVariableRow(*variable);
    }
    else
    {
      mvvm::ViewModelController::OnItemInsertedEvent(event);
    }
  }

  /**
   * @brief Processes AboutToRemoveItemEvent.
   *
   * @details An AnyValueItem about to be removed from a variable branch removes the variable row.
   * The row is restored in OnItemRemovedEvent.
   */
  void OnAboutToRemoveItemEvent(const mvvm::AboutToRemoveItemEvent& event) override
  {
    auto [parent, tag_index] = event;
    if (auto* variable = FindVariableOfAnyValueItem(parent, tag_index); variable != nullptr)
    {
      RemoveVariableRow(*variable);
    }
    else
    {
      mvvm::ViewModelController::OnAboutToRemoveItemEvent(event);
    }
  }

  /**
   * @brief Processes ItemRemovedEvent.
   *
   * @details Restores the row of a variable, removed in OnAboutToRemoveItemEvent.
   */
  void OnItemRemovedEvent(const mvvm::ItemRemovedEvent& event) override
  {
    auto* variable = mvvm::utils::FindItemUp<VariableItem>(event.item);
    if ((variable != nullptr) && (GetViewItemMap().FindView(variable) == nullptr))
    {
      InsertVariableRow(*variable);
    }
  }

private:
  //! Removes the row of the variable.
  void RemoveVariableRow(VariableItem& variable)
  {
    mvvm::ViewModelController::OnAboutToRemoveItemEvent(
        mvvm::AboutToRemoveItemEvent{variable.GetParent(), variable.GetTagIndex()});
  }

  //! Inserts the row of the variable.
  void InsertVariableRow(VariableItem& variable)
  {
    mvvm::ViewModelController::OnItemInsertedEvent(
        mvvm::ItemInsertedEvent{variable.GetParent(), variable.GetTagIndex()});
  }
};

WorkspaceOperationViewModel::WorkspaceOperationViewModel(mvvm::ISessionModel* model,
                                                         QObject* parent_object)
    : ViewModel(parent_object)
{
  auto controller = std::make_unique<WorkspaceOperationViewModelController>(this);
  controller->SetModel(model);
  SetController(std::move(controller));
}

int WorkspaceOperationViewModel::columnCount(const QModelIndex& parent) const
{
  (void)parent;
  return 4;  // "Name", "Value", "Type", "Channel"
}

}  // namespace oac_tree_gui
