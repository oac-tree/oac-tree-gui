/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#include "sequencergui/mainwindow/appsettings.h"

namespace sequi
{

AppSettings::AppSettings() : m_tree_style(BehaviorTreeStyle::kTopToBottom)
{

}

BehaviorTreeStyle AppSettings::GetTreeStyle() const
{
  return m_tree_style;
}

bool AppSettings::IsTopToBottomStyle() const
{
  return m_tree_style == BehaviorTreeStyle::kTopToBottom;
}

}  // namespace sequi
