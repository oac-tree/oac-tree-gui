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

#ifndef SEQUENCERGUI_MONITOR_JOBPROPERTYWIDGET_H
#define SEQUENCERGUI_MONITOR_JOBPROPERTYWIDGET_H

#include <QWidget>

class QListWidget;

namespace sequencergui
{

//! Shows properties of currently selected JobItem, populates lower left corner of
//! SequencerMonitorView.

class JobPropertyWidget : public QWidget
{
  Q_OBJECT

public:
  explicit JobPropertyWidget(QWidget* parent = nullptr);

private:
  QListWidget* m_list_widget{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MONITOR_JOBPROPERTYWIDGET_H
