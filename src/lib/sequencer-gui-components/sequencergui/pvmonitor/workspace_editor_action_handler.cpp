/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
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

#include <sequencergui/components/anyvalue_editor_dialog_factory.h>
#include <sequencergui/core/exceptions.h>
#include <sequencergui/model/standard_variable_items.h>
#include <sequencergui/model/workspace_item.h>
#include <sequencergui/pvmonitor/workspace_monitor_helper.h>
#include <sequencergui/transform/transform_from_domain.h>
#include <sequencergui/viewmodel/drag_and_drop_helper.h>
#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/interfaces/sessionmodel_interface.h>
#include <mvvm/model/item_utils.h>

#include <QMimeData>

namespace sequencergui
{

WorkspaceEditorActionHandler::WorkspaceEditorActionHandler(WorkspaceEditorContext context,
                                                           QObject *parent)
    : QObject(parent), m_context(std::move(context))
{
  if (!m_context.selected_workspace_callback)
  {
    throw RuntimeException("Absent callback to get Workspace");
  }

  if (!m_context.selected_item_callback)
  {
    throw RuntimeException("Absent callback to retrieve currently selected item");
  }

  if (!m_context.send_message_callback)
  {
    throw RuntimeException("Absent callback to send messages");
  }
}

void WorkspaceEditorActionHandler::OnAddVariableRequest(const QString &variable_type_name)
{
  if (!GetWorkspaceItem())
  {
    SendMessage("No variable Workspace is selected", "Please create procedure first");
    return;
  }

  auto variable_item = CreateVariableItem(variable_type_name.toStdString());
  SetupNewVariable(variable_item.get(), GetWorkspaceItem()->GetVariableCount());
  InsertVariableAfterCurrentSelection(std::move(variable_item));
}

bool WorkspaceEditorActionHandler::CanRemoveVariable() const
{
  return GetSelectedVariable() != nullptr;
}

void WorkspaceEditorActionHandler::OnRemoveVariableRequest()
{
  if (auto selected = GetSelectedVariable(); selected)
  {
    auto next_to_select = mvvm::utils::FindNextSiblingToSelect(selected);
    GetModel()->RemoveItem(selected);
    if (next_to_select)
    {
      // suggest to select something else instead of just deleted variable
      emit SelectItemRequest(next_to_select);
    }
  }
}

void WorkspaceEditorActionHandler::OnEditAnyValueRequest()
{
  auto selected_item = m_context.selected_item_callback();
  if (!selected_item)
  {
    SendMessage(
        "Please select Workspace variable (or any of it's leaves) to modify corresponding "
        "AnyValue.");
    return;
  }

  auto selected_variable =
      GetSelectedVariable()
          ? GetSelectedVariable()
          : const_cast<VariableItem *>(mvvm::utils::FindItemUp<VariableItem>(selected_item));

  auto selected_anyvalue = selected_variable->GetAnyValueItem();

  auto edited_anyvalue = m_context.edit_anyvalue_callback(selected_anyvalue);

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
  return GetSelectedVariable() != nullptr;
}

void WorkspaceEditorActionHandler::Cut()
{
  if (!CanCut())
  {
    return;
  }

  Copy();
  OnRemoveVariableRequest();
}

bool WorkspaceEditorActionHandler::CanCopy() const
{
  return GetSelectedVariable() != nullptr && m_context.set_mime_data;
}

void WorkspaceEditorActionHandler::Copy()
{
  if (!CanCopy())
  {
    return;
  }

  m_context.set_mime_data(CreateCopyMimeData(*GetSelectedVariable(), kCopyVariableMimeType));
}

bool WorkspaceEditorActionHandler::CanPaste() const
{
  auto mime_data = GetMimeData();
  return mime_data && mime_data->hasFormat(kCopyVariableMimeType);
}

void WorkspaceEditorActionHandler::Paste()
{
  if (!CanPaste())
  {
    return;
  }

  InsertVariableAfterCurrentSelection(CreateSessionItem(GetMimeData(), kCopyVariableMimeType));
}

mvvm::SessionModelInterface *WorkspaceEditorActionHandler::GetModel() const
{
  return GetWorkspaceItem()->GetModel();
}

WorkspaceItem *WorkspaceEditorActionHandler::GetWorkspaceItem() const
{
  return m_context.selected_workspace_callback();
}

VariableItem *WorkspaceEditorActionHandler::GetSelectedVariable() const
{
  return dynamic_cast<VariableItem *>(m_context.selected_item_callback());
}

const QMimeData *WorkspaceEditorActionHandler::GetMimeData() const
{
  return m_context.get_mime_data ? m_context.get_mime_data() : nullptr;
}

void WorkspaceEditorActionHandler::InsertVariableAfterCurrentSelection(
    std::unique_ptr<mvvm::SessionItem> variable_item)
{
  auto selected_item = GetSelectedVariable();

  try
  {
    auto tagindex = selected_item ? selected_item->GetTagIndex().Next() : mvvm::TagIndex::Append();
    auto inserted = GetModel()->InsertItem(std::move(variable_item), GetWorkspaceItem(), tagindex);
    emit SelectItemRequest(inserted);
  }
  catch (const std::exception &ex)
  {
    SendMessage("Can't add new workspace variable", "Exception was caught", ex.what());
  }
}

void WorkspaceEditorActionHandler::SendMessage(const std::string &text,
                                               const std::string &informative,
                                               const std::string &details)
{
  auto message = sup::gui::CreateInvalidOperationMessage(text, informative, details);
  m_context.send_message_callback(message);
}

}  // namespace sequencergui
