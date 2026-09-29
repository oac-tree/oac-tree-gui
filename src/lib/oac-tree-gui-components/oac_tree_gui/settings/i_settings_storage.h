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

#ifndef OAC_TREE_GUI_SETTINGS_I_SETTINGS_STORAGE_H_
#define OAC_TREE_GUI_SETTINGS_I_SETTINGS_STORAGE_H_

#include <mvvm/core/variant.h>

#include <optional>
#include <string>

namespace oac_tree_gui
{

/**
 * @brief The ISettingsStorage class is an interface to store key/value pairs in a list-like
 * storages.
 *
 * Keys have the form "GroupA/GroupB/Key". The value is a scalar as supported by mvvm::variant_t.
 * Intended for storing simple settings objects organized into (possibly) nested simple structs.
 */
class ISettingsStorage
{
public:
  ISettingsStorage() = default;
  virtual ~ISettingsStorage() = default;

  ISettingsStorage(const ISettingsStorage&) = delete;
  ISettingsStorage& operator=(const ISettingsStorage&) = delete;
  ISettingsStorage(ISettingsStorage&&) noexcept = delete;
  ISettingsStorage& operator=(ISettingsStorage&&) noexcept = delete;

  /**
   * @brief Returns the value stored under the given key, or nullopt if the key is absent.
   */
  virtual std::optional<mvvm::variant_t> Read(const std::string& key) const = 0;

  /**
   * @brief Stores the value under the given key, replacing the previous one.
   */
  virtual void Write(const std::string& key, const mvvm::variant_t& value) = 0;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_SETTINGS_I_SETTINGS_STORAGE_H_
