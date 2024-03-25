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

#ifndef SEQUENCERGUI_VIEWMODEL_DRAG_AND_DROP_HELPER_H_
#define SEQUENCERGUI_VIEWMODEL_DRAG_AND_DROP_HELPER_H_

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

namespace sequencergui
{

class InstructionItem;
class ProcedureItem;

//! Mime type for drag-and-drop event leading to the instruction move.
const QString kInstructionMoveMimeType = "application/coa.sequencer.instruction.move";

//! Mime type for drag-and-drop event leading to the creation of new instruction.
const QString kNewInstructionMimeType = "application/coa.sequencer.instruction.new";

//! Mime type for procedure copy.
const QString kCopyProcedureMimeType = "application/coa.sequencer.procedure.copy";

//! Mime type for instruction copy.
const QString kCopyInstructionMimeType = "application/coa.sequencer.instruction.copy";

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
                                       const mvvm::SessionItem& parent, int drop_indicator_row);

/**
 * @brief Calculate TagIndex corresponding to the drop indicator position.
 *
 * @param parent The parent which will get a new child.
 * @param drop_indicator_row Position of drop indicator as reported by QTreeView
 *
 * @return TagIndex to use for move operation
 */
mvvm::TagIndex GetDropTagIndex(int drop_indicator_row);

/**
 * @brief Process drop instruction request by creating and inserting corresponding item to given
 * parent.
 *
 * @param instruction_name Instruction type, or name of complex instruction aggregate.
 * @param parent Parent to insert an instruction.
 * @param tag_index Where to insert.
 *
 * @return Pointer to just inserted object.
 */
InstructionItem* DropInstruction(const std::string& instruction_type, mvvm::SessionItem* parent,
                                 const mvvm::TagIndex& tag_index);

/**
 * @brief Creates mime data to copy given item.
 *
 * @param item The item to copy.
 * @param mime_format String representing format.
 */
std::unique_ptr<QMimeData> CreateCopyMimeData(const mvvm::SessionItem& item,
                                              const QString& mime_format);

/**
 * @brief Creates item from given mime data.
 *
 * @param mime_data Mime data from copy operation.
 * @param mime_format String representing format.
 * @return New item.
 */
std::unique_ptr<mvvm::SessionItem> CreateSessionItem(const QMimeData* mime_data,
                                                     const QString& mime_format);

/**
 * @brief Creates mime data to copy given procedure.
 *
 * @param item The procedure to copy.
 */
std::unique_ptr<QMimeData> CreateProcedureCopyMimeData(const ProcedureItem& item);

/**
 * @brief Creates procedure item from mime data.
 */
std::unique_ptr<ProcedureItem> CreateProcedureItem(const QMimeData* mime_data);

/**
 * @brief Creates mime data to copy given instruction.
 *
 * @param item The instruction to copy.
 */
std::unique_ptr<QMimeData> CreateInstructionCopyMimeData(const InstructionItem& item);

/**
 * @brief Creates instruction item from mime data.
 */
std::unique_ptr<InstructionItem> CreateInstructionItem(const QMimeData* mime_data);

}  // namespace sequencergui

#endif  // SEQUENCERGUI_VIEWMODEL_DRAG_AND_DROP_HELPER_H_
