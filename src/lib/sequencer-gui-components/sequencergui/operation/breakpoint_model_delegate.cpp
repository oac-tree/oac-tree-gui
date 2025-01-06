/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
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

#include "breakpoint_model_delegate.h"

#include <sequencergui/operation/breakpoint_types.h>
#include <sequencergui/viewmodel/instruction_operation_viewmodel.h>

#include <mvvm/widgets/widget_utils.h>

#include <QPainter>

namespace
{
const QColor kBreakpointPenColor("#df5050");
const QColor kBreakpointSetColor("#df5050");
const QColor kBreakpointDisabledColor("#ecd0d1");

void PaintBreakpoint(QPainter *painter, const QRect &rect, const QColor &base_color)
{
  painter->setRenderHint(QPainter::Antialiasing);
  painter->setPen(kBreakpointPenColor);
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
      PaintBreakpoint(painter, option.rect, kBreakpointSetColor);
    }
    else if (status == BreakpointStatus::kDisabled)
    {
      PaintBreakpoint(painter, option.rect, kBreakpointDisabledColor);
    }
  }
  else
  {
    QStyledItemDelegate::paint(painter, option, index);
  }
}

}  // namespace sequencergui
