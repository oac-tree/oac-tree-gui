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

#include "instruction_editor_action_handler.h"

#include "instruction_copy_helper.h"

#include <oac_tree_gui/components/anyvalue_dialog_result.h>
#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/universal_item_helper.h>
#include <oac_tree_gui/nodeeditor/scene_utils.h>
#include <oac_tree_gui/nodeeditor/sequencer_align_utils.h>
#include <oac_tree_gui/transform/transform_from_domain.h>
#include <oac_tree_gui/viewmodel/drag_and_drop_helper.h>

#include <sup/gui/components/item_filter_helper.h>
#include <sup/gui/components/mime_conversion_helper.h>
#include <sup/gui/core/query_result.h>
#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/model/model_utils.h>
#include <mvvm/model/validate_utils.h>

#include <QMimeData>
#include <QPointF>
#include <sstream>

namespace oac_tree_gui
{

namespace
{

/**
 * @brief Returns coordinate located not far from the given reference.
 */
IInstructionEditorActionHandler::position_t GetCoordinateNearby(
    oac_tree_gui::InstructionItem *reference)
{
  const auto default_center = oac_tree_gui::GetGraphicsViewportCenter();
  const double x =
      reference ? reference->GetX() + oac_tree_gui::GetInstructionDropOffset() : default_center.x();
  const double y =
      reference ? reference->GetY() + oac_tree_gui::GetInstructionDropOffset() : default_center.y();

  return {x, y};
}

/**
 * @brief Updates instruction coordinate, as well as coordinates of all possible children, so it is
 * nicely aligned.
 *
 * @param ref_point Coordinates of parent instructions.
 */
void AlignInstructionTree(const IInstructionEditorActionHandler::position_t &ref_point,
                          mvvm::SessionItem *instruction)
{
  if (auto inserted_instruction = dynamic_cast<oac_tree_gui::InstructionItem *>(instruction);
      inserted_instruction)
  {
    algorithm::AlignInstructionTreeWalker({ref_point.first, ref_point.second},
                                          inserted_instruction);
  }
}

const std::string kFailedActionTitle = "Invalid Operation";

}  // namespace

InstructionEditorActionHandler::InstructionEditorActionHandler(InstructionEditorContext context)
    : m_context(std::move(context))
{
  if (!m_context.instruction_container)
  {
    throw RuntimeException("Callback to retrieve current instruction container is not defined");
  }

  if (!m_context.selected_instructions)
  {
    throw RuntimeException("Callback to get selected instruction is not defined");
  }

  if (!m_context.notify_request)
  {
    throw RuntimeException("Callback to notify about selection is not defined");
  }

  if (!m_context.create_instruction)
  {
    m_context.create_instruction = [](const auto &item_type)
    { return CreateInstructionItem(item_type); };
  }
}

InstructionEditorActionHandler::~InstructionEditorActionHandler() = default;

void InstructionEditorActionHandler::DropInstruction(const std::string &item_type,
                                                     const position_t &pos)
{
  std::vector<std::unique_ptr<mvvm::SessionItem>> items;
  items.push_back(CreateInstructionTree(item_type));

  InsertItem(std::move(items), GetInstructionContainer(), mvvm::TagIndex::Append(), pos);
}

bool InstructionEditorActionHandler::CanInsertInstructionAfter(const std::string &item_type) const
{
  return CanInsertTypeAfterCurrentSelection(item_type).IsSuccess();
}

void InstructionEditorActionHandler::InsertInstructionAfter(const std::string &item_type)
{
  auto querry = CanInsertTypeAfterCurrentSelection(item_type);
  if (!querry.IsSuccess())
  {
    SendMessage(querry.GetMessage());
    return;
  }

  std::vector<std::unique_ptr<mvvm::SessionItem>> items;
  items.push_back(CreateInstructionTree(item_type));

  InsertAfterCurrentSelection(std::move(items));
}

bool InstructionEditorActionHandler::CanInsertInstructionInto(const std::string &item_type) const
{
  return CanInsertTypeIntoCurrentSelection(item_type).IsSuccess();
}

void InstructionEditorActionHandler::InsertInstructionInto(const std::string &item_type)
{
  auto querry = CanInsertTypeIntoCurrentSelection(item_type);
  if (!querry.IsSuccess())
  {
    SendMessage(querry.GetMessage());
    return;
  }

  std::vector<std::unique_ptr<mvvm::SessionItem>> items;
  items.push_back(CreateInstructionTree(item_type));

  InsertIntoCurrentSelection(std::move(items));
}

bool InstructionEditorActionHandler::CanRemoveInstruction() const
{
  const bool has_model = GetModel() != nullptr;
  const bool has_selection = GetSelectedInstruction() != nullptr;
  return has_model && has_selection;
}

void InstructionEditorActionHandler::RemoveInstruction()
{
  if (!CanRemoveInstruction())
  {
    return;
  }

  mvvm::utils::BeginMacro(*GetModel(), "Remove instruction");

  mvvm::SessionItem *next_to_select{nullptr};

  // remove children from the selection list to avoid double delete
  auto selected = mvvm::utils::CastItems<mvvm::SessionItem>(GetSelectedInstructions());
  for (auto item : sup::gui::FilterOutChildren(selected))
  {
    next_to_select = mvvm::utils::FindNextSiblingToSelect(item);
    GetModel()->RemoveItem(item);
  }

  UpdateProcedurePreamble();

  mvvm::utils::EndMacro(*GetModel());

  if (next_to_select)
  {
    // suggest to select something else instead of just deleted instruction
    SelectNotify(next_to_select);
  }
}

void InstructionEditorActionHandler::MoveUp()
{
  if (auto instruction = GetSelectedInstruction(); instruction)
  {
    if (mvvm::utils::MoveUp(*instruction))
    {
      SelectNotify(instruction);
    }
  }
}

void InstructionEditorActionHandler::MoveDown()
{
  if (auto instruction = GetSelectedInstruction(); instruction)
  {
    if (mvvm::utils::MoveDown(*instruction))
    {
      SelectNotify(instruction);
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
  RemoveInstruction();
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

  m_context.set_mime_data(CreateInstructionSelectionCopyMimeData(GetSelectedInstructions()));
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

  InsertAfterCurrentSelection(CreateInstructions(GetMimeData()));
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

  InsertIntoCurrentSelection(CreateInstructions(GetMimeData()));
}

void InstructionEditorActionHandler::InsertItem(const std::string &item_type,
                                                mvvm::SessionItem *parent,
                                                const mvvm::TagIndex &index)
{
  std::vector<std::unique_ptr<mvvm::SessionItem>> items;
  items.push_back(CreateInstructionTree(item_type));
  InsertItem(std::move(items), parent, index, GetCoordinateNearby(nullptr));
}

InstructionItem *InstructionEditorActionHandler::GetSelectedInstruction() const
{
  auto instructions = GetSelectedInstructions();
  return instructions.empty() ? nullptr : instructions.front();
}

std::vector<InstructionItem *> InstructionEditorActionHandler::GetSelectedInstructions() const
{
  return m_context.selected_instructions();
}

mvvm::ISessionModel *InstructionEditorActionHandler::GetModel() const
{
  return GetInstructionContainer() ? GetInstructionContainer()->GetModel() : nullptr;
}

mvvm::SessionItem *InstructionEditorActionHandler::GetInstructionContainer() const
{
  return m_context.instruction_container();
}

void InstructionEditorActionHandler::SelectNotify(mvvm::SessionItem *item) const
{
  m_context.notify_request(item);
}

std::unique_ptr<InstructionItem> InstructionEditorActionHandler::CreateInstructionTree(
    const std::string &item_type)
{
  return m_context.create_instruction(item_type);
}

void InstructionEditorActionHandler::SendMessage(const std::string &text,
                                                 const std::string &informative,
                                                 const std::string &details) const
{
  SendMessage(sup::gui::CreateInvalidOperationMessage(text, informative, details));
}

void InstructionEditorActionHandler::SendMessage(const sup::gui::MessageEvent &message_event) const
{
  if (m_context.send_message)
  {
    m_context.send_message(message_event);
  }
  else
  {
    throw RuntimeException(message_event.detailed);
  }
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
    std::vector<std::unique_ptr<mvvm::SessionItem>> items)
{
  auto selected_item = GetSelectedInstruction();
  auto parent = selected_item ? selected_item->GetParent() : GetInstructionContainer();
  auto tagindex = selected_item ? selected_item->GetTagIndex().Next() : mvvm::TagIndex::Append();
  InsertItem(std::move(items), parent, tagindex, GetCoordinateNearby(selected_item));
}

void InstructionEditorActionHandler::InsertIntoCurrentSelection(
    std::vector<std::unique_ptr<mvvm::SessionItem>> items)
{
  auto selected_item = GetSelectedInstruction();
  InsertItem(std::move(items), selected_item, mvvm::TagIndex::Append(),
             GetCoordinateNearby(selected_item));
}

void InstructionEditorActionHandler::InsertItem(
    std::vector<std::unique_ptr<mvvm::SessionItem>> items, mvvm::SessionItem *parent,
    const mvvm::TagIndex &index, const position_t &position)
{
  if (!GetModel())
  {
    throw RuntimeException("Uninitialised model");
  }

  if (parent == nullptr)
  {
    throw RuntimeException("Uninitialised parent");
  }

  mvvm::utils::BeginMacro(*GetModel(), "Insert instruction");

  auto last_tag_index = index;
  std::vector<mvvm::SessionItem *> to_notify;
  for (auto &item : items)
  {
    mvvm::SessionItem *result{nullptr};
    const auto item_type = item->GetType();
    try
    {
      result = GetModel()->InsertItem(std::move(item), parent, last_tag_index);
      to_notify.push_back(result);
      last_tag_index = result->GetTagIndex().Next();
      UpdateProcedurePreamble();
      AlignInstructionTree(position, result);
    }
    catch (const std::exception &ex)
    {
      std::ostringstream ostr;
      ostr << "Exception was caught while trying to insert instruction [" << item_type
           << "] into parent [" << parent->GetType() << "]";
      SendMessage("Can't insert instruction", ostr.str(), ex.what());
    }
  }

  mvvm::utils::EndMacro(*GetModel());

  for (auto item : to_notify)
  {
    SelectNotify(item);
  }
}

}  // namespace oac_tree_gui
