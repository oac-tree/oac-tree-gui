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

#ifndef SEQUENCERGUI_JOBSYSTEM_SEQUENCER_OBSERVER_H_
#define SEQUENCERGUI_JOBSYSTEM_SEQUENCER_OBSERVER_H_

#include <sup/gui/core/dto_types_fwd.h>

#include <sup/sequencer/user_interface.h>

namespace sequencergui
{
class ProcedureReporter;

//! Sequencer observer which will be invoked during domain Procedure execution.
//! Reports to ProcedureRunner.

class SequencerObserver : public ::sup::sequencer::UserInterface
{
public:
  explicit SequencerObserver(ProcedureReporter* procedure_runner);
  ~SequencerObserver() override;

  void UpdateInstructionStatus(const ::sup::sequencer::Instruction* instruction) override;

  void VariableUpdated(const std::string& name, const sup::dto::AnyValue& value,
                       bool connected) override;

  bool PutValue(const sup::dto::AnyValue& value, const std::string& description) override;

  bool GetUserValue(sup::dto::AnyValue& value, const std::string& description) override;

  int GetUserChoice(const std::vector<std::string>& options,
                    const sup::dto::AnyValue& metadata) override;

  void Message(const std::string& message) override;

  void Log(int severity, const std::string& message) override;

private:
  ProcedureReporter* m_procedure_reporter{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_SEQUENCER_OBSERVER_H_
