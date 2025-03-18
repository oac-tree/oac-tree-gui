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

#include "workspace_editor_action_handler.h"

#include <oac_tree_gui/components/anyvalue_dialog_result.h>
#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/standard_variable_items.h>
#include <oac_tree_gui/model/workspace_item.h>
#include <oac_tree_gui/pvmonitor/workspace_monitor_helper.h>
#include <oac_tree_gui/transform/transform_from_domain.h>
#include <oac_tree_gui/viewmodel/drag_and_drop_helper.h>

#include <sup/gui/components/mime_conversion_helper.h>
#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/model/i_session_model.h>
#include <mvvm/model/item_utils.h>
#include <mvvm/model/model_utils.h>

#include <QMimeData>

namespace oac_tree_gui
{

namespace
{

std::vector<const mvvm::SessionItem *> GetItemVector(const std::vector<VariableItem *> &vec)
{
  std::vector<const mvvm::SessionItem *> result;
  std::transform(vec.begin(), vec.end(), std::back_inserter(result),
                 [](auto element) { return element; });
  return result;
}

}  // namespace

WorkspaceEditorActionHandler::WorkspaceEditorActionHandler(WorkspaceEditorContext context)
    : m_context(std::move(context))
{
  if (!m_context.selected_workspace)
  {
    throw RuntimeException("Absent callback to get Workspace");
  }

  if (!m_context.selected_items_callback)
  {
    throw RuntimeException("Absent callback to retrieve currently selected items");
  }

  if (!m_context.select_notify)
  {
    throw RuntimeException("Absent callback to request item selection");
  }

  if (!m_context.send_message)
  {
    throw RuntimeException("Absent callback to send messages");
  }
}

void WorkspaceEditorActionHandler::AddVariable(const std::string &variable_type_name)
{
  if (!GetWorkspaceItem())
  {
    SendMessage("No variable Workspace is selected", "Please create procedure first");
    return;
  }

  auto variable_item = CreateVariableItem(variable_type_name);
  SetupNewVariable(variable_item.get(), GetWorkspaceItem()->GetVariableCount());
  std::vector<std::unique_ptr<mvvm::SessionItem>> items;
  items.push_back(std::move(variable_item));
  InsertVariableAfterCurrentSelection(std::move(items));
}

bool WorkspaceEditorActionHandler::CanRemoveVariable() const
{
  const bool has_model = GetModel() != nullptr;
  const bool has_selection = GetSelectedVariable() != nullptr;
  return has_model && has_selection;
}

void WorkspaceEditorActionHandler::RemoveVariable()
{
  mvvm::SessionItem *next_to_select{nullptr};

  mvvm::utils::BeginMacro(*GetModel(), "Remove variable");
  for (auto selected : GetSelectedVariables())
  {
    next_to_select = mvvm::utils::FindNextSiblingToSelect(selected);
    GetModel()->RemoveItem(selected);
  }
  UpdateProcedurePreamble();
  mvvm::utils::EndMacro(*GetModel());

  if (next_to_select)
  {
    // suggest to select something else instead of just deleted variable
    SelectNotify(next_to_select);
  }
}

void WorkspaceEditorActionHandler::EditAnyValue()
{
  auto selected_items = m_context.selected_items_callback();
  if (selected_items.empty())
  {
    SendMessage(
        "Please select Workspace variable (or any of it's leaves) to modify corresponding "
        "AnyValue.");
    return;
  }

  auto selected_item = selected_items.front();

  auto selected_variable =
      GetSelectedVariable()
          ? GetSelectedVariable()
          : const_cast<VariableItem *>(mvvm::utils::FindItemUp<VariableItem>(selected_item));

  auto selected_anyvalue = selected_variable->GetAnyValueItem();

  auto edited_anyvalue = m_context.edit_anyvalue(selected_anyvalue);

  // existent value means that the user exited from the dialog with OK
  if (edited_anyvalue.is_accepted)
  {
    if (!edited_anyvalue.result)
    {
      SendMessage("It is not possible to remove AnyValue from variable.");
      return;
    }

    // remove previous AnyValueItem
    if (selected_anyvalue)
    {
      GetModel()->RemoveItem(selected_anyvalue);
    }

    GetModel()->InsertItem(std::move(edited_anyvalue.result), selected_variable, {});
  }
}

bool WorkspaceEditorActionHandler::CanCut() const
{
  return CanRemoveVariable();
}

void WorkspaceEditorActionHandler::Cut()
{
  if (!CanCut())
  {
    return;
  }

  Copy();
  RemoveVariable();
}

bool WorkspaceEditorActionHandler::CanCopy() const
{
  const bool has_model = GetModel() != nullptr;
  const bool has_selection = GetSelectedVariable() != nullptr;
  const bool has_clipboard = static_cast<bool>(m_context.set_mime_data);
  return has_model && has_selection && has_clipboard;
}

void WorkspaceEditorActionHandler::Copy()
{
  if (!CanCopy())
  {
    return;
  }

  auto items = GetItemVector(GetSelectedVariables());
  m_context.set_mime_data(sup::gui::CreateCopyMimeData(items, kCopyVariableMimeType));
}

bool WorkspaceEditorActionHandler::CanPaste() const
{
  const bool has_model = GetModel() != nullptr;
  auto mime_data = GetMimeData();
  return has_model && mime_data && mime_data->hasFormat(kCopyVariableMimeType);
}

void WorkspaceEditorActionHandler::Paste()
{
  if (!CanPaste())
  {
    return;
  }

  InsertVariableAfterCurrentSelection(sup::gui::CreateSessionItems(GetMimeData(), kCopyVariableMimeType));
}

VariableItem *WorkspaceEditorActionHandler::GetSelectedVariable() const
{
  auto selected_variables = GetSelectedVariables();
  return selected_variables.empty() ? nullptr : selected_variables.front();
}

std::vector<VariableItem *> WorkspaceEditorActionHandler::GetSelectedVariables() const
{
  return mvvm::utils::CastItems<VariableItem>(m_context.selected_items_callback());
}

mvvm::ISessionModel *WorkspaceEditorActionHandler::GetModel() const
{
  return GetWorkspaceItem() ? GetWorkspaceItem()->GetModel() : nullptr;
}

WorkspaceItem *WorkspaceEditorActionHandler::GetWorkspaceItem() const
{
  return m_context.selected_workspace();
}

void WorkspaceEditorActionHandler::SelectNotify(mvvm::SessionItem *item) const
{
  m_context.select_notify(item);
}

const QMimeData *WorkspaceEditorActionHandler::GetMimeData() const
{
  return m_context.get_mime_data ? m_context.get_mime_data() : nullptr;
}

void WorkspaceEditorActionHandler::UpdateProcedurePreamble()
{
  if (auto procedure_item = mvvm::utils::FindItemUp<ProcedureItem>(GetWorkspaceItem());
      procedure_item)
  {
    UpdatePluginNames(*procedure_item);
  }
}

void WorkspaceEditorActionHandler::InsertVariableAfterCurrentSelection(
    std::vector<std::unique_ptr<mvvm::SessionItem> > variable_items)
{
  if (!GetModel())
  {
    throw RuntimeException("Model is not initialised");
  }

  auto selected_item = GetSelectedVariable();

  mvvm::utils::BeginMacro(*GetModel(), "Insert variable");

  try
  {
    auto tagindex = selected_item ? selected_item->GetTagIndex().Next() : mvvm::TagIndex::Append();

    mvvm::SessionItem *inserted{nullptr};
    for (auto &item : variable_items)
    {
      inserted = GetModel()->InsertItem(std::move(item), GetWorkspaceItem(), tagindex);
      tagindex = inserted->GetTagIndex().Next();
    }
    UpdateProcedurePreamble();

    SelectNotify(inserted);
  }
  catch (const std::exception &ex)
  {
    SendMessage("Can't add new workspace variable", "Exception was caught", ex.what());
  }

  mvvm::utils::EndMacro(*GetModel());
}

void WorkspaceEditorActionHandler::SendMessage(const std::string &text,
                                               const std::string &informative,
                                               const std::string &details)
{
  auto message = sup::gui::CreateInvalidOperationMessage(text, informative, details);
  m_context.send_message(message);
}

}  // namespace oac_tree_gui
