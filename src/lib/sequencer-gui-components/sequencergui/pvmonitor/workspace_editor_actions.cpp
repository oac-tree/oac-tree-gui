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

#include "workspace_editor_actions.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/model/standard_variable_items.h>
#include <sequencergui/model/workspace_item.h>
#include <sequencergui/transform/transform_helpers.h>
#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/interfaces/sessionmodel_interface.h>

#include <sup/dto/anyvalue.h>

namespace sequencergui
{

WorkspaceEditorActions::WorkspaceEditorActions(WorkspaceEditorContext context, QObject *parent)
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

  if (!m_context.edit_anyvalue_callback)
  {
    throw RuntimeException("Absent callback to get AnyValueItem");
  }
}

void WorkspaceEditorActions::OnAddVariableRequest(const QString &variable_type_name)
{
  if (!GetWorkspaceItem())
  {
    SendMessage("No variable Workspace is selected", "Please create procedure first");
    return;
  }

  auto selected_item = GetSelectedVariable();

  try
  {
    auto tagindex = selected_item ? selected_item->GetTagIndex().Next() : mvvm::TagIndex::Append();
    auto inserted = GetModel()->InsertItem(
        GetModel()->GetFactory()->CreateItem(variable_type_name.toStdString()), GetWorkspaceItem(),
        tagindex);
    SetupVariable(dynamic_cast<VariableItem *>(inserted));
  }
  catch (const std::exception &ex)
  {
    SendMessage("Can't add new workspace variable", "Exception was caught", ex.what());
  }
}

void WorkspaceEditorActions::OnRemoveVariableRequest()
{
  if (auto selected = GetSelectedVariable(); selected)
  {
    GetModel()->RemoveItem(selected);
  }
}

void WorkspaceEditorActions::OnEditAnyvalueRequest()
{
  auto selected_anyvalue = GetAnyValueItemToEdit();
  if (!selected_anyvalue)
  {
    SendMessage("Please select AnyValue you want to modify",
                "You can also select workspace variable itself");
    return;
  }

  auto edited_anyvalue = m_context.edit_anyvalue_callback(*selected_anyvalue);

  // existent value means that the user exited from the dialog with OK
  if (edited_anyvalue.has_value())
  {
    // remove previous AnyValueItem
    auto prev_parent = selected_anyvalue->GetParent();
    GetModel()->RemoveItem(selected_anyvalue);

    if (edited_anyvalue.value())
    {
      // if unique_ptr<AnyValueItem> is not empty, move it as a new value
      GetModel()->InsertItem(std::move(edited_anyvalue.value()), prev_parent, {});
    }
  }
}

mvvm::SessionModelInterface *WorkspaceEditorActions::GetModel() const
{
  return GetWorkspaceItem()->GetModel();
}

WorkspaceItem *WorkspaceEditorActions::GetWorkspaceItem() const
{
  return m_context.selected_workspace_callback();
}

VariableItem *WorkspaceEditorActions::GetSelectedVariable()
{
  return dynamic_cast<VariableItem *>(m_context.selected_item_callback());
}

//! Returns selected AnyValueItem.
sup::gui::AnyValueItem *WorkspaceEditorActions::GetSelectedAnyValueItem()
{
  return dynamic_cast<sup::gui::AnyValueItem *>(m_context.selected_item_callback());
}

//! Returns AnyValueItem intended for editing.

sup::gui::AnyValueItem *WorkspaceEditorActions::GetAnyValueItemToEdit()
{
  // If top level VariableItem is selected, it will return its underlying AnyValueItem.
  // Otherwise it will return selected AnyValueItem, if any.
  return GetSelectedVariable() ? GetSelectedVariable()->GetAnyValueItem()
                               : GetSelectedAnyValueItem();
}

//! Set reasonable initial values for just created variable.
//! Might be changed in the future.

void WorkspaceEditorActions::SetupVariable(VariableItem *item)
{
  if (!item)
  {
    return;
  }

  item->SetName(ProposeVariableName());
  // By default we always set scalar anyvalue to any VariableItem added to the WorkspaceItem.
  // If user wants something else, he has to start AnyValueEditor.
  SetAnyValue(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 0}, *item);
}

void WorkspaceEditorActions::SendMessage(const std::string &text, const std::string &informative,
                                         const std::string &details)
{
  auto message = sup::gui::CreateInvalidOperationMessage(text, informative, details);
  m_context.send_message_callback(message);
}

std::string WorkspaceEditorActions::ProposeVariableName() const
{
  return "var" + std::to_string(GetWorkspaceItem()->GetVariableCount() - 1);
}

}  // namespace sequencergui
