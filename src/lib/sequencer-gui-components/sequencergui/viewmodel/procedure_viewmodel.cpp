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

#include "procedure_viewmodel.h"

#include <sequencergui/model/procedure_item.h>

#include <mvvm/viewmodel/viewmodel_controller_factory.h>
#include <mvvm/viewmodel/abstract_row_strategy.h>
#include <mvvm/viewmodel/standard_children_strategies.h>
#include <mvvm/viewmodel/viewitem_factory.h>
#include <mvvm/viewmodelbase/viewitem.h>

namespace sequencergui
{

class ProcedureRowStrategy : public mvvm::AbstractRowStrategy
{
public:
  QStringList GetHorizontalHeaderLabels() const override
  {
    static QStringList result = {"Type"};
    return result;
  }

private:
  std::vector<std::unique_ptr<mvvm::ViewItem>> ConstructRowImpl(mvvm::SessionItem *item) override
  {
    std::vector<std::unique_ptr<mvvm::ViewItem>> result;
    result.emplace_back(mvvm::CreateEditableDisplayNameViewItem(item));
    return result;
  }
};

ProcedureViewModel::ProcedureViewModel(mvvm::ISessionModel *model, QObject *parent)
    : ViewModel(parent)
{
  SetController(
      mvvm::factory::CreateController<mvvm::TopItemsStrategy, ProcedureRowStrategy>(model, this));
}

int ProcedureViewModel::columnCount(const QModelIndex &parent) const
{
  return 1;  // Type
}

}  // namespace sequencergui
