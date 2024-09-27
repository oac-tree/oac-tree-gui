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

#include "domain_procedure_observer.h"

#include "user_choice_provider.h"
#include "user_context.h"
#include "user_input_provider.h"

#include <sequencergui/core/exceptions.h>
#include <sup/gui/model/anyvalue_utils.h>

#include <mvvm/utils/string_utils.h>

#include <sup/sequencer/instruction.h>

#include <sstream>

namespace sequencergui
{

DomainProcedureObserver::DomainProcedureObserver(post_event_callback_t post_event_callback,
                                                 const UserContext &user_context)
    : m_post_event_callback(std::move(post_event_callback))
{
  if (!m_post_event_callback)
  {
    throw RuntimeException("Callback is not initialised");
  }

  if (user_context.user_choice_callback)
  {
    m_choice_provider = std::make_unique<UserChoiceProvider>(user_context.user_choice_callback);
  }

  if (user_context.user_input_callback)
  {
    m_input_provider = std::make_unique<UserInputProvider>(user_context.user_input_callback);
  }
}

DomainProcedureObserver::~DomainProcedureObserver() = default;

void DomainProcedureObserver::UpdateInstructionStatus(
    const sup::sequencer::Instruction *instruction)
{
  (void)(instruction);
  m_post_event_callback(InstructionStatusChangedEvent{instruction, instruction->GetStatus()});
}

void DomainProcedureObserver::VariableUpdated(const std::string &name,
                                              const sup::dto::AnyValue &value, bool connected)
{
  m_post_event_callback(WorkspaceEvent{name, value, connected});
}

bool DomainProcedureObserver::PutValue(const sup::dto::AnyValue &value,
                                       const std::string &description)
{
  auto value_string = sup::gui::ValuesToJSONString(value);
  std::ostringstream ostr;
  ostr << description << " " << value_string;
  m_post_event_callback(CreateLogEvent(Severity::kInfo, ostr.str()));
  return true;
}

bool DomainProcedureObserver::GetUserValue(sup::dto::AnyValue &value,
                                           const std::string &description)
{
  if (m_input_provider)
  {
    auto result = m_input_provider->GetUserInput({value, description});
    value = result.value;
    return result.processed;
  }

  m_post_event_callback(CreateLogEvent(Severity::kWarning, "User input callback is not set"));
  return false;
}

int DomainProcedureObserver::GetUserChoice(const std::vector<std::string> &options,
                                           const sup::dto::AnyValue &metadata)
{
  if (m_choice_provider)
  {
    auto result = m_choice_provider->GetUserChoice({options, metadata});
    return result.processed ? result.index : -1;
  }

  m_post_event_callback(CreateLogEvent(Severity::kWarning, "User choice callback is not set"));
  return -1;
}

void DomainProcedureObserver::Message(const std::string &message)
{
  m_post_event_callback(CreateLogEvent(Severity::kInfo, message));
}

void DomainProcedureObserver::Log(int severity, const std::string &message)
{
  // assuming sequencer severity is the same as GUI severity
  m_post_event_callback(CreateLogEvent(static_cast<Severity>(severity), message));
}

}  // namespace sequencergui
