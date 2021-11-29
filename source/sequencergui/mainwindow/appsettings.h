/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#ifndef SEQUENCERGUI_MAINWINDOW_APPSETTINGS_H
#define SEQUENCERGUI_MAINWINDOW_APPSETTINGS_H

namespace sequi
{

enum class BehaviorTreeStyle
{
  kTopToBottom,
  kLeftToRight
};

class AppSettings
{
public:
  AppSettings();

  BehaviorTreeStyle GetTreeStyle() const;

  bool IsTopToBottomStyle() const;

private:
  BehaviorTreeStyle m_tree_style;
};
}  // namespace sequi

#endif  // SEQUENCERGUI_MAINWINDOW_COLLAPSIBLEWIDGET_H
