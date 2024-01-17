/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
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

#include <sup/gui/app/application_helper.h>

#include <QSettings>

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
  static const QColor result("#00aa00");  // green
  return result;
}

QColor GetDisonnectedColor()
{
  static const QColor result("#b9b9b9");  // gray
  return result;
}

QString GetCustomToolTipStyle()
{
  static const QString style(
      "QToolTip {border: 2px solid darkgrey; padding: 5px; border-radius: 3px;}");

  // Adwaita style has own white-on-black semi-transparent tooltip style.
  // We have to define dark background to match this style.
  static const QString adwaita_style(
      "QToolTip {border: 2px solid lightgray; padding: 5px; border-radius: 3px; background-color: "
      "darkgray}");

  return sup::gui::IsOnCodac() ? adwaita_style : style;
}

}  // namespace sequencergui
