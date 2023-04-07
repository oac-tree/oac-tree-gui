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

#include "sequencer_observer.h"

#include <sequencergui/jobsystem/job_log_severity.h>
#include <sequencergui/jobsystem/log_event.h>
#include <sequencergui/jobsystem/procedure_runner.h>
#include <sequencergui/jobsystem/request_types.h>

#include <sup/gui/model/anyvalue_utils.h>
#include <sup/sequencer/instruction.h>

#include <sstream>

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
  m_procedure_runner->OnInstructionStatusChange(instruction, GetStatus(instruction));
}

void SequencerObserver::VariableUpdatedImpl(const std::string &name,
                                            const sup::dto::AnyValue &value, bool connected)
{
  (void)connected;
  auto value_string = sup::gui::ValuesToJSONString(value);
  std::ostringstream ostr;
  ostr << "VariableUpdatedImpl(): " << name << ", " << value_string;
  m_procedure_runner->OnLogEvent(CreateLogEvent(Severity::kDebug, ostr.str()));
}

bool SequencerObserver::PutValueImpl(const sup::dto::AnyValue &value,
                                     const std::string &description)
{
  auto value_string = sup::gui::ValuesToJSONString(value);
  std::ostringstream ostr;
  ostr << "PutValueImpl(): " << description << ", " << value_string;
  m_procedure_runner->OnLogEvent(CreateLogEvent(Severity::kInfo, ostr.str()));
  return true;
}

bool SequencerObserver::GetUserValueImpl(sup::dto::AnyValue &value, const std::string &description)
{
  auto result = m_procedure_runner->OnUserInput({value, description});
  value = result.value;
  return result.processed;
}

int SequencerObserver::GetUserChoiceImpl(const std::vector<std::string> &choices,
                                         const std::string &description)
{
  return m_procedure_runner->OnUserChoice({choices, description}).index;
}

void SequencerObserver::StartSingleStepImpl()
{
  m_procedure_runner->OnLogEvent(CreateLogEvent(Severity::kDebug, "StartSingleStep()"));
}

void SequencerObserver::EndSingleStepImpl() {}

void SequencerObserver::MessageImpl(const std::string &message)
{
  m_procedure_runner->OnLogEvent(CreateLogEvent(Severity::kInfo, message));
}

void SequencerObserver::LogImpl(int severity, const std::string &message)
{
  // assuming sequencer severity is the same as GUI severity
  m_procedure_runner->OnLogEvent(CreateLogEvent(static_cast<Severity>(severity), message));
}

}  // namespace sequencergui
