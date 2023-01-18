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

#ifndef SEQUENCERGUI_MONITOR_MESSAGE_PANEL_H_
#define SEQUENCERGUI_MONITOR_MESSAGE_PANEL_H_

#include <sequencergui/jobsystem/log_event.h>

#include <QWidget>

class QTextEdit;
class QAction;
class QTreeView;

namespace sequencergui
{

class MessagePanel : public QWidget
{
public:
  explicit MessagePanel(QWidget* parent = nullptr);

  void OnClearLog();

  void OnMessage(const LogEvent& log_event);

private:
  QTextEdit* m_text_edit{nullptr};
  QAction* m_remove_selected_action{nullptr};
  QTreeView* m_tree_view{nullptr};
};
}  // namespace sequencergui

#endif  // SEQUENCERGUI_MONITOR_MESSAGE_PANEL_H_
