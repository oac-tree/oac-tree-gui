/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#ifndef SEQUENCERGUI_MONITOR_SEQUENCEROBSERVER_H
#define SEQUENCERMONITORCORE_SEQUENCEROBSERVER_H

#include "UserInterface.h"

namespace sequi
{
class ProcedureRunner;

//! Sequencer observer which will be invoked during domain Procedure execution.
//! Reports to ProcedureRunner.

class SequencerObserver : public ::sup::sequencer::UserInterface
{
public:
  SequencerObserver(ProcedureRunner* procedure_runner);
  ~SequencerObserver();

  void UpdateInstructionStatusImpl(const ::sup::sequencer::Instruction* instruction) override;

  void VariableUpdatedImpl(const std::string& name, const ccs::types::AnyValue& value) override;

  bool PutValueImpl(const ::ccs::types::AnyValue& value, const std::string& description) override;

  bool GetUserValueImpl(::ccs::types::AnyValue& value, const std::string& description) override;

  int GetUserChoiceImpl(const std::vector<std::string>& choices,
                        const std::string& description) override;
  void StartSingleStepImpl() override;
  void EndSingleStepImpl() override;

private:
  ProcedureRunner* m_procedure_runner{nullptr};
};

}  // namespace sequi

#endif  // SEQUENCERGUI_MONITOR_SEQUENCEROBSERVER_H
