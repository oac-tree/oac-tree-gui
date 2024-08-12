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

#ifndef SEQUENCERGUI_VIEWS_REMOTE_REMOTE_MONITOR_VIEW_H_
#define SEQUENCERGUI_VIEWS_REMOTE_REMOTE_MONITOR_VIEW_H_

#include <QWidget>

class QLineEdit;

namespace sequencergui
{

class RemoteMonitorView : public QWidget
{
  Q_OBJECT

public:
  RemoteMonitorView(QWidget *parent = nullptr);

private:
  QLineEdit* m_line_edit{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_VIEWS_REMOTE_REMOTE_MONITOR_VIEW_H_
