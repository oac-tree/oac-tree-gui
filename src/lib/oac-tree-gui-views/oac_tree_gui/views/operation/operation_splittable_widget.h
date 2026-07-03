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

class JobModel;
class JobItem;

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
   * @brief Adds panel to the splitter.
   */
  sup::gui::SplittableComboPanel* CreatePanel();

  /**
   * @brief Returns controller of the editor, which allows to manipulate panels and their content.
   *
   * Used for testing.
   */
  sup::gui::SplittableEditorController* GetController() const;

signals:
  /**
   * @brief Notifies that the job selection in the focus widget has changed.
   */
  void focusWidgetJobSelectionChanged(const mvvm::ItemSelection& selection);

private:
  std::unique_ptr<sup::gui::SplittableEditorController> CreateSplitterController() const;

  QSplitter* m_splitter{nullptr};
  JobModel* m_model{nullptr};
  std::unique_ptr<sup::gui::SplittableEditorController> m_splitter_controller;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_OPERATION_OPERATION_SPLITTABLE_WIDGET_H_
