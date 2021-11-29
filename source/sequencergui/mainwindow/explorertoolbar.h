/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#ifndef SEQUENCERGUI_MAINWINDOW_EXPLORERTOOLBAR_H
#define SEQUENCERGUI_MAINWINDOW_EXPLORERTOOLBAR_H

#include <QToolBar>

class QToolBar;
class QLabel;
class QAction;

namespace sequi
{
//! Instrument tool bar with menu selector.

class ExplorerToolBar : public QToolBar
{
  Q_OBJECT

public:
  ExplorerToolBar(QWidget* parent = nullptr);

  void setText(const QString& text);

  void AddDotsMenu();

  void InsertElement(QWidget* widget);

private:
  void insertStrech();

  QLabel* m_label{nullptr};
  QAction* m_spacer_action{nullptr};
};

}  // namespace sequi

#endif  // SEQUENCERGUI_MAINWINDOW_EXPLORERTOOLBAR_H
