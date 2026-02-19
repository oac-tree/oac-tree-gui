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

#include "flatlist_delegate.h"

#include <oac_tree_gui/style/style_helper.h>

#include <mvvm/style/mvvm_style_helper.h>

#include <QApplication>
#include <QPainter>

namespace oac_tree_gui
{

FlatListDelegate::FlatListDelegate(QObject* parent) : QStyledItemDelegate(parent) {}

void FlatListDelegate::setCloseButtonVisible(bool visible)
{
  m_close_button_visible = visible;
}

void FlatListDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option,
                             const QModelIndex& index) const
{
  painter->setRenderHint(QPainter::Antialiasing);

  if (option.state.testFlag(QStyle::State_MouseOver))
  {
    const QBrush brush = option.palette.alternateBase();
    painter->fillRect(option.rect, brush);
  }

  QStyledItemDelegate::paint(painter, option, index);

  if (m_close_button_visible && index.column() == 1
      && option.state.testFlag(QStyle::State_MouseOver))
  {
    const QIcon icon = FindIcon("dialog-close-16");

    const QRect option_rect(option.rect.right() - option.rect.height(), option.rect.top(),
                            option.rect.height(), option.rect.height());

    QRect small_rect(0, 0, mvvm::style::UnitSize(1.2), mvvm::style::UnitSize(1.2));
    small_rect.moveCenter(option_rect.center());

    icon.paint(painter, small_rect, Qt::AlignRight | Qt::AlignVCenter);
  }
}

QSize FlatListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  constexpr double scale_default_height_factor{1.2};
  // increase height of the cell by 20%
  QSize result = QStyledItemDelegate::sizeHint(option, index);
  result.setHeight(static_cast<int>(result.height() * scale_default_height_factor));
  return result;
}

}  // namespace oac_tree_gui
