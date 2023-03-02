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

#ifndef ANYVALUE_EDITOR_ANYVALUE_EDITOR_MAIN_WINDOW_H_
#define ANYVALUE_EDITOR_ANYVALUE_EDITOR_MAIN_WINDOW_H_

#include <QMainWindow>
#include <memory>

class QCloseEvent;

namespace anyvalueeditor
{

class AnyValueEditor;

//! The main window of this application.

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow();
  ~MainWindow() override;

protected:
  void closeEvent(QCloseEvent* event) override;

private:
  void InitApplication();
  void InitMenu();
  void InitComponents();
  void WriteSettings();

  AnyValueEditor* m_anyvalue_editor{nullptr};
};

}  // namespace anyvalueeditor

#endif  // ANYVALUE_EDITOR_ANYVALUE_EDITOR_MAIN_WINDOW_H_
