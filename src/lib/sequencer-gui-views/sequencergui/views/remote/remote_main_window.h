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

#ifndef SEQUENCERGUI_VIEWS_REMOTE_REMOTE_MAIN_WINDOW_H_
#define SEQUENCERGUI_VIEWS_REMOTE_REMOTE_MAIN_WINDOW_H_

#include <sup/gui/app/main_window_types.h>

#include <QMainWindow>

class QCloseEvent;

namespace sequencergui
{

/**
 * @brief The RemoteMainWindow class
 */
class RemoteMainWindow : public QMainWindow
{
  Q_OBJECT

public:
  RemoteMainWindow();
  ~RemoteMainWindow() override;

  bool ImportProcedure(const QString& file_name) { return false; }

protected:
  void closeEvent(QCloseEvent* event) override;

private:
  void InitApplication();
  void ReadSettings();
  void WriteSettings();
  bool CanCloseApplication();
  void OnRestartRequest(sup::gui::AppExitCode exit_code);
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_VIEWS_REMOTE_REMOTE_MAIN_WINDOW_H_
