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

#ifndef OAC_TREE_GUI_MODEL_SETTINGS_CONTAINER_ITEM_H_
#define OAC_TREE_GUI_MODEL_SETTINGS_CONTAINER_ITEM_H_

#include <mvvm/model/compound_item.h>

#include <optional>
#include <string>

namespace oac_tree_gui
{

/**
 * @brief The SettingsContainerItem class holds settings as key/value pairs.
 *
 * Each key is a property. The property is added on the first write of the key.
 * It is used to store a mixture of widget's settings in project file.
 */
class SettingsContainerItem : public mvvm::CompoundItem
{
public:
  SettingsContainerItem();

  std::unique_ptr<SessionItem> Clone() const override;

  /**
   * @brief Returns the value of the key, or nullopt if the key is absent.
   */
  std::optional<mvvm::variant_t> GetValue(const std::string& key) const;

  /**
   * @brief Sets the value of the key.
   *
   * Adds the property if the key is absent. Replaces the data if the value type differs.
   */
  void SetValue(const std::string& key, const mvvm::variant_t& value);
};

}  // namespace oac_tree_gui

namespace mvvm
{

template <>
struct item_traits<oac_tree_gui::SettingsContainerItem>
{
  static constexpr std::string_view type_name() noexcept { return "SettingsContainer"; }
};

}  // namespace mvvm

#endif  // OAC_TREE_GUI_MODEL_SETTINGS_CONTAINER_ITEM_H_
