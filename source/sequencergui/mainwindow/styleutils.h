/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#ifndef SEQUENCERGUI_MAINWINDOW_STYLEUTILS_H
#define SEQUENCERGUI_MAINWINDOW_STYLEUTILS_H

#include <QWidget>

class QToolBar;

namespace sequi::StyleUtils
{

//! Size of tolbar icons.
QSize ToolBarIconSize();

//! Size of tolbar icons.
QSize NarrowToolBarIconSize();

}  // namespace sequi

#endif  // SEQUENCERGUI_MAINWINDOW_STYLEUTILS_H
