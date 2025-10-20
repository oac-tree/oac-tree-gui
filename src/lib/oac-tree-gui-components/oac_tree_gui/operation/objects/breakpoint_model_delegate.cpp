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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "breakpoint_model_delegate.h"

#include <oac_tree_gui/operation/breakpoint_types.h>
#include <oac_tree_gui/viewmodel/instruction_operation_viewmodel.h>

#include <mvvm/style/mvvm_style_helper.h>

#include <QPainter>

namespace
{
const QColor kBreakpointPenColor("#df5050");
const QColor kBreakpointSetColor("#df5050");
const QColor kBreakpointDisabledColor("#ecd0d1");

void PaintBreakpoint(QPainter *painter, const QPointF &center, const QPen &pen, const QBrush &brush,
                     double scale = 0.8)
{
  painter->save();
  painter->setRenderHint(QPainter::Antialiasing);
  painter->setPen(pen);
  painter->setBrush(brush);

  const auto x_size = mvvm::style::UnitSize(scale);

  QRectF ellipse_rect(0.0, 0.0, x_size, x_size);
  ellipse_rect.moveCenter(center);
  painter->drawEllipse(ellipse_rect);
  painter->restore();
}

void PaintSetBreakpoint(QPainter *painter, const QPointF &center)
{
  PaintBreakpoint(painter, center, kBreakpointSetColor, kBreakpointSetColor);
}

void PaintDisabledBreakpoint(QPainter *painter, const QPointF &center)
{
  PaintBreakpoint(painter, center, kBreakpointSetColor, kBreakpointDisabledColor);
}

void PaintHitBreakpoint(QPainter *painter, const QPointF &center)
{
  PaintBreakpoint(painter, center, kBreakpointSetColor, Qt::NoBrush, 1.2);
  PaintSetBreakpoint(painter, center);
}

}  // namespace

namespace oac_tree_gui
{

BreakpointModelDelegate::BreakpointModelDelegate(QObject *parent) : QStyledItemDelegate(parent) {}

void BreakpointModelDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                                    const QModelIndex &index) const
{
  if (index.column() == InstructionOperationViewModel::GetBreakpointColumn())
  {
    if (option.state & QStyle::State_Selected)
    {
      // to highlight background when the whole row is selected
      painter->fillRect(option.rect, option.palette.highlight());
    }

    BreakpointStatus status = static_cast<BreakpointStatus>(index.data().toInt());
    if (status == BreakpointStatus::kSet)
    {
      PaintSetBreakpoint(painter, option.rect.center());
    }
    else if (status == BreakpointStatus::kDisabled)
    {
      PaintDisabledBreakpoint(painter, option.rect.center());
    }
    else if (status == BreakpointStatus::kSetAndHit)
    {
      PaintHitBreakpoint(painter, option.rect.center());
    }
  }
  else
  {
    QStyledItemDelegate::paint(painter, option, index);
  }
}

}  // namespace oac_tree_gui
