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

#include "workspace_editor_viewmodel.h"

#include <sequencergui/model/sequencer_item_helper.h>
#include <sequencergui/model/standard_variable_items.h>
#include <sequencergui/model/workspace_item.h>

#include <mvvm/factories/viewmodel_controller_factory.h>
#include <mvvm/interfaces/children_strategy_interface.h>
#include <mvvm/interfaces/row_strategy_interface.h>
#include <mvvm/utils/container_utils.h>
#include <mvvm/viewmodel/viewitem_factory.h>
#include <mvvm/viewmodelbase/viewitem.h>

#include <algorithm>

namespace
{

/**
 * @brief Returns copy of container, without elements in the exclude list.
 */

template <typename T, typename F>
T FilterElements(const T &container, const F &to_exclude)
{
  T result;

  auto not_in_exclude_list = [&to_exclude](const auto &x)
  { return !mvvm::utils::Contains(to_exclude, x); };

  std::copy_if(container.begin(), container.end(), std::back_inserter(result), not_in_exclude_list);

  return result;
}

}  // namespace

namespace sequencergui
{

/**
 * @brief The VariableRowStrategy generates the row of two elements representing a Variable.
 *
 * @details For VariableItem itself it will generate a row with [display_name, editable_name].
 * For everything beneath it it will return usual [property_name, value] pair.
 */

class VariableRowStrategy : public mvvm::RowStrategyInterface
{
public:
  QStringList GetHorizontalHeaderLabels() const override
  {
    static QStringList result = {"Type", "Name"};
    return result;
  }

  std::vector<std::unique_ptr<mvvm::ViewItem>> ConstructRow(mvvm::SessionItem *item) override
  {
    std::vector<std::unique_ptr<mvvm::ViewItem>> result;

    if (!item)
    {
      return result;
    }

    result.emplace_back(mvvm::CreateDisplayNameViewItem(item));

    if (auto variable = dynamic_cast<VariableItem *>(item); variable)
    {
      result.emplace_back(mvvm::CreateDataViewItem(GetNameItem(*variable)));
    }
    else
    {
      result.emplace_back(mvvm::CreateDataViewItem(item));
    }

    return result;
  }
};

/**
 * @brief The VariableChildrenStrategy class reports which items should be in the tree representing
 * WorkspaceItem.
 *
 * @details It allows all children items to appear in the tree, except PropertyItem representing the
 * name and IsAvailable status.
 */
class VariableChildrenStrategy : public mvvm::ChildrenStrategyInterface
{
public:
  std::vector<mvvm::SessionItem *> GetChildren(const mvvm::SessionItem *item) const override
  {
    if (!item)
    {
      return {};
    }

    std::vector<mvvm::SessionItem *> to_exclude = {GetNameItem(*item), GetIsAvailableItem(*item)};

    auto result = FilterElements(item->GetAllItems(), to_exclude);

    return result;
  }
};

WorkspaceEditorViewModel::WorkspaceEditorViewModel(mvvm::SessionModelInterface *model,
                                                   QObject *parent)
    : ViewModel(parent)
{
  SetController(
      mvvm::factory::CreateController<VariableChildrenStrategy, VariableRowStrategy>(model, this));
}

}  // namespace sequencergui
