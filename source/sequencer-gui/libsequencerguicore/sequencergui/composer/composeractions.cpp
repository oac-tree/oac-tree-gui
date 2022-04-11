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
#include "sequencergui/model/sequenceritems.h"
#include "sequencergui/model/sequencermodel.h"

namespace sequencergui
{

ComposerActions::ComposerActions(SequencerModel *model, QObject *parent) : QObject(parent)
{
  SetModel(model);
}

void ComposerActions::SetModel(SequencerModel *model)
{
  m_model = model;
}

void ComposerActions::SetContext(ComposerContext context)
{
  m_context = std::move(context);
}

//! Inserts new instruction of given type after the current selection.
//! The selection is retrieved via a callback.
void ComposerActions::InsertInstructionAfterRequest(const std::string &item_type)
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
    m_model->InsertNewItem(item_type, selected_instruction->GetParent(),
                           selected_instruction->GetTagIndex().Next());
  }
  else
  {
    m_model->InsertNewItem(item_type, selected_procedure->GetInstructionContainer(),
                           mvvm::TagIndex::Append());
  }
}

//! Inserts new instruction of given type after the current selection.
//! The selection is retrieved via a callback.
void ComposerActions::InsertInstructionIntoRequest(const std::string &item_type)
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
    m_model->InsertNewItem(item_type, selected_instruction, mvvm::TagIndex::Append());
  }
}

}  // namespace sequencergui
