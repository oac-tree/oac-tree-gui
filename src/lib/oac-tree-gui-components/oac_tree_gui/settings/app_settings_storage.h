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

#ifndef OAC_TREE_GUI_SETTINGS_APP_SETTINGS_STORAGE_H_
#define OAC_TREE_GUI_SETTINGS_APP_SETTINGS_STORAGE_H_

#include <oac_tree_gui/settings/i_settings_storage.h>

#include <QString>
#include <QVariant>
#include <functional>
#include <memory>

namespace oac_tree_gui
{

/**
 * @brief The AppSettingsStorage class stores application settings via given read and write
 * functions.
 *
 * Converts keys to QString and values to QVariant. In the application the functions work with
 * QSettings, see CreateAppSettingsStorage().
 */
class AppSettingsStorage : public ISettingsStorage
{
public:
  //! A function returning the value for the key, or invalid QVariant if the key is absent.
  using read_func_t = std::function<QVariant(const QString& key)>;

  //! A function storing the value under the key.
  using write_func_t = std::function<void(const QString& key, const QVariant& value)>;

  AppSettingsStorage(read_func_t read_func, write_func_t write_func);

  /**
   * @copydoc ISettingsStorage::Read
   *
   * @throw RuntimeException if the stored value has a type unsupported by variant_t.
   */
  std::optional<mvvm::variant_t> Read(const std::string& key) const override;

  void Write(const std::string& key, const mvvm::variant_t& value) override;

private:
  read_func_t m_read_func;
  write_func_t m_write_func;
};

/**
 * @brief Creates application settings storage working with QSettings.
 */
std::unique_ptr<AppSettingsStorage> CreateAppSettingsStorage();

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_SETTINGS_APP_SETTINGS_STORAGE_H_
