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

#ifndef SEQUENCERGUI_MAINWINDOW_COLOR_PALETTE_H_
#define SEQUENCERGUI_MAINWINDOW_COLOR_PALETTE_H_

//! @file
//! Collection of colors for light and dark themes.

#include <QColor>

namespace sequencergui
{

/**
 * @brief Returns the color of the main toolbar.
 */
QColor GetMainToolBarColor();

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MAINWINDOW_COLOR_PALETTE_H_
