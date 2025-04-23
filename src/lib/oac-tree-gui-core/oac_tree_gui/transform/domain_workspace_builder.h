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

#ifndef OAC_TREE_GUI_TRANSFORM_DOMAIN_WORKSPACE_BUILDER_H_
#define OAC_TREE_GUI_TRANSFORM_DOMAIN_WORKSPACE_BUILDER_H_

#include <oac_tree_gui/domain/sequencer_types_fwd.h>

#include <map>
#include <string>

namespace oac_tree_gui
{

class WorkspaceItem;
class VariableItem;

//! Populates empty domain Workspace with the content of WorkspaceItem. Keeps track of workspace's
//! variables and corresponding VariableItem's.

class DomainWorkspaceBuilder
{
public:
  void PopulateDomainWorkspace(const WorkspaceItem* item, workspace_t* workspace);

  VariableItem* GetVariableItemFromDomainVariableName(const std::string& name) const;

private:
  std::map<const variable_t*, VariableItem*> m_domain_variable_to_item;
  std::map<std::string, VariableItem*> m_domain_name_to_item;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_TRANSFORM_DOMAIN_WORKSPACE_BUILDER_H_
