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

#include "instruction_editor_actions.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/instruction_item.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/nodeeditor/scene_utils.h>

#include <mvvm/core/exceptions.h>

#include <QDebug>
#include <QPointF>
#include <sstream>

namespace
{

//! Update child coordinate so it is located near the reference
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

InstructionEditorActions::InstructionEditorActions(InstructionEditorContext context,
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

InstructionEditorActions::~InstructionEditorActions() = default;

//! Inserts new instruction of given type after the current selection.
//! The selection is retrieved via a callback.
void InstructionEditorActions::OnInsertInstructionAfterRequest(const QString &item_type)
{
  auto instruction_container = GetInstructionContainer();
  if (!instruction_container)
  {
    SendMessage("No procedure selected");
    return;
  }

  auto item = m_context.selected_instruction();

  auto parent = item ? item->GetParent() : instruction_container;
  auto tagindex = item ? item->GetTagIndex().Next() : mvvm::TagIndex::Append();

  auto child = InsertItem(item_type.toStdString(), parent, tagindex);
  UpdateChildCoordinate(item, child);
}

//! Inserts new instruction of given type after the current selection.
//! The selection is retrieved via a callback.
void InstructionEditorActions::OnInsertInstructionIntoRequest(const QString &item_type)
{
  auto selected_instruction = m_context.selected_instruction();
  auto child = InsertItem(item_type.toStdString(), selected_instruction, mvvm::TagIndex::Append());
  UpdateChildCoordinate(selected_instruction, child);
}

//! Removes currently selected instruction.
//! The selection is retrieved via a callback.
void InstructionEditorActions::OnRemoveInstructionRequest()
{
  if (auto selected_instruction = m_context.selected_instruction(); selected_instruction)
  {
    GetModel()->RemoveItem(selected_instruction);
  }
}

mvvm::SessionModelInterface *InstructionEditorActions::GetModel() const
{
  return GetInstructionContainer() ? GetInstructionContainer()->GetModel() : nullptr;
}

InstructionContainerItem *InstructionEditorActions::GetInstructionContainer() const
{
  auto procedure = m_context.selected_procedure();
  return procedure ? procedure->GetInstructionContainer() : nullptr;
}

void InstructionEditorActions::SendMessage(const std::string &text, const std::string &informative,
                                           const std::string &details)
{
  auto message = sup::gui::CreateInvalidOperationMessage(text, informative, details);
  m_context.send_message_callback(message);
}

mvvm::SessionItem *InstructionEditorActions::InsertItem(const std::string &item_type,
                                                        mvvm::SessionItem *parent,
                                                        const mvvm::TagIndex &index)
{
  mvvm::SessionItem *result{nullptr};
  try
  {
    result = GetModel()->InsertItem(GetModel()->GetFactory()->CreateItem(item_type), parent, index);
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
