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

#ifndef SEQUENCERGUI_JOBSYSTEM_PROCEDURE_REPORTER_H_
#define SEQUENCERGUI_JOBSYSTEM_PROCEDURE_REPORTER_H_

#include <sequencergui/domain/sequencer_types_fwd.h>
#include <sequencergui/jobsystem/job_types.h>
#include <sequencergui/jobsystem/user_context.h>

#include <QObject>
#include <memory>

namespace sequencergui
{
class SequencerObserver;
class LogEvent;
class UserChoiceProvider;
class UserInputProvider;
struct UserChoiceResult;

//! Reports events happening in the running sequencer procedure to the GUI, handles input requests.

class ProcedureReporter : public QObject
{
  Q_OBJECT

public:
  explicit ProcedureReporter(QObject* parent = nullptr);
  ~ProcedureReporter() override;

  void SetUserContext(const UserContext& user_context);

  void OnInstructionStatusChange(const instruction_t* instruction, const std::string& value);

  void OnLogEvent(const LogEvent& event);

  UserInputResult OnUserInput(const UserInputArgs& args);

  UserChoiceResult OnUserChoice(const UserChoiceArgs& args);

  SequencerObserver* GetObserver();

  /**
   * @brief Processes the change of status of the domain runner.
   *
   * @param status The status of the domain runner adapter.
   *
   * @details Should be called from the sequencer thread.
   */
  void OnDomainRunnerStatusChanged(RunnerStatus status);

  /**
   * @brief Process the tick of the domain runner and reports
   *
   * @details Should be called from the sequencer thread.
   */
  void OnDomainProcedureTick(const procedure_t& procedure);

signals:
  void InstructionStatusChanged(const instruction_t* instruction, const QString& status);
  void LogEventReceived(const sequencergui::LogEvent& event);

  /**
   * @brief The signal will be emited when domain runner adapter has chainged its status.
   *
   * @param status The status of the domain runner adapter.
   *
   * @details Must be connected with the GUI thread via queued connection.
   */
  void RunnerStatusChanged(sequencergui::RunnerStatus status);

private:
  std::unique_ptr<SequencerObserver> m_observer;
  std::unique_ptr<UserChoiceProvider> m_choice_provider;
  std::unique_ptr<UserInputProvider> m_input_provider;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_PROCEDURE_REPORTER_H_
