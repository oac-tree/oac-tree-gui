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

#ifndef OAC_TREE_GUI_MODEL_WORKSPACE_ITEM_H_
#define OAC_TREE_GUI_MODEL_WORKSPACE_ITEM_H_

#include <mvvm/model/compound_item.h>

namespace oac_tree_gui
{
class VariableItem;

//! Represents a Workspace.

class WorkspaceItem : public mvvm::CompoundItem
{
public:
  WorkspaceItem();

  static std::string GetStaticType();

  std::unique_ptr<SessionItem> Clone() const override;

  std::vector<VariableItem*> GetVariables() const;

  int GetVariableCount() const;
};
}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_MODEL_WORKSPACE_ITEM_H_
