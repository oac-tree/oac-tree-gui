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

#ifndef SEQUENCERGUI_NODEEDITOR_SCENEUTILS_H
#define SEQUENCERGUI_NODEEDITOR_SCENEUTILS_H

//! @file sceneutils.h
//! Collection of utility functions for graphics scene.

#include <vector>

class QGradient;
class QColor;
class QRectF;
class QPointF;

namespace sequencergui
{
class InstructionContainerItem;
class InstructionItem;

//! Returns vertical gradient to paint connectable view with given color and bounding rectangle.
QGradient ConnectableViewGradient(const QColor& color, const QRectF& rect);

//! Returns characteristic rectangle to represent connectable view on a graphics scene.
QRectF ConnectableViewRectangle();

//! Returns vector of points symmetrically distributed in a horizontal direction with
//! center at `reference`.
std::vector<QPointF> GetPositions(const QPointF& reference, int n_points, double width);

//! Returns vertical size of the alignment grid.
double GetAlignmentGridHeight();

//! Returns horizontal size of the alignment grid.
double GetAlignmentGridWidth();

//! Align instructions tree on graphics scene. When `force` is false allignment is performed only
//! for instructions with unitialised coordinates.
void AlignTree(const QPointF& reference, InstructionContainerItem* container, bool force = false);

//! Align children of given instruction on graphics scene.  When `force` is false allignment is
//! performed only for instructions with unitialised coordinates.
//! The position of parent `instruction` remains unchanged.
void AlignInstructionTree(const QPointF& reference, InstructionItem* instruction, bool force);

//! Returns base color of given instruction
QColor GetBaseColor(const InstructionItem* instruction);

}  // namespace sequi

#endif  // SEQUENCERGUI_NODEEDITOR_SCENEUTILS_H
