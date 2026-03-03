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

//! Mime type for drag-and-drop events containing item's identifier information.
constexpr auto kItemIdentifierMimeType = "application/coa.sequencer.item.identifier";
constexpr auto kInstructionIdentifierMimeType = "application/coa.sequencer.instruction.identifier";
constexpr auto kJobIdentifierMimeType = "application/coa.sequencer.job.identifier";
constexpr auto kProcedureIdentifierMimeType = "application/coa.sequencer.procedure.identifier";

//! Mime type for drag-and-drop events in instruction editor. This is similar to
//! kInstructionIdentifierMimeType, but it is used to distinguish between moving instruction within
//! the procedure and dragging instruction from the favorite instructions list.
constexpr auto kInstructionEditorMimeType = "application/coa.sequencer.instruction.move";

//! Mime type for drag-and-drop events leading to the creation of new instruction.
constexpr auto kNewInstructionMimeType = "application/coa.sequencer.instruction.new";

//! Mime type for procedure copy.
constexpr auto kCopyProcedureMimeType = "application/coa.sequencer.procedure.copy";

//! Mime type for instruction copy.
constexpr auto kCopyInstructionMimeType = "application/coa.sequencer.instruction.copy";

//! Mime type for variable copy.
constexpr auto kCopyVariableMimeType = "application/coa.sequencer.variable.copy";


/**
  * @brief Returns list of indexes corresponding to the first column for each row in the given list of
 * indexes.
 */
QModelIndexList GetFirstColumnIndexes(const QModelIndexList& indexes);

/**
 * @brief Creates mime data carrying identifiers of items corersponding to given indexes.
 *
 * @param indexes List of indexes participating in drag-and-drop operation.
 * @param mime_type Mime type of the data to encode inside mime object.
 */
std::unique_ptr<QMimeData> CreateItemIdentifierMimeData(const QModelIndexList& indexes,
                                                        const QString& mime_type);

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
 * @brief Calculate TagIndex to move item to a parent within a tree.
 *
 * @param item Item which will be moved.
 * @param parent Its new parent.
 * @param drop_indicator_row Position of drop indicator as reported by QTreeView
 *
 * @return TagIndex to use for move operation
 */
mvvm::TagIndex GetTreeInternalMoveTagIndex(int32_t drop_indicator_row,
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
 * @brief Calculate TagIndex corresponding to the drop indicator position for internal move in the
 * list view.
 *
 * This functions is intended to be used for internal move operations in the views showing flat list
 * of items (e.g. FlatListViewModel) where the drop indicator position and parent's QModelIndex
 * are reported by QAbstractItemModel::dropMimeData.
 *
 * The only difference with GetTreeInternalMoveTagIndex is that dropping on top of a child will lead
 * to appending after that child (it never tries to insert into).
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

/**
 * @brief Checks if given mime data contains information about items that can be inserted into
 * the parent item at the position corresponding to drop indicator.
 *
 * Mime data is what AvailableInstructionsTreeView produces.
 *
 * @param data Mime data to check.
 * @param action Drop action.
 * @param drop_row_indicator Position of drop indicator as reported by QTreeView.
 * @param parent Parent item to check insert into.
 *
 * @return True if items described by mime data can be inserted, false otherwise.
 */
bool CanDropNewType(const QMimeData& data, Qt::DropAction action, int32_t drop_row_indicator,
                    const mvvm::SessionItem& parent);

/**
 * @brief Inserts new item described by mime data into the parent item at the position corresponding
 * to drop indicator.
 *
 * Mime data is what AvailableInstructionsTreeView produces.
 *
 * @param data Mime data describing item to insert.
 * @param action Drop action.
 * @param drop_row_indicator Position of drop indicator as reported by QTreeView.
 * @param parent Parent item to insert into.
 *
 * @return True if item was successfully inserted, false otherwise.
 */
bool HandleDropNewType(const QMimeData& data, Qt::DropAction action, int32_t drop_row_indicator,
                       mvvm::SessionItem& parent);

/**
 * @brief Checks if given mime data contains information about items that can be moved into
 * the parent item at the position corresponding to drop indicator.
 *
 * Mime data is what InstructionEditor produces.
 *
 * @param data Mime data to check.
 * @param action Drop action.
 * @param drop_row_indicator Position of drop indicator as reported by QTreeView.
 * @param parent Parent item to check move into.
 *
 * @return True if items described by mime data can be moved, false otherwise.
 */
bool CanDropInstructionEditorMimeData(const QMimeData& data, Qt::DropAction action,
                                      int32_t drop_row_indicator, const mvvm::SessionItem& parent);

/**
 * @brief Moves items described by mime data into the parent item at the position corresponding to
 * drop indicator.
 *
 * Mime data is what InstructionEditor produces.
 *
 * @param data Mime data describing items to move.
 * @param action Drop action.
 * @param drop_row_indicator Position of drop indicator as reported by QTreeView.
 * @param parent Parent item to move into.
 *
 * @return True if items were successfully moved, false otherwise.
 */
bool HandleDropInstructionEditorMimeData(const QMimeData& data, Qt::DropAction action,
                                         int32_t drop_row_indicator, mvvm::SessionItem& parent);

/**
 * @brief Checks if given mime data contains information about instructions that can be copied into
 * the parent item at the position corresponding to drop indicator.
 *
 * Mime data represents generic item copy.
 *
 * @param data Mime data to check.
 * @param action Drop action.
 * @param drop_row_indicator Position of drop indicator as reported by QTreeView.
 * @param parent Parent item to check copy into.
 *
 * @return True if items described by mime data can be copied, false otherwise.
 */
bool CanDropInstructionCopyMimeData(const QMimeData& data, Qt::DropAction action,
                                    int32_t drop_row_indicator, const mvvm::SessionItem& parent);

/**
 * @brief Copies instructions described by mime data into the parent item at the position
 * corresponding to drop indicator.
 *
 * Mime data represents generic item copy.
 *
 * @param data Mime data describing items to copy.
 * @param action Drop action.
 * @param drop_row_indicator Position of drop indicator as reported by QTreeView.
 * @param parent Parent item to copy into.
 *
 * @return True if items were successfully copied, false otherwise.
 */
bool HandleDropInstructionCopyMimeData(const QMimeData& data, Qt::DropAction action,
                                       int32_t drop_row_indicator, mvvm::SessionItem &parent);

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_COMPONENTS_DRAG_AND_DROP_HELPER_H_
