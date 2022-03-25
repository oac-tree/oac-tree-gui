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

#include "sequencergui/monitor/procedureviewmodel.h"

#include "sequencergui/model/item_constants.h"
#include "sequencergui/model/sequenceritems.h"

#include "mvvm/factories/viewmodelcontrollerfactory.h"
#include "mvvm/model/applicationmodel.h"
#include "mvvm/viewmodel/standardchildrenstrategies.h"
#include "mvvm/viewmodel/standardrowstrategies.h"
#include "mvvm/viewmodel/viewitemfactory.h"
#include "mvvm/viewmodelbase/viewitem.h"

namespace sequencergui
{

class ProcedureRowStrategy : public mvvm::RowStrategyInterface
{
public:
  QStringList GetHorizontalHeaderLabels() const override
  {
    static QStringList result = {"Type", "Status"};
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

    if (auto instruction = dynamic_cast<ProcedureItem *>(item); instruction)
    {
      result.emplace_back(mvvm::CreateDataViewItem(instruction->GetItem(ItemConstants::kStatus)));
    }
    return result;
  }
};

ProcedureViewModel::ProcedureViewModel(mvvm::ApplicationModel *model, QObject *parent)
    : ViewModel(parent)
{
  SetController(
      mvvm::factory::CreateController<mvvm::TopItemsStrategy, ProcedureRowStrategy>(model, this));
}

}  // namespace sequencergui
