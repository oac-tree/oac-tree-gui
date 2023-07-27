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

#ifndef SEQUENCERGUI_VIEWMODEL_DRAG_AND_DROP_HELPER_H_
#define SEQUENCERGUI_VIEWMODEL_DRAG_AND_DROP_HELPER_H_

//! Helper functions for drag-and-drop operations.

#include <QModelIndexList>
#include <memory>
#include <string>
#include <vector>

class QMimeData;

namespace sequencergui
{

const QString kInstructionMoveMimeType = "application/coa.sequencer.instruction.move";

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

}  // namespace sequencergui

#endif  // SEQUENCERGUI_VIEWMODEL_DRAG_AND_DROP_HELPER_H_
