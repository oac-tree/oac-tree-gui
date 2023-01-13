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

#include "sequencergui/jobsystem/sequencer_observer.h"

#include <sequencergui/jobsystem/procedure_runner.h>

#include <sup/gui/dto/anyvalue_utils.h>
#include <sup/sequencer/instruction.h>

namespace
{
std::string GetStatus(const instruction_t *instruction)
{
  return ::sup::sequencer::StatusToString(instruction->GetStatus());
}
}  // namespace

namespace sequencergui
{
SequencerObserver::SequencerObserver(ProcedureRunner *procedure_runner)
    : m_procedure_runner(procedure_runner)
{
}

SequencerObserver::~SequencerObserver() = default;

void SequencerObserver::UpdateInstructionStatusImpl(const sup::sequencer::Instruction *instruction)
{
  m_procedure_runner->onInstructionStatusChange(instruction, GetStatus(instruction));
}

void SequencerObserver::VariableUpdatedImpl(const std::string &name,
                                            const sup::dto::AnyValue &value)
{
  auto value_string = sup::gui::GetValuesToJSONString(&value);
  m_procedure_runner->onVariableChange(name, value_string);
}

bool SequencerObserver::PutValueImpl(const sup::dto::AnyValue &value,
                                     const std::string &description)
{
  auto value_string = sup::gui::GetValuesToJSONString(&value);
  m_procedure_runner->onLogMessage(description + value_string, JobMessageType::kHighlight);
  return true;
}

bool SequencerObserver::GetUserValueImpl(sup::dto::AnyValue &value, const std::string &description)
{
  auto value_string = sup::gui::GetValuesToJSONString(&value);
  auto user_input = m_procedure_runner->onUserInput(value_string, description);
  if (!sup::gui::ParseStringToScalarAnyvalue(user_input, value))
  {
    throw std::runtime_error(
        "Error in SequencerObserver::GetUserValueImpl: can't parse user input");
  }
  return true;
}

int SequencerObserver::GetUserChoiceImpl(const std::vector<std::string> &choices,
                                         const std::string &description)
{
  return m_procedure_runner->onUserChoice(choices, description);
}

void SequencerObserver::StartSingleStepImpl() {}

void SequencerObserver::EndSingleStepImpl() {}

void SequencerObserver::MessageImpl(const std::string &message)
{
  m_procedure_runner->onLogMessage(message, JobMessageType::kHighlight);
}

}  // namespace sequencergui
