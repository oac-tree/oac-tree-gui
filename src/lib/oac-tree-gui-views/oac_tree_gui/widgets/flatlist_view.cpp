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

#include "flatlist_view.h"

#include <QHeaderView>
#include <QMouseEvent>

namespace oac_tree_gui
{

FlatListView::FlatListView(QWidget* parent) : QTreeView(parent)
{
  setRootIsDecorated(false);
  setTextElideMode(Qt::ElideMiddle);
  setFrameStyle(QFrame::NoFrame);
  setAttribute(Qt::WA_MacShowFocusRect, false);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  viewport()->setAttribute(Qt::WA_Hover);

  setSelectionMode(QAbstractItemView::SingleSelection);
  setSelectionBehavior(QAbstractItemView::SelectRows);
}

void FlatListView::setModel(QAbstractItemModel* model)
{
  QTreeView::setModel(model);
  header()->hide();
  header()->setStretchLastSection(false);
  header()->setSectionResizeMode(0, QHeaderView::Stretch);
  header()->setSectionResizeMode(1, QHeaderView::Fixed);
  header()->setMinimumSectionSize(0);
  header()->resizeSection(1, 16);
}

void FlatListView::mousePressEvent(QMouseEvent* e)
{
  // ignore press in "close button" column to avoid selection
  if (indexAt(e->position().toPoint()).column() != 1)
  {
    QTreeView::mousePressEvent(e);
  }
}

void FlatListView::mouseReleaseEvent(QMouseEvent* e)
{
  // manually handle click in "close button" column
  // to avoid selection
  const QModelIndex mouseIndex = indexAt(e->position().toPoint());
  if (mouseIndex.column() == 1)
  {
    emit activated(mouseIndex);
  }
  else
  {
    QTreeView::mouseReleaseEvent(e);
  }
}

}  // namespace oac_tree_gui
