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

#include "style_types.h"

#include "style_helper.h"
#include "style_resource_constants.h"

namespace oac_tree_gui::style
{

GraphicsViewStyle GraphicsViewStyle::CreateFromStyle(const QJsonObject &json)
{
  ValidateStyleKey(json, NodeGraphicsViewStyleKey,
                   {RenderBackgroundKey, BackgroundColorKey, FineGridColorKey, CoarseGridColorKey});

  GraphicsViewStyle result;
  const QJsonValue node_style_values = json[NodeGraphicsViewStyleKey];

  QJsonObject obj = node_style_values.toObject();

  result.render_background = obj[RenderBackgroundKey].toBool();
  result.background_color = QColor(obj[BackgroundColorKey].toString());
  result.fine_grid_size = obj[FineGridSizeKey].toInt();
  result.fine_grid_color = QColor(obj[FineGridColorKey].toString());
  result.coarse_grid_size = obj[CoarseGridSizeKey].toInt();
  result.corase_grid_color = QColor(obj[CoarseGridColorKey].toString());

  return result;
}

}  // namespace oac_tree_gui::style
