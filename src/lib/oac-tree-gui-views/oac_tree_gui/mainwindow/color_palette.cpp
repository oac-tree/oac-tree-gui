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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "color_palette.h"

#include <mvvm/style/mvvm_style_helper.h>

namespace oac_tree_gui
{

namespace
{

constexpr auto kMediumElectricBlue = "#005291";
constexpr auto kChineseBlack = "#171717";

}  // namespace

QColor GetMainToolBarColor()
{
  return mvvm::style::IsDarkTheme() ? QColor::fromString(kChineseBlack)
                                    : QColor::fromString(kMediumElectricBlue);
}

}  // namespace oac_tree_gui
