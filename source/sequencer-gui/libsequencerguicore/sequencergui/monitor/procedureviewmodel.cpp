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
#include "mvvm/interfaces/childrenstrategyinterface.h"
#include "mvvm/model/applicationmodel.h"
#include "mvvm/model/itemutils.h"
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

    if (auto procedure = dynamic_cast<ProcedureItem *>(item); procedure)
    {
      result.emplace_back(mvvm::CreateDataViewItem(procedure->GetItem(ItemConstants::kStatus)));
    }
    return result;
  }
};

class TopProcedureStrategy : public mvvm::ChildrenStrategyInterface
{
public:
  std::vector<mvvm::SessionItem *> GetChildren(const mvvm::SessionItem *item) const override
  {
    if (item && item->GetType() == ProcedureItem::Type)
    {
      return {};
    }
    return item ? mvvm::utils::TopLevelItems(*item) : std::vector<mvvm::SessionItem *>();
  }
};

ProcedureViewModel::ProcedureViewModel(mvvm::ApplicationModel *model, QObject *parent)
    : ViewModel(parent)
{
  SetController(
      mvvm::factory::CreateController<TopProcedureStrategy, ProcedureRowStrategy>(model, this));
}

}  // namespace sequencergui
