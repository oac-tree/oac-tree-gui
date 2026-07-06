/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef OAC_TREE_GUI_VIEWS_OPERATION_OPERATION_SPLITTABLE_WIDGET_H_
#define OAC_TREE_GUI_VIEWS_OPERATION_OPERATION_SPLITTABLE_WIDGET_H_

#include <QWidget>
#include <memory>
#include <vector>

class QSplitter;

namespace mvvm
{
class ItemSelection;
}

namespace sup::gui
{
class SplittableEditorController;
class SplittableComboPanel;
}  // namespace sup::gui

namespace oac_tree_gui
{

class InstructionItem;
class JobLog;
class JobModel;
class JobItem;
class OperationTabWidget;

/**
 * @brief The OperationSplittableWidget holds multiple splittable OperationTabWidgets.
 */
class OperationSplittableWidget : public QWidget
{
  Q_OBJECT

public:
  explicit OperationSplittableWidget(QWidget* parent_widget = nullptr);
  ~OperationSplittableWidget() override;

  OperationSplittableWidget(const OperationSplittableWidget&) = delete;
  OperationSplittableWidget& operator=(const OperationSplittableWidget&) = delete;
  OperationSplittableWidget(OperationSplittableWidget&&) = delete;
  OperationSplittableWidget& operator=(OperationSplittableWidget&&) = delete;

  void SetModel(JobModel* model);

  /**
   * @brief Sets the job to be shown in the currently focused panel.
   */
  void SetCurrentJob(JobItem* job_item);

  /**
   * @brief Sets the log to be shown in every panel showing the given job.
   */
  void SetJobLog(JobItem* job, JobLog* job_log);

  /**
   * @brief Sets currently selected instructions in every panel showing the given job.
   */
  void SetSelectedInstructions(JobItem* job, const std::vector<const InstructionItem*>& items);

  /**
   * @brief Returns tick timeout currently set in the focused panel.
   */
  int GetCurrentTickTimeout();

  /**
   * @brief Adds panel to the splitter.
   */
  sup::gui::SplittableComboPanel* CreatePanel();

  /**
   * @brief Returns controller of the editor, which allows to manipulate panels and their content.
   *
   * Used for testing.
   */
  sup::gui::SplittableEditorController* GetController() const;

  /**
   * @brief Returns the set of jobs currently shown across all panels.
   */
  std::vector<JobItem*> GetActiveJobs() const;

signals:
  /**
   * @brief Notifies that the job selection in the focus widget has changed.
   */
  void focusWidgetJobSelectionChanged(const mvvm::ItemSelection& selection);

  /**
   * @brief Notifies that the set of jobs shown across the panels has changed.
   */
  void activeJobsChanged(const std::vector<oac_tree_gui::JobItem*>& active_jobs);

  void RunRequest(oac_tree_gui::JobItem* job);
  void PauseRequest(oac_tree_gui::JobItem* job);
  void StepRequest(oac_tree_gui::JobItem* job);
  void StopRequest(oac_tree_gui::JobItem* job);
  void ResetRequest(oac_tree_gui::JobItem* job);
  void ChangeDelayRequest(int msec);
  void ToggleBreakpointRequest(oac_tree_gui::JobItem* job,
                               const oac_tree_gui::InstructionItem* instruction);

private:
  std::unique_ptr<sup::gui::SplittableEditorController> CreateSplitterController() const;

  /**
   * @brief Returns the tab widget of the currently focused panel, or nullptr if none.
   */
  OperationTabWidget* GetFocusTabWidget() const;

  /**
   * @brief Collects jobs shown across all panels, optionally excluding one panel.
   *
   * The excluded panel is used when a panel is about to close and should no longer contribute.
   */
  std::vector<JobItem*> CollectActiveJobs(
      const sup::gui::SplittableComboPanel* excluded = nullptr) const;

  /**
   * @brief Wires a newly added panel and announces the updated active-jobs set.
   */
  void OnPanelAdded(sup::gui::SplittableComboPanel* panel);

  QSplitter* m_splitter{nullptr};
  JobModel* m_model{nullptr};
  std::unique_ptr<sup::gui::SplittableEditorController> m_splitter_controller;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_OPERATION_OPERATION_SPLITTABLE_WIDGET_H_
