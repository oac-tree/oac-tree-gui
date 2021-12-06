/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/monitor/sequencerobserver.h"

#include "Instruction.h"
#include "sequencergui/model/shallnotbenamedvalueutils.h"
#include "sequencergui/monitor/procedurerunner.h"

namespace sequi
{
SequencerObserver::SequencerObserver(ProcedureRunner *procedure_runner)
    : m_procedure_runner(procedure_runner)
{
}

SequencerObserver::~SequencerObserver() {}

void SequencerObserver::UpdateInstructionStatusImpl(const sup::sequencer::Instruction *instruction)
{
  m_procedure_runner->onLogMessage("UpdateInstructionStatusImpl");
  m_procedure_runner->onInstructionStatusChange(instruction);
}

void SequencerObserver::VariableUpdatedImpl(const std::string &name,
                                            const ccs::types::AnyValue &value)
{
  auto value_string = DomainUtils::GetJsonString(&value);
  m_procedure_runner->onVariableChange(name, value_string);
}

bool SequencerObserver::PutValueImpl(const ccs::types::AnyValue &value,
                                     const std::string &description)
{
  auto value_string = DomainUtils::GetJsonString(&value);
  m_procedure_runner->onLogMessage(description + value_string, MessageType::kHighlight);
  return true;
}

bool SequencerObserver::GetUserValueImpl(ccs::types::AnyValue &, const std::string &)
{
  return false;
}

int SequencerObserver::GetUserChoiceImpl(const std::vector<std::string> &, const std::string &)
{
  return 0;
}

void SequencerObserver::StartSingleStepImpl() {}

void SequencerObserver::EndSingleStepImpl() {}

void SequencerObserver::MessageImpl(const std::string &message)
{
  m_procedure_runner->onLogMessage(message, MessageType::kHighlight);
}

}  // namespace sequi
