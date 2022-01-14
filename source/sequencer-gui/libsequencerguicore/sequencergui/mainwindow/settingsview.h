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

#ifndef SEQUENCERGUI_MAINWINDOW_SETTINGSVIEW_H
#define SEQUENCERGUI_MAINWINDOW_SETTINGSVIEW_H

#include <QWidget>

class QToolBar;
class QListWidget;
class QStackedWidget;
class QTabWidget;

namespace sequencergui
{
class SequencerModel;

//! Simple file browser with bread crumb on top, and tree view at the bottom.
class SettingsView : public QWidget
{
  Q_OBJECT

public:
  SettingsView(QWidget* parent = nullptr);

  void SetModel(SequencerModel* model);

private:
  void SetupListSelector();
  void SetupModelSettings();
  void SetupOtherSettings();

  QToolBar* m_tool_bar{nullptr};
  QListWidget* m_list_widget;                 //!< selector for specific settings window on the left
  QStackedWidget* m_stacked_widget{nullptr};  //!< stack with settings widgets
  QTabWidget* m_tab_widget{nullptr};          //!< application model settings

  SequencerModel* m_model;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MAINWINDOW_SETTINGSVIEW_H
