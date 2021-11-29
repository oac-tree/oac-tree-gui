/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#ifndef SEQUENCERGUI_MAINWINDOW_COLLAPSIBLEWIDGET_H
#define SEQUENCERGUI_MAINWINDOW_COLLAPSIBLEWIDGET_H

#include <QWidget>

class QSplitter;

namespace sequi
{

class ExplorerNarrowToolBar;

class CollapsibleWidget : public QWidget
{
  Q_OBJECT

public:
  CollapsibleWidget(QWidget* parent = nullptr);
  ~CollapsibleWidget();

  void AddToSplitter(QSplitter* splitter);

  ExplorerNarrowToolBar* GetToolBar();

  void SetText(const QString& text);

private:
  ExplorerNarrowToolBar* m_tool_bar{nullptr}; // ToolBar intended to go to splitter separately
};

}  // namespace sequi

#endif  // SEQUENCERGUI_MAINWINDOW_COLLAPSIBLEWIDGET_H
