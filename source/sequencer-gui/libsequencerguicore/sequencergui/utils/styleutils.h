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

#ifndef SEQUENCERGUI_UTILS_STYLEUTILS_H
#define SEQUENCERGUI_UTILS_STYLEUTILS_H

#include <QWidget>

class QToolBar;
class QIcon;

namespace sequencergui::StyleUtils
{

//! Size of tolbar icons.
QSize ToolBarIconSize();

//! Size of tolbar icons.
QSize NarrowToolBarIconSize();

QIcon GetIcon(const std::string& icon_name);
}  // namespace sequencergui::StyleUtils

#endif  // SEQUENCERGUI_UTILS_STYLEUTILS_H
