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

#ifndef SEQUENCERGUI_MONITOR_PROCEDURERUNNER_H
#define SEQUENCERGUI_MONITOR_PROCEDURERUNNER_H

#include "sequencergui/jobsystem/job_types.h"
#include "sequencergui/jobsystem/user_controller.h"
#include "sequencergui/model/sequencer_types.h"

#include <QObject>
#include <memory>

namespace sequencergui
{
class SequencerObserver;
class DomainRunnerAdapter;

//! Executes sequencer's procedure in a separate thread.
//! Observes sequence execution and converts sequencer UI calls into signals.
//! These signals must be connected with the rest of the GUI via queed connections.

class ProcedureRunner : public QObject
{
  Q_OBJECT

public:
  explicit ProcedureRunner(QObject* parent = nullptr);
  ~ProcedureRunner() override;

  // Methods to control flow.

  void SetProcedure(procedure_t* procedure);

  bool Start();

  bool Step();

  bool Stop();

  bool Pause();

  void SetSleepTime(int time_msec);

  void SetUserContext(const UserContext& user_context);

  // Methods to retrieve status

  bool IsBusy() const;

  RunnerStatus GetRunnerStatus() const;

  // Methods used by the domain to to report the activity.

  void onInstructionStatusChange(const instruction_t* instruction, const std::string& value);

  void onLogMessage(const std::string& message,
                    JobMessageType message_type = JobMessageType::kDefault);

  void onVariableChange(const std::string& variable_name, const std::string& value);

  std::string onUserInput(const std::string& current_value, const std::string& description);

  int onUserChoice(const std::vector<std::string>& choices, const std::string& description);

signals:
  void InstructionStatusChanged(const instruction_t* instruction, const QString& status);
  void LogMessageRequest(const QString& message, int message_type);
  void RunnerStatusChanged();
  void VariableChanged(const QString& variable_name, const QString& value);

private:
  void CheckConditions() const;

  std::unique_ptr<SequencerObserver> m_observer;
  std::unique_ptr<DomainRunnerAdapter> m_domain_runner_adapter;
  UserController m_user_controller;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MONITOR_PROCEDURERUNNER_H
