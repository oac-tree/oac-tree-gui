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

#include "instruction_viewmodel.h"

#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/sequencer_item_helper.h>
#include <sequencergui/model/standard_instruction_items.h>

#include <mvvm/factories/viewmodel_controller_factory.h>
#include <mvvm/interfaces/row_strategy_interface.h>
#include <mvvm/viewmodel/standard_children_strategies.h>
#include <mvvm/viewmodel/viewitem_factory.h>
#include <mvvm/viewmodelbase/viewitem.h>

namespace sequencergui
{

class InstructionRowStrategy : public mvvm::RowStrategyInterface
{
public:
  QStringList GetHorizontalHeaderLabels() const override
  {
    static QStringList result = {"Type", "Name", "Status"};
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

    if (auto instruction = dynamic_cast<InstructionItem *>(item); instruction)
    {
      result.emplace_back(mvvm::CreateDataViewItem(GetNameItem(*instruction)));
      result.emplace_back(mvvm::CreateDataViewItem(GetStatusItem(*instruction)));
    }
    return result;
  }
};

InstructionViewModel::InstructionViewModel(mvvm::SessionModelInterface *model, QObject *parent)
    : ViewModel(parent)
{
  SetController(
      mvvm::factory::CreateController<mvvm::TopItemsStrategy, InstructionRowStrategy>(model, this));
}

}  // namespace sequencergui
