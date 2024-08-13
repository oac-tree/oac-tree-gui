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

#ifndef SEQUENCERGUI_VIEWS_OPERATION_JOB_LIST_WIDGET_H_
#define SEQUENCERGUI_VIEWS_OPERATION_JOB_LIST_WIDGET_H_

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

namespace sequencergui
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
  explicit JobListWidget(QWidget* parent = nullptr);
  ~JobListWidget() override;

  void SetJobModel(JobModel* model);

  JobItem* GetSelectedJob();

  void SetSelectedJob(JobItem* job);

  QTreeView* GetTreeView();

  mvvm::ViewModel* GetViewModel();

signals:
  void JobSelected(sequencergui::JobItem* job_item);

private:
  QTreeView* m_tree_view{nullptr};
  std::unique_ptr<mvvm::ItemViewComponentProvider> m_component_provider;
  sup::gui::CustomHeaderView* m_custom_header{nullptr};

  JobModel* m_model{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_VIEWS_OPERATION_JOB_LIST_WIDGET_H_
