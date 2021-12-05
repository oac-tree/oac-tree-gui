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

#ifndef SEQUENCERGUI_MONITOR_PROCEDURERUNNER_H
#define SEQUENCERGUI_MONITOR_PROCEDURERUNNER_H

#include "sequencergui/model/sequencer_types.h"
#include "sequencergui/monitor/flowcontroller.h"
#include "sequencergui/monitor/job_types.h"

#include <QObject>
#include <memory>
#include <mutex>
#include <thread>

namespace sequi
{
class SequencerObserver;

//! Executes sequencer's procedure in a separate thread.
//! Observes sequence execution and converts sequencer UI calls into signals.
//! These signals must be connected with the rest of the GUI via queed connections.

class ProcedureRunner : public QObject
{
  Q_OBJECT

public:
  ProcedureRunner(QObject* parent = nullptr);
  ~ProcedureRunner() override;

  // control flow

  void ExecuteProcedure(procedure_t* procedure, bool make_setup = true);

  void onMakeStepRequest();

  void Terminate();

  void SetWaitingMode(WaitingMode waiting_mode);

  void SetSleepTime(int time_msec);

  bool IsBusy() const;

  RunnerStatus GetRunnerStatus() const;

  bool WaitForCompletion(double timeout_sec);

public slots:
  void onInstructionStatusChange(const instruction_t* instruction);

  void onLogMessage(const std::string& message, MessageType message_type = MessageType::kDefault);

  void onVariableChange(const std::string& variable_name, const std::string& value);

signals:
  void InstructionStatusChanged(const instruction_t* instruction);
  void LogMessageRequest(const QString& message, int message_type);
  void RunnerStatusChanged();
  void VariableChanged(const QString& variable_name, const QString& value);

private:
  void LaunchDomainRunner(procedure_t* procedure);
  void SetRunnerStatus(RunnerStatus value);

  std::unique_ptr<SequencerObserver> m_observer;
  std::thread m_runner_thread;
  RunnerStatus m_runner_status;
  std::unique_ptr<runner_t> m_domain_runner;
  FlowController m_flow_controller;
  mutable std::mutex m_mutex;
};

}  // namespace sequi

#endif  // SEQUENCERGUI_MONITOR_PROCEDURERUNNER_H
