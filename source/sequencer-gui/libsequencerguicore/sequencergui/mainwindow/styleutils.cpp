/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Creation, execution and monitoring of the Sequencer procedures
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

#include "sequencergui/mainwindow/styleutils.h"

#include <QIcon>
#include <QSize>

namespace
{
//! Flag if to use svg versions of icons.
const bool kSvgIcons = false;
}

namespace sequencergui::StyleUtils
{

QSize ToolBarIconSize()
{
  return QSize(24, 24);
}

QSize NarrowToolBarIconSize()
{
  return QSize(20, 20);
}

QIcon GetIcon(const std::string &icon_name)
{
  auto name = QString(":/icons/%1").arg(QString::fromStdString(icon_name));
  if (!kSvgIcons)
  {
    name.replace(".svg", ".png");
  }
  auto result = QIcon(name);
  return QIcon(name);
}

}  // namespace sequi::StyleUtils
