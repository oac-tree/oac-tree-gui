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

#ifndef SEQUENCERGUI_JOBSYSTEM_JOB_CONTEXT_H_
#define SEQUENCERGUI_JOBSYSTEM_JOB_CONTEXT_H_

#include <sequencergui/domain/sequencer_types.h>
#include <sequencergui/jobsystem/job_types.h>
#include <sequencergui/jobsystem/user_context.h>

#include <QObject>
#include <memory>

namespace sequencergui
{
class ProcedureItem;
class ProcedureRunner;
class JobLog;
class JobItem;
class MessagePanel;
class InstructionItem;
class GUIObjectBuilder;

//! Contains all information necessary for job to run: runner, log, domain builder.
class JobContext : public QObject
{
  Q_OBJECT
public:
  explicit JobContext(JobItem* job_item);
  ~JobContext() override;

  // Methods to control procedure execution.

  void onPrepareJobRequest();

  void onStartRequest();

  void onPauseRequest();

  void onMakeStepRequest();

  void onStopRequest();

  bool IsRunning() const;

  // Methods to setup procedure execution.

  void SetMessagePanel(MessagePanel* panel);

  void SetSleepTime(int time_msec);

  void SetUserContext(const UserContext& user_context);

  // Access to internals

  ProcedureItem* GetExpandedProcedure() const;

  bool IsValid() const;  // FIXME find better name

  RunnerStatus GetRunnerStatus() const;

signals:
  void InstructionStatusChanged(sequencergui::InstructionItem* instruction);

private:
  void onInstructionStatusChange(const instruction_t* instruction, const QString& status);
  void onLogMessage(const QString& message, int message_type);
  void onVariableChange(const QString& variable_name, const QString& value);
  void onRunnerStatusChanged();

  void CheckRunner() const;

  std::unique_ptr<ProcedureRunner> CreateProcedureRunner(procedure_t* procedure);

  std::unique_ptr<GUIObjectBuilder> m_guiobject_builder;

  //!< domain procedure should live longer than the runner
  std::unique_ptr<procedure_t> m_domain_procedure;

  std::unique_ptr<ProcedureRunner> m_procedure_runner;
  JobLog* m_job_log{nullptr};

  JobItem* m_job_item{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_JOB_CONTEXT_H_
