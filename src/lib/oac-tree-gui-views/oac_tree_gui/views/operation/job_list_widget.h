/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
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

#ifndef OAC_TREE_GUI_VIEWS_OPERATION_JOB_LIST_WIDGET_H_
#define OAC_TREE_GUI_VIEWS_OPERATION_JOB_LIST_WIDGET_H_

#include <QWidget>
#include <memory>

class QTreeView;

namespace mvvm
{
class ViewModel;
class ItemViewComponentProvider;
}  // namespace mvvm

namespace sup::gui
{
class CustomHeaderView;
}

namespace oac_tree_gui
{

class JobModel;
class JobItem;

/**
 * @brief The JobListWidget class A list-like view of currently running/idle jobs. Occupies a top
 * left corner of OperationJobPanel.
 */
class JobListWidget : public QWidget
{
  Q_OBJECT

public:
  explicit JobListWidget(QWidget* parent_widget = nullptr);
  ~JobListWidget() override;

  void SetJobModel(JobModel* model);

  JobItem* GetSelectedJob();

  void SetSelectedJob(JobItem* job);

  QTreeView* GetTreeView();

  mvvm::ViewModel* GetViewModel();

signals:
  void JobSelected(oac_tree_gui::JobItem* job_item);

private:
  QTreeView* m_tree_view{nullptr};
  std::unique_ptr<mvvm::ItemViewComponentProvider> m_component_provider;
  sup::gui::CustomHeaderView* m_custom_header{nullptr};

  JobModel* m_model{nullptr};
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_OPERATION_JOB_LIST_WIDGET_H_
