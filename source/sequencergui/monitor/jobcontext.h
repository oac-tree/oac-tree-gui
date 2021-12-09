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
#include "sequencergui/monitor/usercontext.h"

#include <QObject>
#include <memory>
#include <functional>

namespace sequi
{
class ProcedureItem;
class ProcedureRunner;
class JobLog;
class SequencerModel;
class MessagePanel;
class InstructionItem;
class GUIObjectBuilder;

//! Contains all information necessary for job to run: runner, log, domain builder.
class JobContext : public QObject
{
  Q_OBJECT
public:
  explicit JobContext(ProcedureItem* procedure_item, QObject* parent = nullptr);
  ~JobContext() override;

  // Methods to control procedure execution.

  void onPrepareJobRequest();

  void onStartRequest();

  void onPauseRequest();

  void onMakeStepRequest();

  void onStopRequest();

  bool WaitForCompletion(double timeout_sec);

  bool IsRunning() const;

  // Methods to setup procedure execution.

  void SetMessagePanel(MessagePanel* panel);

  void SetWaitingMode(WaitingMode waiting_mode);

  void SetSleepTime(int time_msec);

  void SetUserContext(const UserContext& user_context);

  // Access to internals

  ProcedureItem* GetExpandedProcedure() const;
  SequencerModel* GetExpandedModel();

signals:
  void InstructionStatusChanged(sequi::InstructionItem* instruction);

private:
  void onInstructionStatusChange(const instruction_t* instruction);
  void onLogMessage(const QString& message, int message_type);
  void onVariableChange(const QString& variable_name, const QString& value);

  void onInputRequest(const QString& current_value, const QString& description);

  void SetupConnections();

  std::unique_ptr<GUIObjectBuilder> m_guiobject_builder;

  //!< domain procedure should live longer than the runner
  std::unique_ptr<procedure_t> m_domain_procedure;

  std::unique_ptr<ProcedureRunner> m_procedure_runner;
  JobLog* m_job_log{nullptr};

  ProcedureItem* m_procedure_item{nullptr};
  ProcedureItem* m_expanded_procedure_item{nullptr};

  std::unique_ptr<SequencerModel> m_job_model;
  UserContext m_user_context;
};

}  // namespace sequi

#endif  // MVVM_MONITOR_JOBCONTEXT_H
