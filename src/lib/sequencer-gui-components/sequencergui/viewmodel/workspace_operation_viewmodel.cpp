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

#include "standard_children_strategies.h"
#include "standard_row_strategies.h"

#include <mvvm/factories/viewmodel_controller_factory.h>
#include <mvvm/viewmodel/standard_children_strategies.h>
#include <mvvm/viewmodel/standard_row_strategies.h>

namespace sequencergui
{

WorkspaceOperationViewModel::WorkspaceOperationViewModel(mvvm::SessionModelInterface *model,
                                                         QObject *parent)
    : ViewModel(parent)
{
  SetController(
      mvvm::factory::CreateController<VariableTableChildrenStrategy, VariableTableRowStrategy>(
          model, this));
}

}  // namespace sequencergui
