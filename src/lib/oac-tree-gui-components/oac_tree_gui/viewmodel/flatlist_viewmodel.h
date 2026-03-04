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

#ifndef OAC_TREE_GUI_VIEWMODEL_FLATLIST_VIEWMODEL_H_
#define OAC_TREE_GUI_VIEWMODEL_FLATLIST_VIEWMODEL_H_

#include <mvvm/viewmodel/viewmodel.h>

namespace oac_tree_gui
{

/**
 * @brief The FlatListViewModel is intended to show the list of the top level items in the
 * container.
 *
 * It can be the list of procedures, the list of jobs or the list of top-level instructions.
 *
 * Main features are:
 * - do not go down to item hierarchy (like instruction tree) and shows only the top level items
 * - can be used together with QTreeView and there it will look like list too
 * - allows internal item move inside the container
 *
 * For item reordering it is necessary, that the view is configured with
 * view->setDefaultDropAction(Qt::MoveAction) <-- preferably
 * or
 * view->setDragDropMode(QAbstractItemView::InternalMove);
 */
class FlatListViewModel : public mvvm::ViewModel
{
  Q_OBJECT

public:
  explicit FlatListViewModel(const QStringList& expected_mime_types,
                             QObject* parent_object = nullptr);

  int rowCount(const QModelIndex& index = {}) const override;

  // drag-and-drop support

  QStringList mimeTypes() const override;

  QMimeData* mimeData(const QModelIndexList& indexes) const override;

  bool canDropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column,
                       const QModelIndex& parent) const override;

  bool dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column,
                    const QModelIndex& parent) override;

  Qt::DropActions supportedDropActions() const override;

  Qt::DropActions supportedDragActions() const override;

  Qt::ItemFlags flags(const QModelIndex& index) const override;

private:
  /**
   * @brief Returns parent and drop indicator position for drop at given row and parent index.
   *
   * In this list we treat drop on top of an item as drop before that item, so we adjust drop
   * indicator position accordingly.
   */
  std::pair<const mvvm::SessionItem*, std::int32_t> GetDropTarget(std::int32_t row,
                                                                  const QModelIndex& parent) const;
  QString GetPrimaryMimeType() const;

  QStringList m_expected_mime_types;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWMODEL_FLATLIST_VIEWMODEL_H_
