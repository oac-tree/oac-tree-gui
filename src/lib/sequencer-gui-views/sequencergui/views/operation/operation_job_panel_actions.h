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

#ifndef SEQUENCERGUI_OPERATION_OPERATION_JOB_PANEL_ACTIONS_H_
#define SEQUENCERGUI_OPERATION_OPERATION_JOB_PANEL_ACTIONS_H_

#include <QObject>
#include <functional>
#include <memory>

class QMenu;
class QAction;

namespace sup::gui
{
class ActionMenu;
}  // namespace sup::gui

namespace sequencergui
{

class ProcedureItem;

/**
 * @brief The OperationJobPanelToolActions class contains set of actions for OperationJobPanel.
 */

class OperationJobPanelActions : public QObject
{
  Q_OBJECT

public:
  using callback_t = std::function<std::vector<ProcedureItem*>()>;

  explicit OperationJobPanelActions(QObject* parent = nullptr);
  ~OperationJobPanelActions() override;

  void SetAvailableProcedures(callback_t available_procedures);

  QList<QAction*> GetSequencerMonitorViewActions();

  QList<QAction*> GetOperationMonitorViewActions();

signals:
  void SubmitProcedureRequest(sequencergui::ProcedureItem* item);
  void ImportJobRequest();
  void RegenerateJobRequest();
  void RemoveJobRequest();
  void RemoveAndCleanupJobRequest();

private:
  std::unique_ptr<QMenu> CreateSubmitProcedureMenu();
  void OnAboutToShowMenu();

  std::unique_ptr<QMenu> m_submit_procedure_menu;

  QAction* m_import_action{nullptr};
  sup::gui::ActionMenu* m_submit_action{nullptr};
  QAction* m_regenerate_action{nullptr};
  QAction* m_remove_action{nullptr};
  QAction* m_remove_and_cleanup_action{nullptr};

  callback_t m_available_procedures;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_OPERATION_OPERATION_JOB_PANEL_ACTIONS_H_
