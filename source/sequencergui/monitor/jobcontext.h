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

#ifndef MVVM_MONITOR_JOBCONTEXT_H
#define MVVM_MONITOR_JOBCONTEXT_H

#include "sequencergui/model/sequencer_types.h"
#include "sequencergui/monitor/job_types.h"

#include <QObject>
#include <memory>

namespace sequi
{
class ProcedureItem;
class ProcedureRunner;
class JobLog;
class DomainObjectBuilder;
class SequencerModel;
class MessagePanel;
class InstructionItem;

//! Contains all information necessary for job to run: runner, log, domain builder.
class JobContext : public QObject
{
  Q_OBJECT
public:
  explicit JobContext(SequencerModel* model, ProcedureItem* procedure_item,
                      QObject* parent = nullptr);
  ~JobContext() override;

  void onStartRequest();

  void onPauseRequest();

  void onMakeStepRequest();

  void onStopRequest();

  bool IsRunning() const;

  void SetMessagePanel(MessagePanel* panel);

  bool WaitForCompletion(double timeout_sec);

  void onInstructionStatusChange(const instruction_t* instruction);

  void onLogMessage(const QString& message, int message_type);

  void onVariableChange(const QString& variable_name, const QString& value);

  void SetWaitingMode(WaitingMode waiting_mode);

  void SetSleepTime(int time_msec);

signals:
  void InstructionStatusChanged(sequi::InstructionItem* instruction);

private:
  void SetupConnections();

  std::unique_ptr<DomainObjectBuilder> m_domain_builder; // builder should live longer than the runner
  std::unique_ptr<ProcedureRunner> m_procedure_runner;
  JobLog* m_job_log{nullptr};

  ProcedureItem* m_procedure_item{nullptr};
  SequencerModel* m_model{nullptr};
};

}  // namespace sequi

#endif  // MVVM_MONITOR_JOBCONTEXT_H
