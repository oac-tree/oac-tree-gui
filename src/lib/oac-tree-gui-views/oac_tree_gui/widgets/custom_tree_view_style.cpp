/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "custom_tree_view_style.h"

#include <QStyleOption>

namespace oac_tree_gui
{

CustomTreeViewStyle::CustomTreeViewStyle(QStyle* style) : QProxyStyle(style) {}

void CustomTreeViewStyle::drawPrimitive(PrimitiveElement element, const QStyleOption* option,
                                        QPainter* painter, const QWidget* widget) const
{
  if (element == QStyle::PE_IndicatorItemViewItemDrop && !option->rect.isNull())
  {
    QStyleOption opt(*option);
    if (widget != nullptr)
    {
      opt.rect.setRight(widget->width());
    }
    QProxyStyle::drawPrimitive(element, &opt, painter, widget);
    return;
  }
  QProxyStyle::drawPrimitive(element, option, painter, widget);
}

QRect CustomTreeViewStyle::subElementRect(SubElement element, const QStyleOption* option,
                                          const QWidget* widget) const
{
  QRect rect = QProxyStyle::subElementRect(element, option, widget);

  if (element != SE_ItemViewItemCheckIndicator || !option)
  {
    return rect;
  }

  const auto* viewOpt = qstyleoption_cast<const QStyleOptionViewItem*>(option);
  if (!viewOpt)
  {
    return rect;
  }

  // Apply only to "check-only" cells: checkbox exists, text does not.
  const auto features = viewOpt->features;
  const bool hasCheck = features.testFlag(QStyleOptionViewItem::HasCheckIndicator);
  const bool hasDisplay = features.testFlag(QStyleOptionViewItem::HasDisplay);
  if (!hasCheck || hasDisplay)
  {
    return rect;
  }

  const QRect cell = viewOpt->rect;
  const int x = cell.x() + (cell.width() - rect.width()) / 2;
  const int y = cell.y() + (cell.height() - rect.height()) / 2;
  return QRect(QPoint(x, y), rect.size());
}

}  // namespace oac_tree_gui
