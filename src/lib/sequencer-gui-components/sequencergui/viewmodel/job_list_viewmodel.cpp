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

#include "job_list_viewmodel.h"

#include <sequencergui/model/item_constants.h>
#include <sequencergui/model/job_item.h>

#include <mvvm/factories/viewmodel_controller_factory.h>
#include <mvvm/interfaces/children_strategy_interface.h>
#include <mvvm/model/item_utils.h>
#include <mvvm/viewmodel/standard_row_strategies.h>
#include <mvvm/viewmodel/viewitem_factory.h>
#include <mvvm/viewmodelbase/viewitem.h>

namespace sequencergui
{

class JobRowStrategy : public mvvm::RowStrategyInterface
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

    if (auto procedure = dynamic_cast<JobItem *>(item); procedure)
    {
      result.emplace_back(mvvm::CreateDataViewItem(procedure->GetItem(itemconstants::kStatus)));
    }
    return result;
  }
};

class TopJobStrategy : public mvvm::ChildrenStrategyInterface
{
public:
  std::vector<mvvm::SessionItem *> GetChildren(const mvvm::SessionItem *item) const override
  {
    if (item && item->GetType() == JobItem::Type)
    {
      return {};
    }
    return item ? mvvm::utils::TopLevelItems(*item) : std::vector<mvvm::SessionItem *>();
  }
};

JobListViewModel::JobListViewModel(mvvm::SessionModelInterface *model, QObject *parent)
    : ViewModel(parent)
{
  SetController(mvvm::factory::CreateController<TopJobStrategy, JobRowStrategy>(model, this));
}

}  // namespace sequencergui
