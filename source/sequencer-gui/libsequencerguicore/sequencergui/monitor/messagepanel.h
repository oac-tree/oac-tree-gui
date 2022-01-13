/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Creation, execution and monitoring of the Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_MONITOR_MESSAGEPANEL_H
#define MVVM_MONITOR_MESSAGEPANEL_H

#include "sequencergui/mainwindow/collapsiblewidget.h"

class QTextEdit;

namespace sequencergui
{

class MessagePanel : public CollapsibleWidget
{
public:
  explicit MessagePanel(QWidget* parent = nullptr);

public slots:
  void onClearLog();
  void onMessage(const QString& message, const QColor& color = QColor(Qt::black));

private:
  QTextEdit* m_text_edit{nullptr};
};

}  // namespace sequi

#endif  // MVVM_MONITOR_MESSAGEPANEL_H
