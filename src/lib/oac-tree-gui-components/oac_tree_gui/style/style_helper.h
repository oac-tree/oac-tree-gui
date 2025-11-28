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

#include <oac_tree_gui/style/json_style_factory.h>

#include <mvvm/style/color_flavor.h>

#include <QJsonObject>
#include <QString>
#include <QStringList>

class QIcon;

namespace oac_tree_gui
{

/**
 * @brief Size of main toolbar icons.
 */
QSize ToolBarIconSize();

/**
 * @brief Size of narrow toolbar icons.
 */
QSize NarrowToolBarIconSize();

/**
 * @brief Finds an icon with the given name and dark/light color flavor.
 *
 * Example: FindIcon("animation", AppIconColorFlavor::kForDarkThemes)
 * Is assumes an existence of animation.svg file in the resource folder of sequencer package.
 */
QIcon FindIcon(const QString& icon_name, mvvm::ColorFlavor color_flavor = mvvm::ColorFlavor::kAuto);

/**
 * @brief Loads JSON from resource with the given name.
 *
 * It has to be a file on disk, or binary resource defined  compiled into the library.
 * @see resources/oac_tree_icons.qrc
 */
QJsonObject LoadJsonFromResource(const QString& name);

/**
 * @brief Loads JSON object representing default style for the given dark/light color.
 */
QJsonObject LoadDefaultJsonStyle(mvvm::ColorFlavor color_flavor);

/**
 * @brief Validates that the given JSON object contains all expected keys in the given group.
 *
 * If some keys are missing, an exception is thrown.
 */
void ValidateStyleKey(const QJsonObject& json, const QString& group,
                      const QStringList& expected_keys);

/**
 * @brief Creates style of the give type for given theme flavor (dark/light/unspecified).
 */
template <typename StyleT>
StyleT CreateStyleFromResource(mvvm::ColorFlavor color_flavor = mvvm::ColorFlavor::kAuto)
{
  auto json = LoadDefaultJsonStyle(color_flavor);
  StyleT result;
  style::PopulateStyleFromJSON(json, result);
  return result;
}

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_STYLE_STYLE_HELPER_H_
