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

#ifndef SEQUENCERGUI_MONITOR_SEQUENCEROBSERVER_H
#define SEQUENCERGUI_MONITOR_SEQUENCEROBSERVER_H

#include "UserInterface.h"

namespace sequencergui
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

  void MessageImpl(const std::string& message) override;

private:
  ProcedureRunner* m_procedure_runner{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MONITOR_SEQUENCEROBSERVER_H
