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

#include "app_settings_storage.h"

#include <mvvm/providers/variant_converter.h>

#include <QSettings>

namespace oac_tree_gui
{

AppSettingsStorage::AppSettingsStorage(read_func_t read_func, write_func_t write_func)
    : m_read_func(std::move(read_func)), m_write_func(std::move(write_func))
{
}

std::optional<mvvm::variant_t> AppSettingsStorage::Read(const std::string& key) const
{
  const auto value = m_read_func(QString::fromStdString(key));
  if (!value.isValid())
  {
    return {};
  }
  return mvvm::GetStdVariant(value);
}

void AppSettingsStorage::Write(const std::string& key, const mvvm::variant_t& value)
{
  m_write_func(QString::fromStdString(key), mvvm::GetQtVariant(value));
}

std::unique_ptr<AppSettingsStorage> CreateAppSettingsStorage()
{
  auto read_func = [](const QString& key) { return QSettings().value(key); };
  auto write_func = [](const QString& key, const QVariant& value)
  { QSettings().setValue(key, value); };
  return std::make_unique<AppSettingsStorage>(read_func, write_func);
}

}  // namespace oac_tree_gui
