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

#include "workspace_operation_viewmodel.h"

#include "custom_children_strategies.h"
#include "custom_row_strategies.h"

#include <mvvm/factories/viewmodel_controller_factory.h>
#include <mvvm/viewmodel/standard_children_strategies.h>
#include <mvvm/viewmodel/standard_row_strategies.h>
#include <mvvm/viewmodel/viewmodel_controller.h>
#include <mvvm/viewmodel/viewmodel_controller_impl.h>

namespace
{
std::unique_ptr<mvvm::IViewModelController> CreateImpl(mvvm::ViewModelBase *viewmodel)
{
  auto children_strategy = std::make_unique<sequencergui::VariableTableChildrenStrategy>();
  auto row_strategy = std::make_unique<sequencergui::VariableTableRowStrategy>();

  auto result = std::make_unique<mvvm::ViewModelControllerImpl>(
      viewmodel, std::move(children_strategy), std::move(row_strategy));

  return result;
}

}  // namespace

namespace sequencergui
{

class WorkspaceOperationViewModelController : public mvvm::ViewModelController
{
public:
  explicit WorkspaceOperationViewModelController(mvvm::ViewModelBase *viewmodel)
      : mvvm::ViewModelController(CreateImpl(viewmodel))
  {
  }
};

WorkspaceOperationViewModel::WorkspaceOperationViewModel(mvvm::SessionModelInterface *model,
                                                         QObject *parent)
    : ViewModel(parent)
{
  auto controller = std::make_unique<WorkspaceOperationViewModelController>(this);
  controller->SetModel(model);
  SetController(std::move(controller));
}

}  // namespace sequencergui
