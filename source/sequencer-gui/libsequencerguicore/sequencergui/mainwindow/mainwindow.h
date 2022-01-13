/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
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

#ifndef SEQUENCERGUI_MAINWINDOW_MAINWINDOW_H
#define SEQUENCERGUI_MAINWINDOW_MAINWINDOW_H

#include <QMainWindow>
#include <memory>

class QCloseEvent;

namespace ModelView
{
class MainVerticalBarWidget;
}

namespace sequencergui
{

class SequencerXMLView;
class SequencerTreeView;
class SequencerModel;
class SequencerMonitorView;
class SequencerComposerView;
class SettingsView;

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
  void InitComponents();
  void WriteSettings();
  void InitMenu();
  void PopulateModel();

  std::unique_ptr<SequencerModel> m_model;

  SequencerXMLView* m_xml_view{nullptr};
  SequencerMonitorView* m_monitor_view{nullptr};
  SequencerComposerView* m_composer_view{nullptr};
  SettingsView* m_settings_view{nullptr};
  ModelView::MainVerticalBarWidget* m_tab_widget{nullptr};
};

}  // namespace sequi

#endif  // SEQUENCERGUI_MAINWINDOW_MAINWINDOW_H
