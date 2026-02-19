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

#include <QApplication>
#include <QPainter>

namespace oac_tree_gui
{

FlatListDelegate::FlatListDelegate(QObject* parent) : QStyledItemDelegate(parent) {}

void FlatListDelegate::setCloseButtonVisible(bool visible)
{
  closeButtonVisible = visible;
}

void FlatListDelegate::handlePressed(const QModelIndex& index)
{
  if (index.column() == 1)
    pressedIndex = index;
}

void FlatListDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option,
                                  const QModelIndex& index) const
{
  if (option.state & QStyle::State_MouseOver)
  {
    if ((QApplication::mouseButtons() & Qt::LeftButton) == 0)
      pressedIndex = QModelIndex();
    QBrush brush = option.palette.alternateBase();
    if (index == pressedIndex)
      brush = option.palette.dark();
    painter->fillRect(option.rect, brush);
  }

  QStyledItemDelegate::paint(painter, option, index);

  if (closeButtonVisible && index.column() == 1 && option.state & QStyle::State_MouseOver)
  {
    const QIcon icon = FindIcon("dialog-close-16");

    QRect iconRect(option.rect.right() - option.rect.height(), option.rect.top(),
                   option.rect.height(), option.rect.height());

    icon.paint(painter, iconRect, Qt::AlignRight | Qt::AlignVCenter);
  }
}

}  // namespace oac_tree_gui
