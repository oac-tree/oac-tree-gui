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

#ifndef SEQUENCERGUI_UTILS_ALIGN_UTILS_H_
#define SEQUENCERGUI_UTILS_ALIGN_UTILS_H_

//! @file alignutils.h
//! Collection of utility function for node positioning of the general tree.
//! The tree is represented by AlignNode classes.

//! Relies on Reingold-Tilford algorithm, as given in https://www.cs.unc.edu/techreports/89-034.pdf
//! closely follows C# Rachel Lim's implementation given at
//! https://rachel53461.wordpress.com/2014/04/20/algorithm-for-drawing-trees algorithm

#include <map>

namespace sequencergui::algorithm
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

}  // namespace sequencergui::algorithm

#endif  // SEQUENCERGUI_UTILS_ALIGN_UTILS_H_
