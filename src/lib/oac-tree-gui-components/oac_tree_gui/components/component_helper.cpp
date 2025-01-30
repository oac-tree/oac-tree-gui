/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "component_helper.h"

#include <sup/gui/app/app_helper.h>

#include <QSettings>

namespace oac_tree_gui
{

BehaviorTreeStyle GetBehaviorTreeStyle()
{
  // fixing tree style
  return BehaviorTreeStyle::kTopToBottom;
}

QColor GetConnectedVariableColor()
{
  static const QColor result("#00aa00");  // green
  return result;
}

QColor GetDisonnectedVariableColor()
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

}  // namespace oac_tree_gui
