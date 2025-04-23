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

#include "scene_utils.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/model/sequencer_item_helper.h>
#include <oac_tree_gui/model/sequencer_model.h>
#include <oac_tree_gui/transform/transform_from_domain.h>

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/widgets/widget_utils.h>

#include <QLinearGradient>
#include <QRectF>
#include <cctype>

namespace
{
const double kWindowSize = 16000;
}

namespace oac_tree_gui
{

QSizeF GetGraphicsViewportSize()
{
  return {kWindowSize, kWindowSize};
}

QPointF GetGraphicsViewportOrigin()
{
  return {-kWindowSize / 2.0, 0.0};
}

QPointF GetGraphicsViewportCenter()
{
  // y-axis on graphics scene is pointing downwards. Our scene's corners looks like this:
  // (-window_size/2, 0)            (window_size/2, 0)
  // (-window_size/2, window_size)  (window_size/2, window_size)
  const auto origin = GetGraphicsViewportOrigin();
  const auto size = GetGraphicsViewportSize();
  return {origin.x() + size.width() / 2, origin.y() + size.height() / 2};
}

double GetAlignmentGridWidth()
{
  return oac_tree_gui::ConnectableViewRectangle().width() * 1.4;
}

double GetAlignmentGridHeight()
{
  return oac_tree_gui::ConnectableViewRectangle().height() * 1.4;
}

QGradient ConnectableViewGradient(const QColor& color, const QRectF& rect)
{
  QLinearGradient result(rect.x() + rect.width() / 2, rect.y(), rect.x() + rect.width() / 2,
                         rect.y() + rect.height());
  result.setColorAt(0, color);
  result.setColorAt(0.5, color.lighter(150));
  result.setColorAt(1, color);
  return result;
}

QRectF ConnectableViewRectangle()
{
  // make size of rectangle depending on 'M'-letter size to address scaling issues
  static QRectF result = QRectF(0, 0, mvvm::utils::UnitSize(8.2), mvvm::utils::UnitSize(10.0));
  return result;
}

std::vector<QPointF> GetPositions(const QPointF& reference, int n_points, double width)
{
  std::vector<QPointF> result;
  if (n_points < 0)
  {
    throw std::runtime_error("Error in GetPositions: invalid number of points");
  }

  double xpos = reference.x() - (n_points - 1) * width / 2;
  for (int i = 0; i < n_points; ++i)
  {
    result.emplace_back(QPointF(xpos, reference.y()));
    xpos += width;
  }
  return result;
}

QColor GetBaseColor(const InstructionItem* instruction)
{
  if (IsDecoratorInstruction(instruction))
  {
    return {"lightseagreen"};
  }

  if (IsCompoundInstruction(instruction))
  {
    return {"royalblue"};
  }

  return {Qt::lightGray};
}

std::string InsertSpaceAtCamelCase(std::string str)
{
  if (str.empty())
  {
    return {};
  }

  auto pos = std::next(str.begin());
  while (pos != str.end())
  {
    // find next lower-case char followed by upper-case char
    if (std::islower(*(pos - 1)) && std::isupper(*pos))
    {
      pos = str.insert(pos, ' ');
    }
    ++pos;
  }

  return str;
}

double GetInstructionDropOffset()
{
  const double shift_of_child_wrt_parent{10};
  return shift_of_child_wrt_parent;
}

QPointF GetNodeDropPosition(const QPointF& scene_pos)
{
  static const QRectF view_bbox = ConnectableViewRectangle();
  // we want the center of instruction node to be right under the coursor tip
  return {scene_pos.x() - (view_bbox.width() / 2), scene_pos.y() - (view_bbox.height() / 2)};
}

}  // namespace oac_tree_gui
