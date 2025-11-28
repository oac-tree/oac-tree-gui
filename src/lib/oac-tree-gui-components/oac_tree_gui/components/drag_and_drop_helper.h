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

//! Mime type for drag-and-drop event leading to the instruction move.
constexpr auto kInstructionMoveMimeType = "application/coa.sequencer.instruction.move";

//! Mime type for drag-and-drop event leading to the creation of new instruction.
constexpr auto kNewInstructionMimeType = "application/coa.sequencer.instruction.new";

//! Mime type for procedure copy.
constexpr auto kCopyProcedureMimeType = "application/coa.sequencer.procedure.copy";

//! Mime type for instruction copy.
constexpr auto kCopyInstructionMimeType = "application/coa.sequencer.instruction.copy";

//! Mime type for variable copy.
constexpr auto kCopyVariableMimeType = "application/coa.sequencer.variable.copy";

/**
 * @brief Creates mime data carrying information for instruction move.
 *
 * @param indexes List of indexes participating in move operation.
 */
std::unique_ptr<QMimeData> CreateInstructionMoveMimeData(const QModelIndexList& indexes);

/**
 * @brief Returns vector of identifiers representing instructions involved into the drag-and-move
 * operation.
 */
std::vector<std::string> GetIdentifiersToMove(const QMimeData* mime_data);

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
mvvm::TagIndex GetInternalMoveTagIndex(const mvvm::SessionItem& item,
                                       const mvvm::SessionItem& parent,
                                       std::int32_t drop_indicator_row);

/**
 * @brief Calculate TagIndex corresponding to the drop indicator position.
 *
 * @param parent The parent which will get a new child.
 * @param drop_indicator_row Position of drop indicator as reported by QTreeView
 *
 * @return TagIndex to use for move operation
 */
mvvm::TagIndex GetDropTagIndex(std::int32_t drop_indicator_row);
}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_COMPONENTS_DRAG_AND_DROP_HELPER_H_
