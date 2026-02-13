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

#ifndef OAC_TREE_GUI_COMPONENTS_DRAG_AND_DROP_HELPER_H_
#define OAC_TREE_GUI_COMPONENTS_DRAG_AND_DROP_HELPER_H_

//! Helper functions for drag-and-drop operations.

#include <QModelIndexList>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace mvvm
{
class TagIndex;
class SessionItem;
}  // namespace mvvm

class QMimeData;

namespace oac_tree_gui
{

class InstructionItem;
class ProcedureItem;

//! Mime type for drag-and-drop events containing item's identifier information.
//! Depending on view model implementation, view context and the Qt::DropAction can lead to item
//! internal move, copy or creation of new item based on the dragged one.
constexpr auto kItemIdentifierMimeType = "application/coa.sequencer.item.identifier";

//! Mime type for drag-and-drop events leading to the instruction move.
constexpr auto kInstructionMoveMimeType = "application/coa.sequencer.instruction.move";

//! Mime type for drag-and-drop events leading to the creation of new instruction.
constexpr auto kNewInstructionMimeType = "application/coa.sequencer.instruction.new";

//! Mime type for procedure copy.
constexpr auto kCopyProcedureMimeType = "application/coa.sequencer.procedure.copy";

//! Mime type for instruction copy.
constexpr auto kCopyInstructionMimeType = "application/coa.sequencer.instruction.copy";

//! Mime type for variable copy.
constexpr auto kCopyVariableMimeType = "application/coa.sequencer.variable.copy";

//! index corresponding to the situation when user drop on viewport area outside of any item's list
const std::int32_t kViewportDropIndex = -1;

/**
 * @brief Returns vector of identifiers representing instructions involved into the drag-and-move
 * operation.
 *
 * @param mime_data Mime data carrying the list of strings.
 * @param mime_type Expected mime type of the data.
 */
std::vector<std::string> GetStringListFromMime(const QMimeData* mime_data,
                                               const QString& mime_type);

/**
 * @brief Creates mime data carrying identifiers of items corersponding to given indexes.
 *
 * @param indexes List of indexes participating in drag-and-drop operation.
 * @param mime_type Mime type of the data to encode inside mime object.
 */
std::unique_ptr<QMimeData> CreateItemIdentifierMimeData(const QModelIndexList& indexes,
                                                        const QString& mime_type);

/**
 * @brief Creates mime data carrying information for instruction move.
 *
 * @param indexes List of indexes participating in move operation.
 */
std::unique_ptr<QMimeData> CreateInstructionMoveMimeData(const QModelIndexList& indexes);

/**
 * @brief Creates mime data carrying information for new instruction creation.
 *
 * @param name The model type name of the instruction.
 */
std::unique_ptr<QMimeData> CreateNewInstructionMimeData(const QString& name);

/**
 * @brief Returns type of the instruction to create encoded in given mime data.
 */
std::string GetNewInstructionType(const QMimeData* mime_data);

/**
 * @brief Calculate TagIndex to move item to a parent.
 *
 * @param item Item which will be moved.
 * @param parent Its new parent.
 * @param drop_indicator_row Position of drop indicator as reported by QTreeView
 *
 * @return TagIndex to use for move operation
 */
mvvm::TagIndex GetInternalMoveTagIndex(int32_t drop_indicator_row,
                                       const mvvm::TagIndex& source_tag_index,
                                       const QModelIndex& parent);

mvvm::TagIndex GetInternalMoveTagIndexV2(int32_t drop_indicator_row,
                                       const mvvm::SessionItem& child,
                                       const mvvm::SessionItem& parent);

/**
 * @brief Calculate TagIndex corresponding to the drop indicator position.
 *
 * @param parent The parent which will get a new child.
 * @param drop_indicator_row Position of drop indicator as reported by QTreeView
 *
 * @return TagIndex to use for move operation
 */
mvvm::TagIndex GetDropTagIndex(std::int32_t drop_indicator_row);

/**
 * @brief Calculate item index corresponding to the drop indicator position.
 *
 * This functions is intended to be used for internal move operations in the views showing flat list
 * of items (e.g. FlatListViewModel) where the drop indicator position and parent's viewmodelindex
 * are reported by QAbstractItemModel::dropMimeData.
 *
 * @param drop_indicator_row Position of drop indicator as reported by QListView.
 * @param source_row Index of item being moved.
 * @param parent The parent index as reported by QListView.
 *
 * @return Index for TagIndex construction corresponding to the drop indicator position.
 *
 */
std::int32_t GetListInternalMoveIndex(std::int32_t drop_indicator_row, std::int32_t source_index,
                                      const QModelIndex& parent);

/**
 * @brief Calculate TagIndex corresponding to the drop indicator position for internal move in the
 * list view.
 *
 * This functions is intended to be used for internal move operations in the views showing flat list
 * of items (e.g. FlatListViewModel) where the drop indicator position and parent's viewmodelindex
 * are reported by QAbstractItemModel::dropMimeData.
 *
 * @param drop_indicator_row Position of drop indicator as reported by QListView.
 * @param source_tag_index TagIndex of the item being moved.
 * @param parent The parent index as reported by QListView.
 *
 * @return TagIndex corresponding to the drop indicator position.
 */
mvvm::TagIndex GetListInternalMoveTagIndex(std::int32_t drop_indicator_row,
                                           const mvvm::TagIndex& source_tag_index,
                                           const QModelIndex& parent);
}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_COMPONENTS_DRAG_AND_DROP_HELPER_H_
