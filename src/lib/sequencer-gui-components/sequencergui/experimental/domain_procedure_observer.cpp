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

#include <sequencergui/core/exceptions.h>

#include <sup/sequencer/instruction.h>

namespace sequencergui
{

DomainProcedureObserver::DomainProcedureObserver(post_event_callback_t post_event_callback)
    : m_post_event_callback(std::move(post_event_callback))
{
  if (!m_post_event_callback)
  {
    throw RuntimeException("Callback is not initialised");
  }
}

void DomainProcedureObserver::UpdateInstructionStatus(
    const sup::sequencer::Instruction *instruction)
{
  (void)(instruction);
  m_post_event_callback(InstructionStatusChanged{instruction, instruction->GetStatus()});
}

void DomainProcedureObserver::VariableUpdated(const std::string &name,
                                              const sup::dto::AnyValue &value, bool connected)
{
  (void)(name);
  (void)(value);
  (void)(connected);
}

bool DomainProcedureObserver::PutValue(const sup::dto::AnyValue &value,
                                       const std::string &description)
{
  (void)(value);
  (void)(description);
  return false;
}

bool DomainProcedureObserver::GetUserValue(sup::dto::AnyValue &value,
                                           const std::string &description)
{
  (void)(value);
  (void)(description);
  return false;
}

int DomainProcedureObserver::GetUserChoice(const std::vector<std::string> &options,
                                           const sup::dto::AnyValue &metadata)
{
  (void)(options);
  (void)(metadata);
  return 0;
}

void DomainProcedureObserver::Message(const std::string &message)
{
  (void)(message);
}

void DomainProcedureObserver::Log(int severity, const std::string &message)
{
  (void)(severity);
  (void)(message);
}

}  // namespace sequencergui
