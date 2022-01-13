/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
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

#ifndef SEQUENCERGUI_MAINWINDOW_STYLEUTILS_H
#define SEQUENCERGUI_MAINWINDOW_STYLEUTILS_H

#include <QWidget>

class QToolBar;
class QIcon;

namespace sequi::StyleUtils
{

//! Size of tolbar icons.
QSize ToolBarIconSize();

//! Size of tolbar icons.
QSize NarrowToolBarIconSize();

QIcon GetIcon(const std::string& icon_name);
}  // namespace sequi

#endif  // SEQUENCERGUI_MAINWINDOW_STYLEUTILS_H
