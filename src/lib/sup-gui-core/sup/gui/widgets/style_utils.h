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

#ifndef SUP_GUI_WIDGETS_STYLE_UTILS_H_
#define SUP_GUI_WIDGETS_STYLE_UTILS_H_

#include <QWidget>

class QIcon;

namespace sup::gui::utils
{

//! Size of tolbar icons.
QSize ToolBarIconSize();

//! Size of tolbar icons.
QSize NarrowToolBarIconSize();
}  // namespace sup::gui::utils

#endif  // SUP_GUI_WIDGETS_STYLE_UTILS_H_
