/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
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
#include <sequencergui/model/sequencer_item_helper.h>

#include <mvvm/model/item_utils.h>
#include <mvvm/viewmodel/abstract_row_strategy.h>
#include <mvvm/viewmodel/i_children_strategy.h>
#include <mvvm/viewmodel/viewitem_factory.h>
#include <mvvm/viewmodel/viewmodel_controller_factory.h>
#include <mvvm/viewmodelbase/viewitem.h>

namespace sequencergui
{

class JobRowStrategy : public mvvm::AbstractRowStrategy
{
public:
  QStringList GetHorizontalHeaderLabels() const override
  {
    static const QStringList result = {"Name", "Status"};
    return result;
  }

private:
  std::vector<std::unique_ptr<mvvm::ViewItem>> ConstructRowImpl(mvvm::SessionItem *item) override
  {
    std::vector<std::unique_ptr<mvvm::ViewItem>> result;
    result.emplace_back(mvvm::CreateEditableDisplayNameViewItem(item));
    result.emplace_back(mvvm::CreateDataViewItem(GetStatusItem(*item)));
    return result;
  }
};

class TopJobStrategy : public mvvm::IChildrenStrategy
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

JobListViewModel::JobListViewModel(mvvm::ISessionModel *model, QObject *parent) : ViewModel(parent)
{
  SetController(mvvm::factory::CreateController<TopJobStrategy, JobRowStrategy>(model, this));
}

int JobListViewModel::columnCount(const QModelIndex &parent) const
{
  return 2;  // Name, Status
}

}  // namespace sequencergui
