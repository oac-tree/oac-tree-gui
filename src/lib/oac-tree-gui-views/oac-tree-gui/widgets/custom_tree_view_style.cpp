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

#include "custom_tree_view_style.h"

#include <QStyleOption>

namespace oac_tree_gui
{

CustomTreeViewStyle::CustomTreeViewStyle(QStyle *style) : QProxyStyle(style) {}

void CustomTreeViewStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option,
                                        QPainter *painter, const QWidget *widget) const
{
  if (element == QStyle::PE_IndicatorItemViewItemDrop && !option->rect.isNull())
  {
    QStyleOption opt(*option);
    if (widget)
    {
      opt.rect.setRight(widget->width());
    }
    QProxyStyle::drawPrimitive(element, &opt, painter, widget);
    return;
  }
  QProxyStyle::drawPrimitive(element, option, painter, widget);
}

}  // namespace oac_tree_gui
