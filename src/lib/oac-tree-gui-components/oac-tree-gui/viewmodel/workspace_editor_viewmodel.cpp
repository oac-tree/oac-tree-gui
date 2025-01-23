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

#include "workspace_editor_viewmodel.h"

#include "custom_row_strategies.h"

#include <mvvm/viewmodel/standard_children_strategies.h>
#include <mvvm/viewmodel/viewmodel_controller_factory.h>

namespace sequencergui
{

WorkspaceEditorViewModel::WorkspaceEditorViewModel(mvvm::ISessionModel *model, bool show_hidden,
                                                   QObject *parent)
    : ViewModel(parent)
{
  if (show_hidden)
  {
    SetController(mvvm::factory::CreateController<mvvm::AllChildrenStrategy, VariableRowStrategy>(
        model, this));
  }
  else
  {
    SetController(
        mvvm::factory::CreateController<mvvm::AllVisibleChildrenStrategy, VariableRowStrategy>(
            model, this));
  }
}

int WorkspaceEditorViewModel::columnCount(const QModelIndex &parent) const
{
  (void)parent;
  return 3;  // Name, Value, TypeName
}

}  // namespace sequencergui
