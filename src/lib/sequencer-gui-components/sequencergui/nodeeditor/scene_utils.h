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

#ifndef SEQUENCERGUI_NODEEDITOR_SCENE_UTILS_H_
#define SEQUENCERGUI_NODEEDITOR_SCENE_UTILS_H_

//! Collection of utility functions for graphics scene.

#include <memory>
#include <vector>
#include <string>

class QGradient;
class QColor;
class QRectF;
class QPointF;
class QString;
class QSizeF;

namespace sequencergui
{
class InstructionContainerItem;
class InstructionItem;
class SequencerModel;

//! Returns maximum size of graphics viewport.
QSizeF GetGraphicsViewportSize();

//! Returns origin of graphics viewport.
QPointF GetGraphicsViewportOrigin();

//! Returns initial center of graphics viewport.
QPointF GetGraphicsViewportCenter();

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

//! Returns base color of given instruction
QColor GetBaseColor(const InstructionItem* instruction);

//! Adds InstructionItem to the `model` for a  given `domain_type`, and returns the result to the
//! user. If the model doesn't have given InstructionItem registered, create UnknownInstructionItem
//! instead.
InstructionItem* AddSingleInstruction(SequencerModel* model, InstructionContainerItem* container,
                                      const std::string& domain_type);

InstructionItem* AddAggregate(SequencerModel* model, InstructionContainerItem* container,
                              const std::string& aggregate_name);

//! Returns a string with space inserted at word boundaries.
//! "CamelCase" -> "Cammel Case"
std::string InsertSpaceAtCamelCase(std::string str);

}  // namespace sequencergui

#endif  // SEQUENCERGUI_NODEEDITOR_SCENE_UTILS_H_
