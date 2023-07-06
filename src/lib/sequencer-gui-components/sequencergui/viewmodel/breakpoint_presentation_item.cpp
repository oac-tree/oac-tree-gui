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

#include <QPainter>
#include <QPixmap>

namespace
{

QPixmap CreateBreakpointSetPixmap()
{
  QPixmap pixmap(20, 20);
  pixmap.fill(Qt::red);

  QPainter painter(&pixmap);
  painter.setPen(Qt::black);

  painter.drawRect(10, 10, 20, 20);
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
  if (qt_role == Qt::DecorationRole)
  {
    return CreateBreakpointSetPixmap();
  }

  return QVariant();
}

}  // namespace sequencergui
