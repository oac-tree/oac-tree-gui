/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#ifndef SEQUENCERGUI_MONITOR_JOBMANAGER_H
#define SEQUENCERGUI_MONITOR_JOBMANAGER_H

#include "sequencergui/model/sequencer_types.h"

#include <QObject>
#include <memory>

namespace sequi
{
class SequencerModel;
class InstructionItem;
class ProcedureItem;
class MessagePanel;
class JobContext;

//! Handles simultaneous execution of multiple procedures.

class JobManager : public QObject
{
  Q_OBJECT

public:
  JobManager(QObject* parent = nullptr);
  ~JobManager() override;

  void setModel(SequencerModel* model);

  void setCurrentProcedure(ProcedureItem* procedure);

  void onStartProcedureRequest(ProcedureItem* procedure);
  void onPauseProcedureRequest();
  void onStopProcedureRequest();
  void onMakeStepRequest();

  void SetMessagePanel(MessagePanel* panel);
  void onChangeDelayRequest(int msec);

signals:
  void InstructionStatusChanged(InstructionItem* instruction);

private:
  void SetupConnections();

  JobContext* GetCurrentContext();
  JobContext* CreateContext();

  ProcedureItem* m_current_procedure{nullptr};
  SequencerModel* m_model{nullptr};
  std::map<ProcedureItem*, JobContext*> m_context_map;
  MessagePanel* m_message_panel{nullptr};
  int m_current_delay{0};
};

}  // namespace sequi

#endif  // SEQUENCERGUI_MONITOR_JOBMANAGER_H
