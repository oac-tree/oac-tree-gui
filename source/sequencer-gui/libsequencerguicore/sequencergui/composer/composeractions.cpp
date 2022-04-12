/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/composer/composeractions.h"

#include "sequencergui/core/exceptions.h"
#include "sequencergui/core/messagehandlerinterface.h"
#include "sequencergui/model/sequenceritems.h"
#include "sequencergui/model/sequencermodel.h"

#include "mvvm/core/exceptions.h"

namespace sequencergui
{

ComposerActions::ComposerActions(SequencerModel *model, QObject *parent) : QObject(parent)
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
void ComposerActions::InsertInstructionAfterRequest(const QString &item_type)
{
  if (!m_model)
  {
    throw NullException("Model is not defined");
  }

  if (!m_context.m_selected_instruction || !m_context.m_selected_procedure)
  {
    throw RuntimeException("Callbacks are not defined");
  }

  auto selected_procedure = m_context.m_selected_procedure();
  auto selected_instruction = m_context.m_selected_instruction();

  if (selected_instruction)
  {
    m_model->InsertNewItem(item_type.toStdString(), selected_instruction->GetParent(),
                           selected_instruction->GetTagIndex().Next());
  }
  else
  {
    m_model->InsertNewItem(item_type.toStdString(), selected_procedure->GetInstructionContainer(),
                           mvvm::TagIndex::Append());
  }
}

//! Inserts new instruction of given type after the current selection.
//! The selection is retrieved via a callback.
void ComposerActions::InsertInstructionIntoRequest(const QString &item_type)
{
  if (!m_model)
  {
    throw NullException("Model is not defined");
  }

  if (!m_context.m_selected_instruction || !m_context.m_selected_procedure)
  {
    throw RuntimeException("Callbacks are not defined");
  }

  auto selected_instruction = m_context.m_selected_instruction();

  if (selected_instruction)
  {
    try
    {
      m_model->InsertNewItem(item_type.toStdString(), selected_instruction, mvvm::TagIndex::Append());
    }
    catch(const mvvm::InvalidInsertException& ex)
    {
      if (m_message_handler)
      {
        m_message_handler->SendMessage(ex.what());
      }
      else
      {
        throw ex;
      }
    }

  }
}

}  // namespace sequencergui
