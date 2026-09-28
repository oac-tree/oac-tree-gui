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

#ifndef OAC_TREE_GUI_SETTINGS_IN_MEMORY_SETTINGS_STORAGE_H_
#define OAC_TREE_GUI_SETTINGS_IN_MEMORY_SETTINGS_STORAGE_H_

#include <oac_tree_gui/settings/i_settings_storage.h>

#include <map>
#include <vector>

namespace oac_tree_gui
{

/**
 * @brief The InMemorySettingsStorage class keeps settings in memory.
 */
class InMemorySettingsStorage : public ISettingsStorage
{
public:
  std::optional<mvvm::variant_t> Read(const std::string& key) const override;

  void Write(const std::string& key, const mvvm::variant_t& value) override;

  void RemoveGroup(const std::string& group) override;

  /**
   * @brief Returns all stored keys in alphabetical order.
   */
  std::vector<std::string> GetKeys() const;

private:
  std::map<std::string, mvvm::variant_t> m_values;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_SETTINGS_IN_MEMORY_SETTINGS_STORAGE_H_
