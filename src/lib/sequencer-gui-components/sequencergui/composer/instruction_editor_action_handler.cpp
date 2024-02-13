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

#include "instruction_editor_action_handler.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/instruction_item.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/nodeeditor/scene_utils.h>
#include <sequencergui/transform/transform_from_domain.h>

#include <mvvm/core/exceptions.h>
#include <mvvm/model/model_utils.h>

#include <QPointF>
#include <sstream>

namespace
{

/**
 * @brief Updates child coordinate so it is located near the reference
 */
void UpdateChildCoordinate(const sequencergui::InstructionItem *reference, mvvm::SessionItem *child)
{
  const int coordinate_offset = 10;  // to place a child aproximately near the parent
  const auto default_center = sequencergui::GetGraphicsViewportCenter();
  if (auto inserted_instruction = dynamic_cast<sequencergui::InstructionItem *>(child);
      inserted_instruction)
  {
    inserted_instruction->SetX(reference ? reference->GetX() + coordinate_offset
                                         : default_center.x());
    inserted_instruction->SetY(reference ? reference->GetY() + coordinate_offset
                                         : default_center.y());
  }
}

}  // namespace

namespace sequencergui
{

InstructionEditorActionHandler::InstructionEditorActionHandler(InstructionEditorContext context,
                                                               QObject *parent)
    : QObject(parent), m_context(std::move(context))
{
  if (!m_context.selected_procedure)
  {
    throw RuntimeException("Callback to retrieve current procedure is not defined");
  }

  if (!m_context.selected_instruction)
  {
    throw RuntimeException("Callback to get selected instruction is not defined");
  }

  if (!m_context.send_message_callback)
  {
    throw RuntimeException("Callback to send messages is not set");
  }
}

InstructionEditorActionHandler::~InstructionEditorActionHandler() = default;

void InstructionEditorActionHandler::OnInsertInstructionAfterRequest(const QString &item_type)
{
  auto instruction_container = GetInstructionContainer();
  if (!instruction_container)
  {
    SendMessage("No procedure selected");
    return;
  }

  auto item = GetSelectedInstruction();

  auto parent = item ? item->GetParent() : instruction_container;
  auto tagindex = item ? item->GetTagIndex().Next() : mvvm::TagIndex::Append();

  auto child = InsertItem(item_type.toStdString(), parent, tagindex);
  UpdateChildCoordinate(item, child);
}

void InstructionEditorActionHandler::OnInsertInstructionIntoRequest(const QString &item_type)
{
  auto selected_instruction = GetSelectedInstruction();
  auto child = InsertItem(item_type.toStdString(), selected_instruction, mvvm::TagIndex::Append());
  UpdateChildCoordinate(selected_instruction, child);
}

void InstructionEditorActionHandler::OnRemoveInstructionRequest()
{
  if (auto selected_instruction = GetSelectedInstruction(); selected_instruction)
  {
    GetModel()->RemoveItem(selected_instruction);
  }
}

void InstructionEditorActionHandler::OnMoveUpRequest()
{
  if (auto instruction = GetSelectedInstruction(); instruction)
  {
    mvvm::utils::MoveUp(*instruction);
    emit SelectItemRequest(instruction);
  }
}

void InstructionEditorActionHandler::OnMoveDownRequest()
{
  if (auto instruction = GetSelectedInstruction(); instruction)
  {
    mvvm::utils::MoveDown(*instruction);
    emit SelectItemRequest(instruction);
  }
}

InstructionItem *InstructionEditorActionHandler::GetSelectedInstruction()
{
  return m_context.selected_instruction();
}

mvvm::SessionModelInterface *InstructionEditorActionHandler::GetModel() const
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
                                                 const std::string &details)
{
  auto message = sup::gui::CreateInvalidOperationMessage(text, informative, details);
  m_context.send_message_callback(message);
}

mvvm::SessionItem *InstructionEditorActionHandler::InsertItem(const std::string &item_type,
                                                              mvvm::SessionItem *parent,
                                                              const mvvm::TagIndex &index)
{
  mvvm::SessionItem *result{nullptr};
  try
  {
    result = GetModel()->InsertItem(CreateInstructionItem(item_type), parent, index);
    emit SelectItemRequest(result);
  }
  catch (const std::exception &ex)
  {
    std::ostringstream ostr;
    ostr << "Can't insert variable '" << item_type << "' into parent '" << parent->GetType()
         << "'. Maximum allowed number of children exceeded?";
    SendMessage(ostr.str());
  }
  return result;
}

}  // namespace sequencergui
