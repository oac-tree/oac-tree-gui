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

#include "breakpoint_presentation_item.h"

#include <mvvm/model/sessionitem.h>
#include <mvvm/widgets/widget_utils.h>

#include <QPainter>
#include <QPixmap>

namespace
{

QPixmap CreateBreakpointPixmap(QColor fill_color)
{
  const auto x_size = mvvm::utils::UnitSize();
  QPixmap pixmap(x_size * 1.0, x_size * 1.0);
  pixmap.fill(Qt::transparent);
  QPainter painter(&pixmap);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.setPen(QColor("#df5050"));
  painter.setBrush(QBrush(fill_color));

  painter.drawEllipse(QRectF(0.1, 0.1, x_size * 0.8, x_size * 0.8));
  return pixmap;
}

}  // namespace

namespace sequencergui
{

BreakpointPresentationItem::BreakpointPresentationItem(mvvm::SessionItem *item)
    : SessionItemPresentation(item, mvvm::DataRole::kData)
{
}

QVariant BreakpointPresentationItem::Data(int qt_role) const
{
  static const QPixmap pixmap_set = CreateBreakpointPixmap("#df5050");
  static const QPixmap pixmap_disabled = CreateBreakpointPixmap("#ecd0d1");

  if (qt_role == Qt::DecorationRole)
  {
    return pixmap_disabled;
  }

  return QVariant();
}

}  // namespace sequencergui
