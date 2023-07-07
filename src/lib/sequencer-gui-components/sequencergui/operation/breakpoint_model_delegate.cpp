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

#include "breakpoint_model_delegate.h"

#include <sequencergui/operation/breakpoint_types.h>

#include <mvvm/widgets/widget_utils.h>

#include <QDebug>
#include <QPainter>

namespace
{
const QColor kColorSet("#df5050");
const QColor kColorDisabled("#ecd0d1");

void PaintBreakpoint(QPainter *painter, const QRect &rect, const QColor &base_color)
{
  painter->setRenderHint(QPainter::Antialiasing);
  painter->setPen(kColorSet);
  painter->setBrush(QBrush(base_color));

  const auto x_size = mvvm::utils::UnitSize() * 0.8;

  QRect ellipse_rect(0.0, 0.0, x_size, x_size);
  ellipse_rect.moveCenter(rect.center());
  painter->drawEllipse(ellipse_rect);
}

}  // namespace

namespace sequencergui
{

BreakpointModelDelegate::BreakpointModelDelegate(QObject *parent) : QStyledItemDelegate(parent) {}

void BreakpointModelDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                                    const QModelIndex &index) const
{
  if (index.column() == 3)
  {
    if (option.state & QStyle::State_Selected)
    {
      painter->fillRect(option.rect, option.palette.highlight());
    }

    BreakpointStatus status = static_cast<BreakpointStatus>(index.data().toInt());
    qDebug() << "aaa" << index.data().toInt();
    if (status == BreakpointStatus::kSet)
    {
      PaintBreakpoint(painter, option.rect, kColorSet);
    }
    else if (status == BreakpointStatus::kDisabled)
    {
      PaintBreakpoint(painter, option.rect, kColorDisabled);
    }
  }
  else
  {
    QStyledItemDelegate::paint(painter, option, index);
  }
}

}  // namespace sequencergui
