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

#include "instruction_operation_viewmodel.h"

#include <oac_tree_gui/model/sequencer_item_helper.h>
#include <oac_tree_gui/model/standard_instruction_items.h>

#include <mvvm/providers/abstract_row_strategy.h>
#include <mvvm/providers/standard_children_strategies.h>
#include <mvvm/providers/viewitem.h>
#include <mvvm/providers/viewitem_factory.h>
#include <mvvm/providers/viewmodel_controller_factory.h>

namespace
{

/**
 * @brief Returns text representing instruction.
 */
std::string GetText(const oac_tree_gui::InstructionItem& item)
{
  return item.GetName().empty() ? item.GetDisplayName() : item.GetName();
}

}  // namespace

namespace oac_tree_gui
{

class InstructionOperationRowStrategy : public mvvm::AbstractRowStrategy
{
public:
  std::size_t GetSize() const override { return 3U; }

  QStringList GetHorizontalHeaderLabels() const override
  {
    static const QStringList result = {"Instruction", "Status", "BP"};
    return result;
  }

private:
  std::vector<std::unique_ptr<mvvm::ViewItem>> ConstructRowImpl(mvvm::SessionItem* item) override
  {
    std::vector<std::unique_ptr<mvvm::ViewItem>> result;

    if (auto instruction = dynamic_cast<InstructionItem*>(item); instruction)
    {
      result.emplace_back(mvvm::CreateLabelViewItem(instruction, GetText(*instruction)));
      result.emplace_back(mvvm::CreateDataViewItem(GetStatusItem(*instruction)));
      result.emplace_back(mvvm::CreateDataViewItem(GetBreakpointItem(*instruction)));
    }
    else
    {
      result.emplace_back(mvvm::CreateDisplayNameViewItem(item));
      result.emplace_back(mvvm::CreateLabelViewItem(item));
      result.emplace_back(mvvm::CreateLabelViewItem(item));
    }
    return result;
  }
};

InstructionOperationViewModel::InstructionOperationViewModel(mvvm::ISessionModel* model,
                                                             QObject* parent_object)
    : ViewModel(parent_object)
{
  SetController(
      mvvm::factory::CreateController<mvvm::TopItemsStrategy, InstructionOperationRowStrategy>(
          model, this));
}

int InstructionOperationViewModel::GetBreakpointColumn()
{
  return 2;
}

}  // namespace oac_tree_gui
