/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
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

bool SequencerObserver::PutValueImpl(const ccs::types::AnyValue &, const std::string &)
{
  return false;
}

bool SequencerObserver::GetUserValueImpl(ccs::types::AnyValue &, const std::string &)
{
  return false;
}

int SequencerObserver::GetUserChoiceImpl(const std::vector<std::string> &, const std::string &)
{
  return 0;
}

void SequencerObserver::StartSingleStepImpl()
{
  m_procedure_runner->onLogMessage("StartSingleStepImpl");
}

void SequencerObserver::EndSingleStepImpl()
{
  m_procedure_runner->onLogMessage("EndSingleStepImpl");
}

}  // namespace sequi
