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

#ifndef SEQUENCERGUI_OPERATION_OPERATION_JOB_PANEL_TOOLBAR_H_
#define SEQUENCERGUI_OPERATION_OPERATION_JOB_PANEL_TOOLBAR_H_

#include <QToolBar>
#include <functional>
#include <vector>

class QToolButton;

namespace sequencergui
{

class ProcedureItem;

class OperationJobPanelToolBar : public QToolBar
{
  Q_OBJECT

public:
  using callback_t = std::function<std::vector<ProcedureItem*>()>;

  explicit OperationJobPanelToolBar(QWidget* parent = nullptr);

  void SetAvailableProcedures(callback_t available_procedures);

signals:
  void SubmitProcedureRequest(sequencergui::ProcedureItem* item);
  void ImportJobRequest();
  void RegenerateJobRequest();
  void RemoveJobRequest();

private:
  std::unique_ptr<QMenu> CreateSubmitProcedureMenu();
  void OnAboutToShowMenu();

  std::unique_ptr<QMenu> m_submit_procedure_menu;

  QToolButton* m_import_button{nullptr};
  QToolButton* m_submit_button{nullptr};
  QToolButton* m_regenerate_button{nullptr};
  QToolButton* m_remove_button{nullptr};

  callback_t m_available_procedures;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_OPERATION_OPERATION_JOB_PANEL_TOOLBAR_H_
