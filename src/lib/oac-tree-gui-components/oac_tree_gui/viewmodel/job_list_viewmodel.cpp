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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "job_list_viewmodel.h"

#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/model/job_item.h>
#include <oac_tree_gui/model/sequencer_item_helper.h>
#include <oac_tree_gui/model/standard_job_items.h>

#include <mvvm/model/item_utils.h>
#include <mvvm/providers/abstract_row_strategy.h>
#include <mvvm/providers/standard_children_strategies.h>
#include <mvvm/providers/viewitem_factory.h>
#include <mvvm/providers/viewmodel_controller_factory.h>
#include <mvvm/providers/viewitem.h>

namespace oac_tree_gui
{

class JobRowStrategy : public mvvm::AbstractRowStrategy
{
public:
  std::size_t GetSize() const override { return 2U; }

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

}  // namespace oac_tree_gui
