/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#ifndef OAC_TREE_GUI_JSON_STYLE_FACTORY_H_
#define OAC_TREE_GUI_JSON_STYLE_FACTORY_H_

//! @file
//! Collection of helper methods to create styles from JSON representation.

#include <QJsonObject>

namespace oac_tree_gui::style
{

class GraphicsViewStyle;
class GraphicsSceneStyle;

/**
 * @brief Populates existing GraphicsViewStyle from JSON object representing application style.
 */
void PopulateStyleFromJSON(const QJsonObject& json, GraphicsViewStyle& style);

/**
 * @brief Populates existing GraphicsSceneStyle from JSON object representing application style.
 */
void PopulateStyleFromJSON(const QJsonObject& json, GraphicsSceneStyle& style);

}  // namespace oac_tree_gui::style

#endif  // OAC_TREE_GUI_JSON_STYLE_FACTORY_H_
