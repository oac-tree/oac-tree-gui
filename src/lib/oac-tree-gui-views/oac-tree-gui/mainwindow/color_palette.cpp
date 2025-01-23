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

#include "color_palette.h"

#include <sup/gui/style/style_helper.h>

namespace sequencergui
{

namespace
{

// const char *kCetaceanBlue = "#070e2b";
// const char *kDarkPurple = "#32002f";
const char *kMediumElectricBlue = "#005291";
// const char *kArsenic = "#35374a";
const char *kChineseBlack = "#171717";

}  // namespace

QColor GetMainToolBarColor()
{
  return sup::gui::utils::IsDarkTheme() ? QColor(kChineseBlack) : QColor(kMediumElectricBlue);
}

}  // namespace sequencergui
