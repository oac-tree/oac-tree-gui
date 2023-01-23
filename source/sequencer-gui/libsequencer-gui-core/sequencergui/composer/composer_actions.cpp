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

#include "sequencergui/composer/composer_actions.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/core/message_handler_factory.h>
#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/instruction_item.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/model/variable_item.h>
#include <sequencergui/model/workspace_item.h>
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

ComposerActions::ComposerActions(SequencerModel *model, QObject *parent)
    : QObject(parent), m_message_handler(CreateNullMessageHandler())
{
  SetModel(model);
}

ComposerActions::~ComposerActions() = default;

void ComposerActions::SetModel(SequencerModel *model)
{
  m_model = model;
}

void ComposerActions::SetContext(ComposerContext context)
{
  m_context = std::move(context);
}

void ComposerActions::SetMessageHandler(std::unique_ptr<MessageHandlerInterface> message_handler)
{
  m_message_handler = std::move(message_handler);
}

//! Inserts new instruction of given type after the current selection.
//! The selection is retrieved via a callback.
void ComposerActions::OnInsertInstructionAfterRequest(const QString &item_type)
{
  ValidatePrecoditions();

  auto procedure = m_context.selected_procedure();
  if (!procedure)
  {
    m_message_handler->SendMessage("No procedure selected");
    return;
  }

  auto item = m_context.selected_instruction();

  auto parent = item ? item->GetParent() : procedure->GetInstructionContainer();
  auto tagindex = item ? item->GetTagIndex().Next() : mvvm::TagIndex::Append();

  auto child = InsertItem(item_type.toStdString(), parent, tagindex);
  UpdateChildCoordinate(item, child);
}

//! Inserts new instruction of given type after the current selection.
//! The selection is retrieved via a callback.
void ComposerActions::OnInsertInstructionIntoRequest(const QString &item_type)
{
  ValidatePrecoditions();

  auto selected_instruction = m_context.selected_instruction();
  auto child = InsertItem(item_type.toStdString(), selected_instruction, mvvm::TagIndex::Append());
  UpdateChildCoordinate(selected_instruction, child);
}

//! Removes currently selected instruction.
//! The selection is retrieved via a callback.
void ComposerActions::OnRemoveInstructionRequest()
{
  auto selected_instruction = m_context.selected_instruction();

  if (selected_instruction)
  {
    m_model->RemoveItem(selected_instruction);
  }
}

void ComposerActions::OnInsertVariableAfterRequest(const QString &item_type)
{
  ValidatePrecoditions();

  auto procedure = m_context.selected_procedure();
  auto item = m_context.selected_variable();

  auto parent = item ? item->GetParent() : procedure->GetWorkspace();
  auto tagindex = item ? item->GetTagIndex().Next() : mvvm::TagIndex::Append();

  InsertItem(item_type.toStdString(), parent, tagindex);
}

void ComposerActions::OnRemoveVariableRequest()
{
  if (auto selected = m_context.selected_variable(); selected)
  {
    m_model->RemoveItem(selected);
  }
}

mvvm::SessionItem *ComposerActions::InsertItem(const std::string &item_type,
                                               mvvm::SessionItem *parent,
                                               const mvvm::TagIndex &index)
{
  mvvm::SessionItem *result{nullptr};
  try
  {
    result = m_model->InsertItem(m_model->GetFactory()->CreateItem(item_type), parent, index);
  }
  catch (const std::exception &ex)
  {
    std::ostringstream ostr;
    ostr << "Can't insert variable '" << item_type << "' into parent '" << parent->GetType()
         << "'. Maximum allowed number of children exceeded?";
    m_message_handler->SendMessage(ostr.str());
  }
  return result;
}

void ComposerActions::ValidatePrecoditions()
{
  if (!m_model)
  {
    throw NullException("Model is not defined");
  }

  if (!m_context.selected_variable || !m_context.selected_procedure)
  {
    throw RuntimeException("Callbacks are not defined");
  }
}

}  // namespace sequencergui
