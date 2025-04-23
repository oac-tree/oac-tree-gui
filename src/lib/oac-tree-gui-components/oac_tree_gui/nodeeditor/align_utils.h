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

#ifndef OAC_TREE_GUI_NODEEDITOR_ALIGN_UTILS_H_
#define OAC_TREE_GUI_NODEEDITOR_ALIGN_UTILS_H_

//! Collection of utility function for node positioning of the general tree.
//! The tree is represented by AlignNode classes.

//! Relies on Reingold-Tilford algorithm, as given in https://www.cs.unc.edu/techreports/89-034.pdf
//! closely follows C# Rachel Lim's implementation given at
//! https://rachel53461.wordpress.com/2014/04/20/algorithm-for-drawing-trees algorithm

#include <map>

namespace oac_tree_gui::algorithm
{

class AlignNode;

//! Visit node hierarchy and set initial values to node parameters.
void InitializeNodes(AlignNode& node);

//! Returns left contour of the tree.
std::map<int, double> GetLeftCountour(AlignNode& node, double mod_sum = 0.0);

//! Returns right contour of the tree.
std::map<int, double> GetRightCountour(AlignNode& node, double mod_sum = 0.0);

void CalculateInitialX(AlignNode& node);

void CheckForConflicts(AlignNode& node);

void CenterNodesBetween(AlignNode& leftNode, AlignNode& rightNode);

void CalculateFinalPositions(AlignNode& node, double mod_sum = 0.0);

//! Align nodes.
void AlignNodes(AlignNode& node);

}  // namespace oac_tree_gui::algorithm

#endif  // OAC_TREE_GUI_NODEEDITOR_ALIGN_UTILS_H_
