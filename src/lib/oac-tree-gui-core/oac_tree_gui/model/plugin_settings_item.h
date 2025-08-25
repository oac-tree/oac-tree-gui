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

#ifndef OAC_TREE_GUI_MODEL_PLUGIN_SETTINGS_ITEM_H_
#define OAC_TREE_GUI_MODEL_PLUGIN_SETTINGS_ITEM_H_

#include <mvvm/model/compound_item.h>

namespace oac_tree_gui
{

/**
 * @brief The PluginSettingsItem class holds settings related to plugin load.
 */
class PluginSettingsItem : public mvvm::CompoundItem
{
public:
  PluginSettingsItem();

  static std::string GetStaticType();

  std::unique_ptr<SessionItem> Clone() const override;

  /**
   * @brief Checks if settings requires to load plugins from the list with directories.
   */
  bool UsePluginDirList() const;

  /**
   * @brief Sets whether to take into account the list of directories while loading plugins.
   */
  void SetUsePluginDirList(bool value);

  /**
   * @brief Returns the list of directories where plugins can be found.
   */
  std::vector<std::string> GetPluginDirList() const;

  /**
   * @brief Sets the list of directories where plugins can be found.
   */
  void SetPluginDirList(const std::vector<std::string>& dir_list);
};

/**
 * @brief Converts vector of strings to a single string.
 *
 * This string serves as a serialization format for the vector, so that it can be used as item
 * property.
 */
std::string GetSettingStringFromVector(const std::vector<std::string>& vec);

/**
 * @brief Returns vector of strings from the single string.
 *
 * This is the inverse function to GetSettingStringFromVector.
 */
std::vector<std::string> GetVectorFromSettingString(const std::string& str);

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_MODEL_PLUGIN_SETTINGS_ITEM_H_
