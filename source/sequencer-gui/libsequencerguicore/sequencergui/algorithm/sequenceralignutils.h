/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_ALGORITHM_SEQUENCERALIGNUTILS_H
#define SEQUENCERGUI_ALGORITHM_SEQUENCERALIGNUTILS_H

//! @file sequenceralignutils.h
//! Collection of utility functions to align Sequencer instruction tree on the graphics scene.

#include <memory>

class QPointF;

namespace sequencergui
{
class InstructionItem;
class InstructionContainerItem;
}

namespace sequencergui::algorithm
{

class AlignNode;


//! Creates AlignTree representing instruction hierarchy. The tree will
//! be used by the align algorithm to nicely position instructions on the graphics scene.
std::unique_ptr<AlignNode> CreateAlignTree(const InstructionContainerItem* container);
std::unique_ptr<AlignNode> CreateAlignTree(const InstructionItem* container);

//! Creates AlignTree representing instruction hierarchy. The tree will
void UpdatePositions(const AlignNode* node, InstructionContainerItem* container);
void UpdatePositions(const AlignNode* node, InstructionItem* item);

//! Align children of given instruction on graphics scene.  When `force` is false allignment is
//! performed only for instructions with unitialised coordinates.
//! The position of parent `instruction` remains unchanged.
void AlignInstructionTreeWalker(const QPointF& reference, InstructionItem* instruction, bool force);

void AlignInstructionTreeWalker(const QPointF& reference, InstructionContainerItem* container, bool force);

}  // namespace sequencergui::algorithm

#endif  // SEQUENCERGUI_ALGORITHM_SEQUENCERALIGNUTILS_H
