/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#include "sequencergui/mainwindow/styleutils.h"
#include <QSize>

namespace sequi::StyleUtils
{

QSize ToolBarIconSize()
{
  return QSize(24, 24);
}

QSize NarrowToolBarIconSize()
{
  return QSize(20, 20);
}

}
