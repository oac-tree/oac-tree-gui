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

#ifndef OAC_TREE_GUI_NODEEDITOR_SCENE_UTILS_H_
#define OAC_TREE_GUI_NODEEDITOR_SCENE_UTILS_H_

//! @file
//! Collection of utility functions for graphics scene.

#include <string>
#include <vector>

class QGradient;
class QColor;
class QRectF;
class QPointF;
class QString;
class QSizeF;

namespace oac_tree_gui
{

class InstructionContainerItem;
class InstructionItem;
class SequencerModel;

/**
 * @brief Returns maximum size of graphics viewport.
 */
QSizeF GetGraphicsViewportSize();

/**
 * @brief Returns origin of graphics viewport.
 */
QPointF GetGraphicsViewportOrigin();

/**
 * @brief Returns initial center of graphics viewport.
 */
QPointF GetGraphicsViewportCenter();

/**
 * @brief Returns vertical gradient to paint connectable view.
 *
 * @param color Base color of the gradient.
 * @param rect Gradient's bounding box.
 * @return Qt gradient.
 */
QGradient ConnectableViewGradient(const QColor& color, const QRectF& rect);

/**
 * @brief Returns rectangle to represent connectable view on a graphics scene.
 * @return
 */
QRectF ConnectableViewRectangle();

/**
 * @brief Returns vector of points symmetrically distributed in a horizontal direction around
 * reference point.
 *
 * @param reference Coordinate of the reference point.
 * @param n_points Number of points to generate.
 * @param width Width of the horizontal area.
 *
 * @return Coordinates of points.
 */
std::vector<QPointF> GetPositions(const QPointF& reference, int n_points, double width);

/**
 * @brief Returns vertical size of the alignment grid.
 *
 * It is used to align instruction rectangles on the graphics scene.
 */
double GetAlignmentGridHeight();

/**
 * @brief Returns horizontal size of the alignment grid.
 *
 * It is used to align instruction rectangles on the graphics scene.
 */
double GetAlignmentGridWidth();

/**
 * @brief Returns base color of given instruction
 */
QColor GetBaseColor(const InstructionItem* instruction);

/**
 * @brief Returns a string with space inserted at word boundaries.
 *
 * "CamelCase" -> "Camel Case"
 */
std::string InsertSpaceAtCamelCase(std::string str);

/**
 * @brief An offset to add to child coordinates concerning its parent.
 *
 * It is used when a child is added to a parent via the context menu. The resulting instruction
 * rectangle will be located next to its parent (slightly shifted) on the graphics scene.
 */
double GetInstructionDropOffset();

/**
 * @brief Returns drop coordinates of instruction node.
 *
 * @param scene_pos The coordinate of the scene where the user has released mouse button.
 * @return Coordinates of instruction to set.
 */
QPointF GetNodeDropPosition(const QPointF& scene_pos);

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_NODEEDITOR_SCENE_UTILS_H_
