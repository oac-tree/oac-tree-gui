/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "procedure_list_viewmodel.h"

#include <mvvm/providers/standard_children_strategies.h>
#include <mvvm/providers/standard_row_strategies.h>
#include <mvvm/providers/viewmodel_controller_factory.h>

namespace oac_tree_gui
{

ProcedureListViewModel::ProcedureListViewModel(mvvm::ISessionModel* model, QObject* parent_object)
    : FlatListViewModel(parent_object)
{
  SetController(mvvm::factory::CreateController<mvvm::TopItemsStrategy, mvvm::LabelDataRowStrategy>(
      model, this));
}

}  // namespace oac_tree_gui
