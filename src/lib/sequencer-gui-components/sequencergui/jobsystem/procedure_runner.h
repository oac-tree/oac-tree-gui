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

#ifndef SEQUENCERGUI_JOBSYSTEM_PROCEDURE_RUNNER_H_
#define SEQUENCERGUI_JOBSYSTEM_PROCEDURE_RUNNER_H_

#include <sequencergui/domain/sequencer_types_fwd.h>
#include <sequencergui/jobsystem/job_log_severity.h>
#include <sequencergui/jobsystem/job_types.h>
#include <sequencergui/jobsystem/user_controller.h>

#include <QObject>
#include <memory>

namespace sequencergui
{
class SequencerObserver;
class DomainRunnerAdapter;
class LogEvent;
class UserChoiceProvider;
struct UserChoiceResult;

//! Executes sequencer's procedure in a separate thread.
//! Observes sequence execution and converts sequencer UI calls into signals.
//! These signals must be connected with the rest of the GUI via queed connections.

class ProcedureRunner : public QObject
{
  Q_OBJECT

public:
  explicit ProcedureRunner(procedure_t* procedure, QObject* parent = nullptr);
  ~ProcedureRunner() override;

  // Methods to control flow.

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

  void OnLogEvent(const LogEvent& event);

  std::string onUserInput(const std::string& current_value, const std::string& description);

  UserChoiceResult onUserChoice(const UserChoiceArgs& args);

signals:
  void InstructionStatusChanged(const instruction_t* instruction, const QString& status);
  void LogEventReceived(const sequencergui::LogEvent& event);
  void RunnerStatusChanged();
  void VariableChanged(const QString& variable_name, const QString& value);

private:
  std::unique_ptr<SequencerObserver> m_observer;
  std::unique_ptr<DomainRunnerAdapter> m_domain_runner_adapter;
  std::unique_ptr<UserChoiceProvider> m_choice_provider;
  UserController m_user_controller;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_PROCEDURE_RUNNER_H_
