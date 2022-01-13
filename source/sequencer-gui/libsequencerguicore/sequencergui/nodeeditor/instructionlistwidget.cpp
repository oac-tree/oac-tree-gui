/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/nodeeditor/instructionlistwidget.h"

#include "InstructionRegistry.h"
#include "sequencergui/nodeeditor/sceneutils.h"

#include "mvvm/widgets/widgetutils.h"

#include <QDrag>
#include <QMimeData>
#include <QPainter>
#include <QPixmap>

namespace
{
QPixmap createPixmap()
{
  QRect rect =
      QRect(0, 0, ModelView::Utils::WidthOfLetterM() * 4, ModelView::Utils::HeightOfLetterM() * 4);
  QPixmap pixmap(rect.width() + 1, rect.height() + 1);
  pixmap.fill(Qt::transparent);
  QPainter painter(&pixmap);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.setPen(Qt::white);
  painter.setBrush(sequi::ConnectableViewGradient(QColor(Qt::gray), rect));
  painter.drawRoundedRect(rect, 5, 5);
  return pixmap;
}

const int ModelTypeRole = Qt::UserRole;
const int PixmapRole = Qt::UserRole + 1;

const int column_width = 160;
}  // namespace

namespace sequi
{
InstructionListWidget::InstructionListWidget(QWidget* parent) : QListWidget(parent)
{
  setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

  setDragEnabled(true);

  auto rect = ConnectableViewRectangle();
  setIconSize(QSize(rect.width(), rect.height()));
  setAcceptDrops(false);
  setUniformItemSizes(true);

  populateList();
}

QString InstructionListWidget::piecesMimeType()
{
  return QStringLiteral("image/x-connectable-view");
}

QSize InstructionListWidget::sizeHint() const
{
  return QSize(column_width, 600);
}

//! Prepare data for dragging.

void InstructionListWidget::startDrag(Qt::DropActions)
{
  QListWidgetItem* item = currentItem();
  auto pixmap = item->data(PixmapRole).value<QPixmap>();
  QStringList dataToGo = QStringList() << item->data(Qt::UserRole).toString();

  auto mimeData = new QMimeData;
  mimeData->setData(piecesMimeType(), ModelView::Utils::GetByteArray(dataToGo));

  auto drag = new QDrag(this);
  drag->setMimeData(mimeData);
  drag->setPixmap(pixmap);
  drag->setHotSpot(QPoint(pixmap.width() / 2, pixmap.height() / 2));

  drag->exec(Qt::CopyAction);
}

//! Populates list with icons representing ConnectableView's to drop on scene.

void InstructionListWidget::populateList()
{
  auto names = ::sup::sequencer::GlobalInstructionRegistry().RegisteredInstructionNames();
  for (const auto& name : names)
  {
    addEntry(QString::fromStdString(name));
  }
  selectionModel()->reset();
}

//! Adds entry to the list. It will be the icon (gray rectangle) with the name.

void InstructionListWidget::addEntry(const QString& name)
{
  auto pieceItem = new QListWidgetItem;
  auto pixmap = createPixmap();
  //  pieceItem->setIcon(QIcon(pixmap));
  pieceItem->setData(Qt::DisplayRole, name);
  pieceItem->setData(ModelTypeRole, name);
  pieceItem->setData(PixmapRole, QVariant(pixmap));
  pieceItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);

  addItem(pieceItem);
}

}  // namespace sequi
