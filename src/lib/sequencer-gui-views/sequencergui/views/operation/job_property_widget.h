/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_VIEWS_OPERATION_JOB_PROPERTY_WIDGET_H_
#define SEQUENCERGUI_VIEWS_OPERATION_JOB_PROPERTY_WIDGET_H_

#include <QWidget>

namespace mvvm
{
class PropertyTreeView;
}

namespace sequencergui
{
class JobItem;

//! Shows properties of currently selected JobItem, populates lower left corner of
//! SequencerMonitorView.

class JobPropertyWidget : public QWidget
{
  Q_OBJECT

public:
  explicit JobPropertyWidget(QWidget* parent_widget = nullptr);

  void SetJob(JobItem* item);

private:
  mvvm::PropertyTreeView* m_property_tree_view{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_VIEWS_OPERATION_JOB_PROPERTY_WIDGET_H_
