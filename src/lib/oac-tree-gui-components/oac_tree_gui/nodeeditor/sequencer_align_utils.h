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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef OAC_TREE_GUI_NODEEDITOR_SEQUENCER_ALIGN_UTILS_H_
#define OAC_TREE_GUI_NODEEDITOR_SEQUENCER_ALIGN_UTILS_H_

//! Collection of utility functions to align oac-tree instruction tree on the graphics scene.

#include <memory>
#include <vector>

class QPointF;

namespace oac_tree_gui
{
class InstructionItem;
}  // namespace oac_tree_gui

namespace oac_tree_gui::algorithm
{

class AlignNode;

//! Creates AlignTree representing instruction hierarchy. The tree will
//! be used by the align algorithm to nicely position instructions on the graphics scene.
std::unique_ptr<AlignNode> CreateAlignTree(std::vector<InstructionItem*> instructions);
std::unique_ptr<AlignNode> CreateAlignTree(const InstructionItem* instruction);

//! Translate positions of node to the coordinates of the graphics scene
void TranslatePositions(const QPointF& reference, AlignNode& root_node);

//! Update positions of instruction items using positions of nodes.
void UpdatePositions(const AlignNode* node, InstructionItem* item);
void UpdatePositions(const AlignNode* node, std::vector<InstructionItem*> instructions);

//! Align children of given instruction on graphics scene.  The position of parent `instruction`
//! remains unchanged.
void AlignInstructionTreeWalker(const QPointF& reference, InstructionItem* instruction);

void AlignInstructionTreeWalker(const QPointF& reference,
                                std::vector<InstructionItem*> instructions);

//! Returns true if given instruction and its children require alignment on the graphics scene.
bool RequiresInitialAlignment(const InstructionItem& instruction);

//! Returns true if given instructionrequire alignment on the graphics scene.
bool RequiresInitialAlignment(const std::vector<InstructionItem*>& instructions);

}  // namespace oac_tree_gui::algorithm

#endif  // OAC_TREE_GUI_NODEEDITOR_SEQUENCER_ALIGN_UTILS_H_
