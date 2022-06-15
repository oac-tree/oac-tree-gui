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

#ifndef SEQUENCERGUI_MONITOR_JOBMANAGER_H
#define SEQUENCERGUI_MONITOR_JOBMANAGER_H

#include "sequencergui/model/sequencer_types.h"

#include <QObject>
#include <memory>

namespace sequencergui
{
class JobModel;
class InstructionItem;
class JobItem;
class MessagePanel;
class JobContext;
class MessageHandlerInterface;

//! Handles simultaneous execution of multiple procedures.

class JobManager : public QObject
{
  Q_OBJECT

public:
  explicit JobManager(QObject* parent = nullptr);
  ~JobManager() override;

  void SetMessageHandler(std::unique_ptr<MessageHandlerInterface> message_handler);

  void SubmitJob(JobItem* job);

  void SetCurrentJob(JobItem* job);

  JobContext* GetCurrentContext();

  JobContext* GetContext(JobItem* job);

  JobItem* GetCurrentJob();

  void onStartProcedureRequest();
  void onPauseProcedureRequest();
  void onStopProcedureRequest();
  void onMakeStepRequest();

  void OnRemoveJobRequest(JobItem* job);

  void SetMessagePanel(MessagePanel* panel);
  void onChangeDelayRequest(int msec);

  QString onUserInputRequest(const QString& current_value, const QString& description);
  int onUserChoiceRequest(const QStringList& choices, const QString& description);

signals:
  void InstructionStatusChanged(sequencergui::InstructionItem* instruction);

private:
  std::unique_ptr<JobContext> CreateContext(JobItem* item);

  JobItem* m_current_job{nullptr};
  std::map<JobItem*, std::unique_ptr<JobContext>> m_context_map;
  MessagePanel* m_message_panel{nullptr};
  int m_current_delay{0};
  std::unique_ptr<MessageHandlerInterface> m_message_handler;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MONITOR_JOBMANAGER_H
