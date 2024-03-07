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

#ifndef SEQUENCERGUI_JOBSYSTEM_PROCEDURE_REPORTER_H_
#define SEQUENCERGUI_JOBSYSTEM_PROCEDURE_REPORTER_H_

#include <sequencergui/domain/sequencer_types_fwd.h>
#include <sequencergui/jobsystem/runner_status.h>
#include <sequencergui/jobsystem/user_context.h>

#include <QObject>
#include <memory>

namespace sequencergui
{

class InstructionItem;
class LogEvent;
class SequencerObserver;
class SignalQueue;
struct UserChoiceResult;

/**
 * @brief The SignalQueue helper class is intended to re-send ProcedureReporter signals in queued
 * manner.
 */

class SignalQueue : public QObject
{
  Q_OBJECT

public:
  explicit SignalQueue(QObject* parent) : QObject(parent) {}

signals:
  void DomainInstructionStatusChanged(const instruction_t* instruction, const QString&);
  void NextLeavesChanged(const std::vector<sequencergui::InstructionItem*>&);
  void RunnerStatusChanged(sequencergui::RunnerStatus status);
};

/**
 * @brief The ProcedureReporter class reports event happening in sequencer domain to the GUI and
 * handles the sequencer user input requests.
 *
 * @details It transforms direct calls from the sequencer to queued signals to the GUI thread.
 */

class ProcedureReporter : public QObject
{
  Q_OBJECT

public:
  using get_instruction_item_cb_t = std::function<InstructionItem*(const instruction_t&)>;

  explicit ProcedureReporter(get_instruction_item_cb_t callback, QObject* parent = nullptr);
  ~ProcedureReporter() override;

  void SetUserContext(const UserContext& user_context);

  void OnLogEvent(const LogEvent& event);

  SequencerObserver* GetObserver();

  /**
   * @brief Processes the change of status of the domain instruction.
   *
   * @param instruction The domain instruction.
   * @param status The status of the domain runner adapter.
   *
   * @details Should be called from the sequencer thread.
   */
  void OnDomainInstructionStatusChange(const instruction_t* instruction, const std::string& value);

  /**
   * @brief Processes the change of status of the domain runner.
   *
   * @param status The status of the domain runner adapter.
   *
   * @details Should be called from the sequencer thread.
   */
  void OnDomainRunnerStatusChanged(RunnerStatus status);

  /**
   * @brief Process the tick of the domain runner and reports next instruction leaves.
   *
   * @details Should be called from the sequencer thread.
   */
  void OnDomainProcedureTick(const procedure_t& procedure);

signals:
  void LogEventReceived(const sequencergui::LogEvent& event);

  /**
   * @brief The signal reports InstructionItem which status has changed.
   *
   * @details Signal is triggered via sequencer observer. The signal will be emitted in
   * already queued manner thanks to SignalQueue. It is safe to connect to it from the GUI thread
   * using a direct connection.
   */
  void InstructionStatusChanged(sequencergui::InstructionItem*);

  /**
   * @brief The signal reports InstructionItem next leaves.
   *
   * @details Signal is triggered via the runner's tick callback. The signal will be emitted in
   * already queued manner thanks to SignalQueue. It is safe to connect to it from the GUI thread
   * using a direct connection.
   */
  void NextLeavesChanged(const std::vector<sequencergui::InstructionItem*>&);

  /**
   * @brief The signal will be emited when domain runner adapter has chainged its status.
   *
   * @param status The status of the domain runner adapter.
   *
   * @details The signal will be emitted in already queued manner thanks to SignalQueue. It is safe
   * to connect to it from the GUI thread using a direct connection.
   */
  void RunnerStatusChanged(sequencergui::RunnerStatus status);

private:
  /**
   * @brief Processes domain status change.
   */
  void ProcessDomainInstructionStatusChange(const instruction_t* instruction, const QString& value);

  std::unique_ptr<SequencerObserver> m_observer;
  SignalQueue* m_signal_queue{nullptr};

  //! callback to retrieve instruction item corresponding to given domain instruction
  get_instruction_item_cb_t m_get_instruction_item;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_PROCEDURE_REPORTER_H_
