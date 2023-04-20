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

#ifndef SEQUENCERGUI_PVMONITOR_WORKSPACE_MONITOR_HELPER_H_
#define SEQUENCERGUI_PVMONITOR_WORKSPACE_MONITOR_HELPER_H_

#include <sequencergui/domain/sequencer_types_fwd.h>

#include <mvvm/model/sessionitem.h>

namespace mvvm
{
class SessionItem;
}

namespace sequencergui
{
class WorkspaceItem;
class DomainWorkspaceBuilder;
class VariableItem;
}  // namespace sequencergui

namespace sequencergui
{

//! Populate empty domain workspace with the content in WorkspaceItem.
//! If domain workspace is non-empty, will throw.
void PopulateDomainWorkspace(const WorkspaceItem& item, workspace_t& workspace);

/**
 * @brief Find ancestor of given type by going up in the hierarchy.
 *
 * @param item The child to start with.
 * @return First parent which was successfully dynamically casted to a given type.
 */
template <typename T>
T* FindAncestor(const mvvm::SessionItem* item)
{
  if (!item)
  {
    return nullptr;
  }

  mvvm::SessionItem* parent = item->GetParent();

  while (parent)
  {
    if (auto result = dynamic_cast<T*>(parent); result)
    {
      return result;
    }
    parent = parent->GetParent();
  }

  return nullptr;
}

}  // namespace sequencergui

#endif  // SEQUENCERGUI_PVMONITOR_WORKSPACE_MONITOR_HELPER_H_
