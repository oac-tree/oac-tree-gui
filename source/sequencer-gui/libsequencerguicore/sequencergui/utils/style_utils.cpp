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

#include "sequencergui/utils/style_utils.h"

#include <mvvm/widgets/widget_utils.h>

#include <QIcon>
#include <QSize>
#include <cmath>
namespace
{
//! Flag if to use svg versions of icons.
const bool kSvgIcons = false;
}  // namespace

namespace sequencergui::styleutils
{

int UnitSize(double scale)
{
  return static_cast<int>(std::round(mvvm::utils::WidthOfLetterM() * scale));
}

int AppFontSize()
{
  return mvvm::utils::SystemPointSize();
}

QSize ToolBarIconSize()
{
  const int width = UnitSize(2.3);
  return {width, width};
}

QSize NarrowToolBarIconSize()
{
  const int width = std::round(UnitSize(1.75));
  return {width, width};
}

QIcon GetIcon(const std::string &icon_name)
{
  auto name = QString(":/icons/%1").arg(QString::fromStdString(icon_name));
  if (!kSvgIcons)
  {
    name.replace(".svg", ".png");
  }
  return QIcon(name);
}

}  // namespace sequencergui::styleutils
