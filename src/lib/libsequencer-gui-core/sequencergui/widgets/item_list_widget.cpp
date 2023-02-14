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

#include "sequencergui/widgets/item_list_widget.h"

#include <sequencergui/nodeeditor/scene_utils.h>
#include <sequencergui/utils/style_utils.h>

#include <mvvm/widgets/widget_utils.h>

#include <QDrag>
#include <QMimeData>
#include <QPainter>
#include <QPixmap>

namespace
{
QPixmap createPixmap()
{
  QRect rect =
      QRect(0, 0, sequencergui::styleutils::UnitSize(4), sequencergui::styleutils::UnitSize(4));
  QPixmap pixmap(rect.width() + 1, rect.height() + 1);
  pixmap.fill(Qt::transparent);
  QPainter painter(&pixmap);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.setPen(Qt::white);
  painter.setBrush(sequencergui::ConnectableViewGradient(QColor(Qt::gray), rect));
  painter.drawRoundedRect(rect, 5, 5);
  return pixmap;
}

const int kModelTypeRole = Qt::UserRole;
const int kPixmapRole = Qt::UserRole + 1;
const int kColumnWidth = 160;
}  // namespace

namespace sequencergui
{
ItemListWidget::ItemListWidget(QWidget* parent) : QListWidget(parent)
{
  setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

  setDragEnabled(true);

  auto rect = ConnectableViewRectangle();
  setIconSize(QSize(rect.width(), rect.height()));
  setAcceptDrops(false);
  setUniformItemSizes(true);
}

QString ItemListWidget::piecesMimeType()
{
  return QStringLiteral("image/x-connectable-view");
}

QSize ItemListWidget::sizeHint() const
{
  return {kColumnWidth, 600};
}

void ItemListWidget::AddEntries(const QStringList& entries)
{
  for (const auto& entry : entries)
  {
    addEntry(entry);
  }
  selectionModel()->reset();
}

//! Prepare data for dragging.

void ItemListWidget::startDrag(Qt::DropActions)
{
  QListWidgetItem* item = currentItem();
  auto pixmap = item->data(kPixmapRole).value<QPixmap>();
  QStringList data_togo = QStringList() << item->data(Qt::UserRole).toString();

  auto mime_data = new QMimeData;
  mime_data->setData(piecesMimeType(), mvvm::utils::GetByteArray(data_togo));

  auto drag = new QDrag(this);
  drag->setMimeData(mime_data);
  drag->setPixmap(pixmap);
  drag->setHotSpot(QPoint(pixmap.width() / 2, pixmap.height() / 2));

  drag->exec(Qt::CopyAction);
}

//! Adds entry to the list. It will be the icon (gray rectangle) with the name.

void ItemListWidget::addEntry(const QString& name)
{
  auto pieceItem = new QListWidgetItem;
  auto pixmap = createPixmap();
  //  pieceItem->setIcon(QIcon(pixmap));
  pieceItem->setData(Qt::DisplayRole, name);
  pieceItem->setData(kModelTypeRole, name);
  pieceItem->setData(kPixmapRole, QVariant(pixmap));
  pieceItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);

  addItem(pieceItem);
}

}  // namespace sequencergui
