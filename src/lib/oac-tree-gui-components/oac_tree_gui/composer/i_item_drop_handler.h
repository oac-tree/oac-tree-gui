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

#ifndef OAC_TREE_GUI_COMPOSER_I_ITEM_DROP_HANDLER_H_
#define OAC_TREE_GUI_COMPOSER_I_ITEM_DROP_HANDLER_H_

#include <QMimeData>

namespace oac_tree_gui
{

/**
 * @brief The IItemDropHandler class is an interface for all handlers that implements concrete
 * drag-and-drop logic.
 *
 * Used in the context of InstructionTreeView, JobListView, ProcedureListView and similar.
 *
 * Contains the logic to handle drag-copy and drag-move operations from internal source (within the
 * same view) and from external source.
 */
class IItemDropHandler
{
public:
  IItemDropHandler() = default;
  virtual ~IItemDropHandler() = default;

  IItemDropHandler(const IItemDropHandler&) = delete;
  IItemDropHandler& operator=(const IItemDropHandler&) = delete;
  IItemDropHandler(IItemDropHandler&&) = delete;
  IItemDropHandler& operator=(IItemDropHandler&&) = delete;

  virtual bool CanDropMimeData(const QMimeData* data, Qt::DropAction action,
                               std::int32_t drop_row_indicator, const QModelIndex& parent) = 0;

  virtual bool DropMimeData(const QMimeData* data, Qt::DropAction action,
                            std::int32_t drop_row_indicator, const QModelIndex& parent) = 0;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_COMPOSER_I_ITEM_DROP_HANDLER_H_
