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

#include "mock_workspace_editor_context.h"

#include <oac_tree_gui/composer/workspace_editor_action_handler.h>

#include <sup/gui/model/anyvalue_item.h>

#include <QMimeData>

namespace oac_tree_gui::test
{

WorkspaceEditorContext MockWorkspaceEditorContext::CreateContext(
    WorkspaceItem *workspace, const std::vector<mvvm::SessionItem *> &selection)
{
  WorkspaceEditorContext result;

  m_current_selection = selection;

  result.selected_workspace = [this, workspace]() { return workspace; };
  result.selected_items_callback = [this]() { return m_current_selection; };
  result.select_notify = [this](auto item) { SelectRequest(item); };
  result.send_message = [this](const auto &message) { OnMessage(message); };
  result.edit_anyvalue = [this](const sup::gui::AnyValueItem *item)
  { return OnEditAnyvalue(item); };
  result.get_mime_data = [this]() { return OnGetMimeData(); };
  result.set_mime_data = [this](auto mime_data)
  {
    // mimicking clipboard, saving copy result here
    m_copy_result = std::move(mime_data);
    OnSetMimeData();
  };

  return result;
}

std::unique_ptr<WorkspaceEditorActionHandler> MockWorkspaceEditorContext::CreateActionHandler(
    WorkspaceItem *workspace, const std::vector<mvvm::SessionItem *> &selection)
{
  return std::make_unique<WorkspaceEditorActionHandler>(CreateContext(workspace, selection));
}

QMimeData *MockWorkspaceEditorContext::GetCopyResult() const
{
  return m_copy_result.get();
}

}  // namespace oac_tree_gui::test
