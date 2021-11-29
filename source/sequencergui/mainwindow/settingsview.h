/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#ifndef SEQUENCERGUI_MAINWINDOW_SETTINGSVIEW_H
#define SEQUENCERGUI_MAINWINDOW_SETTINGSVIEW_H

#include <QWidget>

class QToolBar;
class QListWidget;
class QStackedWidget;
class QTabWidget;

namespace sequi
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

}  // namespace sequi

#endif  // SEQUENCERGUI_MAINWINDOW_SETTINGSVIEW_H
