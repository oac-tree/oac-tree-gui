/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
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

#include <oac-tree-gui/model/item_constants.h>
#include <oac-tree-gui/model/job_item.h>
#include <oac-tree-gui/model/sequencer_item_helper.h>
#include <oac-tree-gui/model/standard_job_items.h>

#include <mvvm/model/item_utils.h>
#include <mvvm/viewmodel/abstract_row_strategy.h>
#include <mvvm/viewmodel/standard_children_strategies.h>
#include <mvvm/viewmodel/viewitem_factory.h>
#include <mvvm/viewmodel/viewmodel_controller_factory.h>
#include <mvvm/viewmodelbase/viewitem.h>

namespace sequencergui
{

class JobRowStrategy : public mvvm::AbstractRowStrategy
{
public:
  int GetSize() const override { return 2; }

  QStringList GetHorizontalHeaderLabels() const override
  {
    static const QStringList result = {"Name", "Status"};
    return result;
  }

private:
  std::vector<std::unique_ptr<mvvm::ViewItem>> ConstructRowImpl(mvvm::SessionItem *item) override
  {
    std::vector<std::unique_ptr<mvvm::ViewItem>> result;
    result.emplace_back(mvvm::CreateDisplayNameViewItem(item));
    result.emplace_back(mvvm::CreateDataViewItem(GetStatusItem(*item)));
    return result;
  }
};

class TopJobStrategy : public mvvm::FixedItemTypeStrategy
{
public:
  TopJobStrategy() : mvvm::FixedItemTypeStrategy(GetJobItemTypes()) {}
};

JobListViewModel::JobListViewModel(mvvm::ISessionModel *model, QObject *parent_object)
    : ViewModel(parent_object)
{
  SetController(mvvm::factory::CreateController<TopJobStrategy, JobRowStrategy>(model, this));
}

}  // namespace sequencergui
