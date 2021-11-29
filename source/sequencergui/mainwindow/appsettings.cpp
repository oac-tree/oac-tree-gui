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
