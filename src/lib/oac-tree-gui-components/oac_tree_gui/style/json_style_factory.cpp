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

#include "json_style_factory.h"

#include "style_helper.h"
#include "style_resource_constants.h"

namespace oac_tree_gui::style
{

void PopulateStyleFromJSON(const QJsonObject &json, GraphicsViewStyle &style)
{
  ValidateStyleKey(json, NodeGraphicsViewStyleKey,
                   {RenderBackgroundKey, BackgroundColorKey, FineGridColorKey, CoarseGridColorKey});

  const QJsonValue node_style_values = json[NodeGraphicsViewStyleKey];

  QJsonObject obj = node_style_values.toObject();

  style.render_background = obj[RenderBackgroundKey].toBool();
  style.background_color = QColor(obj[BackgroundColorKey].toString());
  style.fine_grid_size = obj[FineGridSizeKey].toInt();
  style.fine_grid_color = QColor(obj[FineGridColorKey].toString());
  style.coarse_grid_size = obj[CoarseGridSizeKey].toInt();
  style.coarse_grid_color = QColor(obj[CoarseGridColorKey].toString());
}

}  // namespace oac_tree_gui::style
