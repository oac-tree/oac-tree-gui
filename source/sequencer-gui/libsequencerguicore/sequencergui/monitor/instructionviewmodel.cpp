/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/monitor/instructionviewmodel.h"

#include "sequencergui/model/sequenceritems.h"

#include "mvvm/interfaces/rowstrategyinterface.h"
#include "mvvm/model/function_types.h"
#include "mvvm/viewmodel/applicationmodel.h"
#include "mvvm/viewmodel/standardchildrenstrategies.h"
#include "mvvm/viewmodel/standardrowstrategies.h"
#include "mvvm/viewmodel/viewitemfactory.h"
#include "mvvm/viewmodel/viewmodelcontroller.h"
#include "mvvm/viewmodelbase/viewitem.h"

namespace sequencergui
{

class InstructionRowStrategy : public ModelView::RowStrategyInterface
{
public:
  explicit InstructionRowStrategy(const ModelView::item_setdata_function_t &set_func)
      : m_set_func(set_func)
  {
  }

  QStringList GetHorizontalHeaderLabels() const override
  {
    static QStringList result = {"Type", "Name", "Status"};
    return result;
  }

  std::vector<std::unique_ptr<ModelView::ViewItem>> ConstructRow(
      ModelView::SessionItem *item) override
  {
    std::vector<std::unique_ptr<ModelView::ViewItem>> result;

    // For one SessionItem the row consisting of two ViewItems will be generated: one for
    // displayName, another for SessionItem's data (if defined).

    if (!item)
    {
      return result;
    }

    result.emplace_back(ModelView::CreateDisplayNameViewItem(item));

    if (auto instruction = dynamic_cast<InstructionItem *>(item); instruction)
    {
      result.emplace_back(ModelView::CreateDataViewItem(instruction->GetNameItem(), {}));
      result.emplace_back(ModelView::CreateDataViewItem(instruction->GetStatusItem(), {}));
    }
    return result;
  }

private:
  ModelView::item_setdata_function_t m_set_func;
};

InstructionViewModel::InstructionViewModel(ModelView::ApplicationModel *model, QObject *parent)
    : ViewModel(parent)
{
  // FIXME remove code duplications from PropertyViewModel, TopItemsViewModel
  auto controller = std::make_unique<ModelView::ViewModelController>(model, this);
  model->Subscribe(controller.get());
  controller->SetChildrenStrategy(std::make_unique<ModelView::TopItemsStrategy>());

  auto set_data = [model](auto item, auto data, auto role)
  { return model->SetData(item, data, role); };

  controller->SetRowStrategy(std::make_unique<InstructionRowStrategy>(set_data));
  SetController(std::move(controller));
}

}  // namespace sequi
