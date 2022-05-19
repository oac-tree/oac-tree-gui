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

#include "sequencergui/utils/styleutils.h"

#include "mvvm/widgets/widgetutils.h"

#include <QIcon>
#include <QSize>

namespace
{
//! Flag if to use svg versions of icons.
const bool kSvgIcons = false;
}  // namespace

namespace sequencergui::styleutils
{

int WidthOfLetterM()
{
  return mvvm::utils::SizeOfLetterM().width();
}

int HeightOfLetterM()
{
  return mvvm::utils::SizeOfLetterM().height();
}

QSize SizeOfLetterM()
{
  return mvvm::utils::SizeOfLetterM();
}

QSize ToolBarIconSize()
{
  return {24, 24};
}

QSize NarrowToolBarIconSize()
{
  return {20, 20};
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
