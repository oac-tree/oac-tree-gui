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

#include "instruction_operation_viewmodel.h"

#include <sequencergui/model/sequencer_item_helper.h>
#include <sequencergui/model/standard_instruction_items.h>

#include <mvvm/factories/viewmodel_controller_factory.h>
#include <mvvm/interfaces/row_strategy_interface.h>
#include <mvvm/viewmodel/standard_children_strategies.h>
#include <mvvm/viewmodel/viewitem_factory.h>
#include <mvvm/viewmodelbase/viewitem.h>

namespace
{

/**
 * @brief Returns text representing instruction.
 */
std::string GetText(const sequencergui::InstructionItem &item)
{
  return item.GetName().empty() ? item.GetDisplayName() : item.GetName();
}

}  // namespace

namespace sequencergui
{

class InstructionOperationRowStrategy : public mvvm::RowStrategyInterface
{
public:
  QStringList GetHorizontalHeaderLabels() const override
  {
    static const QStringList result = {"Instruction", "Status", "BP"};
    return result;
  }

  std::vector<std::unique_ptr<mvvm::ViewItem>> ConstructRow(mvvm::SessionItem *item) override
  {
    std::vector<std::unique_ptr<mvvm::ViewItem>> result;

    if (!item)
    {
      return result;
    }

    if (auto instruction = dynamic_cast<InstructionItem *>(item); instruction)
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

InstructionOperationViewModel::InstructionOperationViewModel(mvvm::SessionModelInterface *model,
                                                             QObject *parent)
    : ViewModel(parent)
{
  SetController(
      mvvm::factory::CreateController<mvvm::TopItemsStrategy, InstructionOperationRowStrategy>(
          model, this));
}

int InstructionOperationViewModel::GetBreakpointColumn()
{
  return 2;
}

}  // namespace sequencergui
