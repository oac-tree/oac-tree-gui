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

#ifndef SEQUENCERGUI_JOBSYSTEM_SEQUENCER_OBSERVER_H_
#define SEQUENCERGUI_JOBSYSTEM_SEQUENCER_OBSERVER_H_

#include <sequencergui/model/dto_types_fwd.h>
#include <sup/sequencer/user_interface.h>

namespace sequencergui
{
class ProcedureRunner;

//! Sequencer observer which will be invoked during domain Procedure execution.
//! Reports to ProcedureRunner.

class SequencerObserver : public ::sup::sequencer::UserInterface
{
public:
  explicit SequencerObserver(ProcedureRunner* procedure_runner);
  ~SequencerObserver() override;

  void UpdateInstructionStatusImpl(const ::sup::sequencer::Instruction* instruction) override;

  void VariableUpdatedImpl(const std::string& name, const sup::dto::AnyValue& value) override;

  bool PutValueImpl(const sup::dto::AnyValue& value, const std::string& description) override;

  bool GetUserValueImpl(sup::dto::AnyValue& value, const std::string& description) override;

  int GetUserChoiceImpl(const std::vector<std::string>& choices,
                        const std::string& description) override;
  void StartSingleStepImpl() override;
  void EndSingleStepImpl() override;

  void MessageImpl(const std::string& message) override;

private:
  ProcedureRunner* m_procedure_runner{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_SEQUENCER_OBSERVER_H_
