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

#ifndef OAC_TREE_GUI_STYLE_STYLE_HELPER_H_
#define OAC_TREE_GUI_STYLE_STYLE_HELPER_H_

#include <sup/gui/style/icon_color_flavor.h>

class QIcon;
class QString;
class QJsonObject;

namespace oac_tree_gui
{

/**
 * @brief Finds an icon with the given name and dark/light color flavor.
 *
 * Example: FindIcon("animation", AppIconColorFlavor::kForDarkThemes)
 * Is assumes an existence of animation.svg file in the resource folder of sequencer package.
 */
QIcon FindIcon(const QString& icon_name,
               sup::gui::IconColorFlavor icon_flavor = sup::gui::IconColorFlavor::kUnspecified);

/**
 * @brief Loads JSON from resource with the given name.
 *
 * It has to be a file on disk, or binary resource defined  compiled into the library.
 * @see resources/oac_tree_icons.qrc
 */
QJsonObject LoadJsonFromResource(const QString& name);

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_STYLE_STYLE_HELPER_H_
