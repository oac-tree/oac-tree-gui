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

#ifndef OAC_TREE_GUI_VIEWMODEL_FAVORITE_INSTRUCTIONS_VIEWMODEL_H_
#define OAC_TREE_GUI_VIEWMODEL_FAVORITE_INSTRUCTIONS_VIEWMODEL_H_

#include <mvvm/viewmodel/viewmodel.h>

namespace oac_tree_gui
{

/**
 * @brief The FavoriteInstructionsViewModel class is a view model to show the list of favorite
 * instructions.
 *
 * It has following features:
 * - allow internal move (reordering instructions in a list)
 * - allow drag-copy from here
 * - allow drop of new instructions (copy from the toolbox's tree of available instructions)
 * - allow drop from the tree of main instruction editor (copies are created)
 */
class FavoriteInstructionsViewModel : public mvvm::ViewModel
{
  Q_OBJECT

public:
  explicit FavoriteInstructionsViewModel(mvvm::ISessionModel* model,
                                         QObject* parent_object = nullptr);

  int rowCount(const QModelIndex& index) const override;

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
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWMODEL_FAVORITE_INSTRUCTIONS_VIEWMODEL_H_
