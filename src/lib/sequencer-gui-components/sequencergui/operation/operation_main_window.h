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

#ifndef SEQUENCERGUI_OPERATION_OPERATION_MAIN_WINDOW_H_
#define SEQUENCERGUI_OPERATION_OPERATION_MAIN_WINDOW_H_

#include <QMainWindow>
#include <memory>

class QCloseEvent;

namespace mvvm
{
class ApplicationModel;
}

namespace sequencergui
{

class MonitorWidget;
class MonitorModel;
class MonitorMainWindowActions;

//! The main window of sequencer-operation applcation

class OperationMainWindow : public QMainWindow
{
  Q_OBJECT

public:
  OperationMainWindow();
  ~OperationMainWindow() override;

protected:
  void closeEvent(QCloseEvent* event) override;

private:
  void PopulateModel();
  void InitApplication();
  void ReadSettings();
  void WriteSettings();

  std::unique_ptr<mvvm::ApplicationModel> m_model;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_OPERATION_OPERATION_MAIN_WINDOW_H_
