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

#include "app_settings.h"

namespace sequencergui
{

AppSettings::AppSettings() : m_tree_style(BehaviorTreeStyle::kTopToBottom) {}

BehaviorTreeStyle AppSettings::GetTreeStyle() const
{
  return m_tree_style;
}

bool AppSettings::IsTopToBottomStyle() const
{
  return m_tree_style == BehaviorTreeStyle::kTopToBottom;
}

QColor GetConnectedColor()
{
  return QColor("#00aa00");
}

QColor GetDisonnectedColor()
{
  return QColor("#b9b9b9");
}

}  // namespace sequencergui
