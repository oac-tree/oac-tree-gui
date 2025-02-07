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

#include "instruction_editor_action_handler.h"

#include <oac_tree_gui/components/anyvalue_dialog_result.h>
#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/universal_item_helper.h>
#include <oac_tree_gui/nodeeditor/scene_utils.h>
#include <oac_tree_gui/transform/transform_from_domain.h>
#include <oac_tree_gui/viewmodel/drag_and_drop_helper.h>

#include <sup/gui/components/mime_conversion_helper.h>
#include <sup/gui/core/query_result.h>
#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/model/model_utils.h>
#include <mvvm/model/validate_utils.h>

#include <QMimeData>
#include <QPointF>
#include <sstream>

namespace
{

/**
 * @brief Updates child coordinate so it is located near the reference
 */
void UpdateChildCoordinate(const oac_tree_gui::InstructionItem *reference, mvvm::SessionItem *child)
{
  const auto default_center = oac_tree_gui::GetGraphicsViewportCenter();
  if (auto inserted_instruction = dynamic_cast<oac_tree_gui::InstructionItem *>(child);
      inserted_instruction)
  {
    inserted_instruction->SetX(reference
                                   ? reference->GetX() + oac_tree_gui::GetInstructionDropOffset()
                                   : default_center.x());
    inserted_instruction->SetY(reference
                                   ? reference->GetY() + oac_tree_gui::GetInstructionDropOffset()
                                   : default_center.y());
  }
}

const std::string kFailedActionTitle = "Invalid Operation";

}  // namespace

namespace oac_tree_gui
{

InstructionEditorActionHandler::InstructionEditorActionHandler(InstructionEditorContext context,
                                                               QObject *parent_object)
    : QObject(parent_object), m_context(std::move(context))
{
  if (!m_context.selected_procedure)
  {
    throw RuntimeException("Callback to retrieve current procedure is not defined");
  }

  if (!m_context.selected_instruction)
  {
    throw RuntimeException("Callback to get selected instruction is not defined");
  }

  if (!m_context.send_message)
  {
    throw RuntimeException("Callback to send messages is not set");
  }
}

bool InstructionEditorActionHandler::CanInsertAfter(const std::string &item_type) const
{
  return CanInsertTypeAfterCurrentSelection(item_type).IsSuccess();
}

InstructionEditorActionHandler::~InstructionEditorActionHandler() = default;

void InstructionEditorActionHandler::OnInsertInstructionAfterRequest(const std::string &item_type)
{
  auto querry = CanInsertTypeAfterCurrentSelection(item_type);
  if (!querry.IsSuccess())
  {
    SendMessage(querry.GetMessage());
    return;
  }

  InsertAfterCurrentSelection(CreateInstructionItem(item_type));
}

bool InstructionEditorActionHandler::CanInsertInto(const std::string &item_type) const
{
  return CanInsertTypeIntoCurrentSelection(item_type).IsSuccess();
}

void InstructionEditorActionHandler::OnInsertInstructionIntoRequest(const std::string &item_type)
{
  auto querry = CanInsertTypeIntoCurrentSelection(item_type);
  if (!querry.IsSuccess())
  {
    SendMessage(querry.GetMessage());
    return;
  }

  InsertIntoCurrentSelection(CreateInstructionItem(item_type));
}

bool InstructionEditorActionHandler::CanRemoveInstruction() const
{
  const bool has_model = GetModel() != nullptr;
  const bool has_selection = GetSelectedInstruction() != nullptr;
  return has_model && has_selection;
}

void InstructionEditorActionHandler::OnRemoveInstructionRequest()
{
  if (!CanRemoveInstruction())
  {
    return;
  }

  mvvm::utils::BeginMacro(*GetModel(), "Remove instruction");

  auto selected_instruction = GetSelectedInstruction();
  auto next_to_select = mvvm::utils::FindNextSiblingToSelect(selected_instruction);
  GetModel()->RemoveItem(selected_instruction);
  UpdateProcedurePreamble();

  mvvm::utils::EndMacro(*GetModel());

  if (next_to_select)
  {
    // suggest to select something else instead of just deleted instruction
    emit SelectItemRequest(next_to_select);
  }
}

void InstructionEditorActionHandler::OnMoveUpRequest()
{
  if (auto instruction = GetSelectedInstruction(); instruction)
  {
    if (mvvm::utils::MoveUp(*instruction))
    {
      emit SelectItemRequest(instruction);
    }
  }
}

void InstructionEditorActionHandler::OnMoveDownRequest()
{
  if (auto instruction = GetSelectedInstruction(); instruction)
  {
    if (mvvm::utils::MoveDown(*instruction))
    {
      emit SelectItemRequest(instruction);
    }
  }
}

void InstructionEditorActionHandler::OnEditAnyvalueRequest()
{
  auto instruction_item = GetSelectedInstruction();

  if (!instruction_item || !mvvm::utils::HasTag(*instruction_item, itemconstants::kAnyValueTag))
  {
    SendMessage("Please select an instruction which is intended for AnyValue storing");
    return;
  }

  auto selected_anyvalue = GetAnyValueItem(*instruction_item);

  auto dialog_result = m_context.edit_anyvalue(selected_anyvalue);

  // existent value means that the user exited from the dialog with OK
  if (dialog_result.is_accepted)
  {
    if (!dialog_result.result)
    {
      SendMessage(
          "It is not possible to remove AnyValue from instruction, but you can mark is as "
          "disabled.");
      return;
    }

    // remove previous AnyValueItem
    if (selected_anyvalue)
    {
      GetModel()->RemoveItem(selected_anyvalue);
    }

    GetModel()->InsertItem(std::move(dialog_result.result), instruction_item, {});
  }
}

bool InstructionEditorActionHandler::CanCut() const
{
  return CanRemoveInstruction();
}

void InstructionEditorActionHandler::Cut()
{
  if (!CanCut())
  {
    return;
  }

  Copy();
  OnRemoveInstructionRequest();
}

bool InstructionEditorActionHandler::CanCopy() const
{
  const bool has_model = GetModel() != nullptr;
  const bool has_selection = GetSelectedInstruction() != nullptr;
  const bool has_clipboard = static_cast<bool>(m_context.set_mime_data);
  return has_model && has_selection && has_clipboard;
}

void InstructionEditorActionHandler::Copy()
{
  if (!CanCopy())
  {
    return;
  }

  m_context.set_mime_data(
      sup::gui::CreateCopyMimeData(*GetSelectedInstruction(), kCopyInstructionMimeType));
}

bool InstructionEditorActionHandler::CanPasteAfter() const
{
  auto querry = CanInsertTypeAfterCurrentSelection(
      sup::gui::GetSessionItemType(GetMimeData(), kCopyInstructionMimeType));
  return querry.IsSuccess();
}

void InstructionEditorActionHandler::PasteAfter()
{
  if (!CanPasteAfter())
  {
    return;
  }

  InsertAfterCurrentSelection(sup::gui::CreateSessionItem(GetMimeData(), kCopyInstructionMimeType));
}

bool InstructionEditorActionHandler::CanPasteInto() const
{
  auto querry = CanInsertTypeIntoCurrentSelection(
      sup::gui::GetSessionItemType(GetMimeData(), kCopyInstructionMimeType));
  return querry.IsSuccess();
}

void InstructionEditorActionHandler::PasteInto()
{
  if (!CanPasteInto())
  {
    return;
  }

  InsertIntoCurrentSelection(sup::gui::CreateSessionItem(GetMimeData(), kCopyInstructionMimeType));
}

InstructionItem *InstructionEditorActionHandler::GetSelectedInstruction() const
{
  return m_context.selected_instruction();
}

mvvm::ISessionModel *InstructionEditorActionHandler::GetModel() const
{
  return GetInstructionContainer() ? GetInstructionContainer()->GetModel() : nullptr;
}

InstructionContainerItem *InstructionEditorActionHandler::GetInstructionContainer() const
{
  auto procedure = m_context.selected_procedure();
  return procedure ? procedure->GetInstructionContainer() : nullptr;
}

void InstructionEditorActionHandler::SendMessage(const std::string &text,
                                                 const std::string &informative,
                                                 const std::string &details) const
{
  SendMessage(sup::gui::CreateInvalidOperationMessage(text, informative, details));
}

void InstructionEditorActionHandler::SendMessage(const sup::gui::MessageEvent &message_event) const
{
  m_context.send_message(message_event);
}

const QMimeData *InstructionEditorActionHandler::GetMimeData() const
{
  return m_context.get_mime_data ? m_context.get_mime_data() : nullptr;
}

void InstructionEditorActionHandler::UpdateProcedurePreamble()
{
  if (auto procedure_item = mvvm::utils::FindItemUp<ProcedureItem>(GetInstructionContainer());
      procedure_item)
  {
    UpdatePluginNames(*procedure_item);
  }
}

sup::gui::QueryResult InstructionEditorActionHandler::CanInsertTypeAfterCurrentSelection(
    const std::string &item_type) const
{
  static const std::string kFailedActionText("Can't insert type after current selection");

  if (item_type.empty())
  {
    return sup::gui::QueryResult::Failure(
        {kFailedActionTitle, kFailedActionText, "Wrong item type [" + item_type + "]", ""});
  }

  auto instruction_container = GetInstructionContainer();
  if (!instruction_container)
  {
    return sup::gui::QueryResult::Failure(
        {kFailedActionTitle, kFailedActionText, "No procedure selected", ""});
  }

  // Checking if there is a selection inside another parent. To paste after this selection, the
  // parent should have the room for more items.
  if (auto selected_item = GetSelectedInstruction(); selected_item)
  {
    auto [success_flag, informative] = mvvm::utils::CanInsertType(
        item_type, selected_item->GetParent(), selected_item->GetTagIndex().Next());
    if (!success_flag)
    {
      return sup::gui::QueryResult::Failure(
          {kFailedActionTitle, kFailedActionText, informative, ""});
    }
  }

  return sup::gui::QueryResult::Success();
}

sup::gui::QueryResult InstructionEditorActionHandler::CanInsertTypeIntoCurrentSelection(
    const std::string &item_type) const
{
  static const std::string kFailedActionText("Can't insert type into current selection");

  if (item_type.empty())
  {
    return sup::gui::QueryResult::Failure(
        {kFailedActionTitle, kFailedActionText, "Wrong item type [" + item_type + "]", ""});
  }

  auto selected_instruction = GetSelectedInstruction();
  if (!selected_instruction)
  {
    return sup::gui::QueryResult::Failure(
        {kFailedActionTitle, kFailedActionText, "No instruction selected", ""});
  }

  // Checking if there is a selection inside another parent. To paste after this selection, the
  // parent should have the room for more items.
  if (auto selected_item = GetSelectedInstruction(); selected_item)
  {
    auto [success_flag, informative] =
        mvvm::utils::CanInsertType(item_type, selected_item, mvvm::TagIndex::Append());
    if (!success_flag)
    {
      return sup::gui::QueryResult::Failure(
          {kFailedActionTitle, kFailedActionText, informative, ""});
    }
  }

  return sup::gui::QueryResult::Success();
}

void InstructionEditorActionHandler::InsertAfterCurrentSelection(
    std::unique_ptr<mvvm::SessionItem> item)
{
  mvvm::utils::BeginMacro(*GetModel(), "Insert instruction");

  auto selected_item = GetSelectedInstruction();

  auto parent = selected_item ? selected_item->GetParent() : GetInstructionContainer();
  auto tagindex = selected_item ? selected_item->GetTagIndex().Next() : mvvm::TagIndex::Append();

  auto child = InsertItem(std::move(item), parent, tagindex);
  UpdateProcedurePreamble();
  UpdateChildCoordinate(selected_item, child);

  mvvm::utils::EndMacro(*GetModel());
}

void InstructionEditorActionHandler::InsertIntoCurrentSelection(
    std::unique_ptr<mvvm::SessionItem> item)
{
  mvvm::utils::BeginMacro(*GetModel(), "Insert instruction");

  auto child = InsertItem(std::move(item), GetSelectedInstruction(), mvvm::TagIndex::Append());
  UpdateProcedurePreamble();
  UpdateChildCoordinate(GetSelectedInstruction(), child);

  mvvm::utils::EndMacro(*GetModel());
}

mvvm::SessionItem *InstructionEditorActionHandler::InsertItem(
    std::unique_ptr<mvvm::SessionItem> item, mvvm::SessionItem *parent, const mvvm::TagIndex &index)
{
  if (!GetModel())
  {
    throw RuntimeException("Uninitialised model");
  }

  if (parent == nullptr)
  {
    throw RuntimeException("Uninitialised parent");
  }

  mvvm::SessionItem *result{nullptr};
  const auto item_type = item->GetType();
  try
  {
    result = GetModel()->InsertItem(std::move(item), parent, index);
    emit SelectItemRequest(result);
  }
  catch (const std::exception &ex)
  {
    std::ostringstream ostr;
    ostr << "Exception was caught while trying to insert instruction [" << item_type
         << "] into parent [" << parent->GetType() << "]";
    SendMessage("Can't insert instruction", ostr.str(), ex.what());
  }
  return result;
}

}  // namespace oac_tree_gui
