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

#include "instruction_toolkit_tree_view.h"

#include <oac_tree_gui/nodeeditor/scene_utils.h>

#include <mvvm/widgets/widget_utils.h>

#include <QDrag>
#include <QMimeData>
#include <QPainter>
#include <QPixmap>

namespace
{

/**
 * @brief Creates a pixmap for drag indicator.
 */
QPixmap CreatePixmap()
{
  QRect rect = QRect(0, 0, mvvm::utils::UnitSize(4), mvvm::utils::UnitSize(4));
  QPixmap pixmap(rect.width() + 1, rect.height() + 1);
  pixmap.fill(Qt::transparent);
  QPainter painter(&pixmap);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.setPen(Qt::white);
  painter.setBrush(oac_tree_gui::ConnectableViewGradient(QColor(Qt::gray), rect));
  painter.drawRoundedRect(rect, 5, 5);
  return pixmap;
}

}  // namespace

namespace oac_tree_gui
{

InsructionToolKitTreeView::InsructionToolKitTreeView(QWidget *parent_widget)
    : QTreeView(parent_widget)
{
  auto on_double_click = [this](auto index)
  {
    // For drag-enabled items, let's signal double click out, so we can provide quick way to insert
    // items.
    if (model()->flags(index).testFlag(Qt::ItemIsDragEnabled))
    {
      emit InstructionDoubleClicked(index.data(Qt::DisplayRole).toString());
    }
  };
  connect(this, &InsructionToolKitTreeView::doubleClicked, this, on_double_click);

  setHeaderHidden(true);
  setDragEnabled(true);
}

void InsructionToolKitTreeView::startDrag(Qt::DropActions supported_actions)
{
  (void)supported_actions;

  if (auto mime_data = model()->mimeData(selectedIndexes()); mime_data)
  {
    auto drag = new QDrag(this);   // drag ownership will be taken
    drag->setMimeData(mime_data);  // mime ownership is taken
    auto pixmap = CreatePixmap();
    drag->setPixmap(pixmap);
    drag->setHotSpot(QPoint(pixmap.width() / 2, pixmap.height() / 2));

    drag->exec(Qt::CopyAction);
  }
}

}  // namespace oac_tree_gui
