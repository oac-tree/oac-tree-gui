/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "instruction_toolkit_tree_view.h"

#include <sequencergui/nodeeditor/scene_utils.h>

#include <mvvm/widgets/widget_utils.h>

#include <QDrag>
#include <QMimeData>
#include <QPainter>
#include <QPixmap>

namespace sequencergui
{

QPixmap createPixmap()
{
  QRect rect = QRect(0, 0, mvvm::utils::UnitSize(4), mvvm::utils::UnitSize(4));
  QPixmap pixmap(rect.width() + 1, rect.height() + 1);
  pixmap.fill(Qt::transparent);
  QPainter painter(&pixmap);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.setPen(Qt::white);
  painter.setBrush(sequencergui::ConnectableViewGradient(QColor(Qt::gray), rect));
  painter.drawRoundedRect(rect, 5, 5);
  return pixmap;
}

void InsructionToolKitTreeView::startDrag(Qt::DropActions supportedActions)
{
  if (auto mime_data = model()->mimeData(selectedIndexes()); mime_data)
  {
    auto drag = new QDrag(this);
    drag->setMimeData(mime_data);  // ownership is taken
    auto pixmap = createPixmap();
    drag->setPixmap(pixmap);
    drag->setHotSpot(QPoint(pixmap.width() / 2, pixmap.height() / 2));

    drag->exec(Qt::CopyAction);
  }
}

}  // namespace sequencergui
